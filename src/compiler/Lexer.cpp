#include "Lexer.h"
#include <iostream>
#include <algorithm>
using namespace Thought;

std::string Thought::to_string(Token::TokenType t)
{
	#define TOK_CASE(tok) case Token::tok: return #tok;
	switch(t)
	{
		TOK_CASE(NUMBER)
		TOK_CASE(FLOAT)
		TOK_CASE(STRING)
		TOK_CASE(DOT)
		TOK_CASE(DASH)
		TOK_CASE(PLUS)
		TOK_CASE(ASTERISK)
		TOK_CASE(FORWARDSLASH)
		TOK_CASE(LBRACKET)
		TOK_CASE(RBRACKET)
		TOK_CASE(EQUAL)
		TOK_CASE(NAME)
		TOK_CASE(KEYWORD)
		TOK_CASE(COLON)
		TOK_CASE(LPAREN)
		TOK_CASE(RPAREN)
		TOK_CASE(CARET)
		TOK_CASE(COMMA)
		TOK_CASE(GREATER)
		TOK_CASE(LBRACE)
		TOK_CASE(RBRACE)
		TOK_CASE(ARROW)
		TOK_CASE(EXCLAIM)
		TOK_CASE(PLUSPLUS)
		TOK_CASE(NOTEQUAL)

		TOK_CASE(FUNCTION)

		TOK_CASE(LINE_END)
		TOK_CASE(EOI)
		default: return "UNKNOWN";
	}
}

std::string Thought::to_string(Token t) {
	return "Token " + to_string(t.type) + ": " + t.text + " Source:" + std::to_string(t.start) + "[" +
		std::to_string(t.rel_start) + ":L" + std::to_string(t.line) + "]";
}

const char* LexerError::what() const noexcept (true)
{
	return std::string("Error[" + msg + "] at " + std::to_string(location) + "[\"" + token + "\"]").c_str();
}

Lexer::Lexer(std::string t) : text(t), index(0), lineCount(1), lcharCount(0) {
	operators['.'] = Token::DOT;
	operators['-'] = Token::DASH;
	operators['+'] = Token::PLUS;
	operators['*'] = Token::ASTERISK;
	operators['/'] = Token::FORWARDSLASH;
	operators['='] = Token::EQUAL;
	operators[':'] = Token::COLON;
	operators['['] = Token::LBRACKET;
	operators[']'] = Token::RBRACKET;
	operators['('] = Token::LPAREN;
	operators[')'] = Token::RPAREN;
	operators['^'] = Token::CARET;
	operators[','] = Token::COMMA;
	operators['>'] = Token::GREATER;
	operators['{'] = Token::LBRACE;
	operators['}'] = Token::RBRACE;
	operators['!'] = Token::EXCLAIM;
	operators[';'] = Token::SEMICOLON;

	#define ADD_KEYWORD(word, type) keywords[word] = type;
	ADD_KEYWORD("fun", Token::FUNCTION);
	/*ADD_KEYWORD("def", Token::KEYWORD);
	ADD_KEYWORD("private", Token::KEYWORD);
	ADD_KEYWORD("public", Token::KEYWORD);

	ADD_KEYWORD("var", Token::VAR);

	ADD_KEYWORD("class", Token::KEYWORD);
	ADD_KEYWORD("packet", Token::KEYWORD);*/

	#define ACCEPT_CHAR(chr) keyword_char.push_back(chr);
	ACCEPT_CHAR('_');
	ACCEPT_CHAR('!');
	ACCEPT_CHAR('?');
	/*ACCEPT_CHAR('$');
	ACCEPT_CHAR('@');*/

	#define ADD_SYMBOL(sym, type) symbols[sym] = type;
	ADD_SYMBOL("->", Token::ARROW);
	// ADD_SYMBOL("++", Token::PLUSPLUS);
	ADD_SYMBOL("==", Token::EQUALEQUAL);
	ADD_SYMBOL("!=", Token::NOTEQUAL);

	#define ACCEPT_CHAR_SYM(chr) symbol_char.push_back(chr);
	ACCEPT_CHAR_SYM('-');
	ACCEPT_CHAR_SYM('>');
	ACCEPT_CHAR_SYM('=');
	ACCEPT_CHAR_SYM('!');
	// ACCEPT_CHAR_SYM('+');
}

bool Lexer::has_next() {
	return index < text.size();
}

Token Lexer::next()
{
	while(index < text.length())
	{
		Token tok {Token::EOI, "", index, lcharCount, lineCount};
		auto current = [this] () { return text[index]; };
		auto next = [this, &current] () { lcharCount++; return (has_next() ? text[index++] : current()); };
		auto prev = [this, &current] () { return (index > 0 ? text[index--] : current()); };
		auto peek = [this, &current] () { return (has_next() ? text[index + 1] : current()); };

		auto match = [this] (char l, char r) { return l == r; };
		auto match_vector = [this] (char l, std::vector<char> r) { 
			return std::find(r.begin(), r.end(), l) != r.end(); 
		};
		auto match_string = [this] (char l, std::string r) { return r.find(l) != std::string::npos; };
		auto eoi = [this] () { return index == text.size(); };

		char c = next();

		if(isblank(c))
			continue;

		if(c == '\n') {
			tok.type = Token::LINE_END;
			lineCount++;
			lcharCount = 0;
			// continue;
			return tok;
		}

		if(isdigit(c)) {
			tok.type = Token::NUMBER;
			std::string str;
			str += c;
			while(isdigit(current()) && index < text.length()) {
				str += next();
			}
			if(match_string(current(), ".e")) {
				tok.type = Token::FLOAT;
				str += next();
				if(match_string(current(), "-")) {
					str += next();
				}
				while(isdigit(current()) && index < text.length()) {
					str += next();
				}
			}
			// prev();
			tok.text = str;
			return tok;
		} else if (isalpha(c)) {
			tok.type = Token::NAME;
			std::string str; str += c;
			while(isalnum(current()) || match_vector(current(), keyword_char)) {
				str += next();
			}
			// prev(); // Backtrack, as we increment PAST the invalid character
			if(keywords.find(str) != keywords.end()) {
				tok.type = keywords.find(str)->second;
			}
			tok.text = str;
			return tok;
		} else if(c == '\"') {
			tok.type = Token::STRING;

			std::string str = "";
			bool in_escape = false;
			std::map<char, char> escape_convert;
			escape_convert['n'] = '\n';
			escape_convert['t'] = '\t';
			escape_convert['s'] = ' ';
			escape_convert['"'] = '\"';
			escape_convert['\\'] = '\\';
			do {
				if(!has_next())
					throw std::runtime_error("Unterminated string literal");
				char x = current();
				if(!in_escape)
				{
					if(x == '\\') {
						in_escape = true;
					}
					else { str += x; }
				} else {
					if(escape_convert.find(x) != escape_convert.end()) {
						str += escape_convert.find(x)->second;
						in_escape = false;
					} else {
						throw std::runtime_error(std::string("Invalid escape sequence \\") + x);
					}
				}
			} while(next() != '\"');

			tok.text = str.substr(0, str.size() - 1);
			return tok;
		} else if(match_vector(c, symbol_char) || operators.find(c) != operators.end()) {
			std::string s; s += c;
			while(match_vector(current(), symbol_char)) {
				s += next();
			}

			// std::cout << s << ":" << c << std::endl;

			if(symbols.find(s) != symbols.end()) {
				tok.type = symbols.find(s)->second;
				tok.text = s;
				return tok;
			} else if(operators.find(c) != operators.end()) {
				tok.type = operators.find(c)->second;
				tok.text = std::string(&c, 1);
				return tok;
			} else {
				throw LexerError(index - 1, s, "Invalid token");
			}
		} else if(c == '#') {
			// Multi-line comment
			if((c = next()) == '#') {
				std::string acc = "";
				// TODO: Find a better way to do this...
				while(true)
				{
					c = next();
					if(c == '\n') {
						lineCount++;
						lcharCount = 0;
					}
					acc += c;
					if(acc.size() > 2) {
						acc = acc.substr(1);
						if(acc == "##" || eoi())
							break;
					}
				}
				continue;
			}
			// One line comment
			while((c = next()) != '\n' && c != '\0') {
			}
			// Do the line thing (\n)
			lineCount++;
			lcharCount = 0;
		} else {
			std::string s; s += c;
			throw LexerError(index - 1, s, "Invalid token");
		}
		// c;
	}

	return Token {Token::EOI, "", index};
}