#ifndef THOUGHT_CALLPARSLET_H
#define THOUGHT_CALLPARSLET_H

#include "InfixParslet.h"
#include "Precedence.h"

#include "../nodes/CallNode.h"

namespace Thought {
	class CallParslet;
};

class Thought::CallParslet : public InfixParslet {
	NodePtr parse(Parser* parser, NodePtr left, Token tok) {
		std::vector<NodePtr> args;

		if(!parser->match(Token::RPAREN))
		{
			do {
				args.push_back(parser->parseNode());
			} while(parser->match(Token::COMMA));
		}

		return std::make_shared<CallNode>(left, args);
	}

	int getPrecedence() {
		return Precedence::CALL;
	}
};

#endif // THOUGHT_CALLPARSLET_H