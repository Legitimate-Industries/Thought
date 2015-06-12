#ifndef THOUGHTC_PARSER_H
#define THOUGHTC_PARSER_H

#include "Lexer.h"
#include "nodes/Node.h"
#include "parslets/PrefixParslet.h"
#include "parslets/InfixParslet.h"
#include <vector>
#include <map>
#include <memory>

namespace Thought {
	class Parser;
};

class Thought::Parser
{
	Lexer& lex;
	std::vector<Token> read;

	std::map<Token::TokenType, std::unique_ptr<PrefixParslet>> prefixParslets;
	std::map<Token::TokenType, std::unique_ptr<InfixParslet>> infixParslets;

	void register_parser(Token::TokenType, PrefixParslet*);
	void register_parser(Token::TokenType, InfixParslet*);
	void prefix(Token::TokenType, int);
	void infix(Token::TokenType, int, bool);
	void postfix(Token::TokenType, int);

	int getPrecedence();

	void eatLines();
public:
	Parser(Lexer&);

	NodePtr parseNode() { return parseNode(0); }
	NodePtr parseNode(int);

	bool match(Token::TokenType);
	Token consume(Token::TokenType);
	Token consume();
	Token lookAhead(int distance);
};

#endif // THOUGHTC_PARSER_H