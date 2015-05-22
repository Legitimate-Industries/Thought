#ifndef THOUGHTC_LEXER_H
#define THOUGHTC_LEXER_H

#include <string>
#include <map>
#include <vector>
#include <exception>

namespace Thought {
	struct Token;
	class Lexer;
	class LexerError;
};

struct Thought::Token
{
	enum TokenType
	{
		NUMBER,
		FLOAT,
		DOT,
		DASH,
		PLUS,
		PLUSPLUS,
		ASTERISK,
		FORWARDSLASH,
		SEMICOLON,
		COLON,
		LPAREN,
		RPAREN,
		EQUAL,
		EQUALEQUAL,
		NOTEQUAL,
		NAME,
		KEYWORD,
		CARET,
		COMMA,
		GREATER,
		LBRACE,
		RBRACE,
		LBRACKET,
		RBRACKET,
		ARROW,
		EXCLAIM,

		FUNCTION,
		STRING,
		LINE_END,

		UNKNOWN,
		EOI
	} type;
	std::string text;
	int start;
	int rel_start;
	int line;
};

namespace Thought {
	std::string to_string(Token::TokenType);
	std::string to_string(Token);
};

class Thought::Lexer
{
	std::string text;
	int index;

	int lineCount;
	int lcharCount;

	std::map<char, Token::TokenType> operators;
	std::vector<char> keyword_char;
	std::map<std::string, Token::TokenType> keywords;

	std::vector<char> symbol_char;
	std::map<std::string, Token::TokenType> symbols;
public:
	Lexer(std::string);
	bool has_next();
	Token next();

	void reset() { index = 0; lineCount = 1; }
};

struct Thought::LexerError : public std::exception
{
	int location;
	std::string token;
	std::string msg;

	LexerError(int l, std::string n, std::string m) : location(l), token(n), msg(m) {}
	const char* what() const noexcept (true) override;
};

#endif // THOUGHTC_LEXER_H