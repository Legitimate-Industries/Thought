#ifndef THOUGHT_GROUPPARSLET_H
#define THOUGHT_GROUPPARSLET_H

#include "PrefixParslet.h"

namespace Thought {
	class GroupParslet;
};

class Thought::GroupParslet : public PrefixParslet
{
public:
	NodePtr parse(Parser* parser, Token token) {
		NodePtr node = parser->parseNode();
		parser->consume(Token::RPAREN);
		return node;
	}
};

#endif // THOUGHT_GROUPPARSLET_H