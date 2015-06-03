#ifndef THOUGHT_LITERALPARSLET_H
#define THOUGHT_LITERALPARSLET_H

#include "PrefixParslet.h"

#include "../nodes/NumberNode.h"

namespace Thought {
	class LiteralParslet;
};

class Thought::LiteralParslet : public PrefixParslet {
	NodePtr parse(Parser* parser, Token tok) {
		switch(tok.type) {
			case Token::FLOAT:
			case Token::NUMBER:
				return std::make_shared<NumberNode>(std::atof(tok.text.c_str()));
		};
		return NodePtr(); // Error out here with a parser error!
	}
};

#endif // THOUGHT_LITERALPARSLET_H