#include "Parser.h"
#include <stdexcept>
using namespace Thought;

#include "parslets/Parslets.h"
#include "nodes/Node.h"
// #include "parslets/Precedence.h"

Parser::Parser(Lexer& l) : lex(l) {
	register_parser(Token::NAME, new NameParslet());
	register_parser(Token::NUMBER, new LiteralParslet());
	register_parser(Token::FLOAT, new LiteralParslet());
	register_parser(Token::LPAREN, new GroupParslet());
	register_parser(Token::LPAREN, new CallParslet());
	register_parser(Token::DOT, new CompoundNameParslet());
	register_parser(Token::EQUAL, new AssignmentParslet());
	register_parser(Token::ARROW, new AssignmentParslet());

	prefix(Token::DASH, Precedence::PREFIX);

	infix(Token::DASH, Precedence::SUBTRACTION, false);
	infix(Token::PLUS, Precedence::ADDITION, false);
	infix(Token::FORWARDSLASH, Precedence::DIVISION, false);
	infix(Token::ASTERISK, Precedence::MULTIPLICATION, false);
	infix(Token::CARET, Precedence::POWER, false);
}

void Parser::register_parser(Token::TokenType t, PrefixParslet* p)
{
	prefixParslets[t] = std::unique_ptr<PrefixParslet>(p);
}

void Parser::register_parser(Token::TokenType t, InfixParslet* i)
{
	infixParslets[t] = std::unique_ptr<InfixParslet>(i);
}

void Parser::prefix(Token::TokenType t, int p)
{
	register_parser(t, new PrefixOpParslet(p));
}

void Parser::infix(Token::TokenType t, int p, bool r)
{
	register_parser(t, new BinaryParslet(p, r));
}

void Parser::postfix(Token::TokenType t, int p)
{
	// register_parser(t, new PostfixOperatorParslet(p));
}

NodePtr Parser::parseNode(int precedence)
{
	Token token = consume();
    auto prefix = prefixParslets.find(token.type);

    if (prefix == prefixParslets.end()) 
    	throw std::runtime_error("Could not parse \"" + token.text + "\"");

    NodePtr left = (*prefix).second->parse(this, token);

    while(precedence < getPrecedence()) {
    	token = consume();
    	auto infix = infixParslets.find(token.type);

    	left = (*infix).second->parse(this, left, token);
    }
    return left;
}

int Parser::getPrecedence()
{
	auto parser = infixParslets.find(lookAhead(0).type);
	if (parser != infixParslets.end()) return (*parser).second->getPrecedence();

	return 0;
}

bool Parser::match(Token::TokenType type)
{
	Token tok = lookAhead(0);
	if(tok.type != type) {
		return false;
	}

	consume();
	return true;
}

Token Parser::consume(Token::TokenType type) 
{
	Token tok = lookAhead(0);
	if(tok.type != type) {
		throw std::runtime_error("Expected " + to_string(type) + " and found " + to_string(tok.type));
	}

	return consume();
}

Token Parser::consume() 
{
	lookAhead(0);
	Token ret = read.at(0);
	read.erase(read.begin());
	return ret;
}

Token Parser::lookAhead(int distance)
{
	while(distance >= read.size())
	{
		read.push_back(lex.next());
	}

	return read.at(distance);
}