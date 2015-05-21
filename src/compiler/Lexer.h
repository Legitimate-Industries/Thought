#ifndef AMAC_LEXER_H
#define AMAC_LEXER_H

#include <string>
#include <map>
#include <vector>
#include <exception>

struct Token
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
		// SEMICOLON,
		COLON,
		LPAREN,
		RPAREN,
		EQUAL,
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

		STRING,

		UNKNOWN,
		EOI
	} type;
	std::string text;
	int start;
	int rel_start;
	int line;
};

std::string to_string(Token::TokenType);
std::string to_string(Token);

class Lexer
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

struct LexerError : public std::exception
{
	int location;
	std::string token;
	std::string msg;

	LexerError(int l, std::string n, std::string m) : location(l), token(n), msg(m) {}
	const char* what() const noexcept (true) override;
};

#endif // AMAC_LEXER_H