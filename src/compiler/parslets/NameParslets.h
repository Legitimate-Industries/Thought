#ifndef THOUGHT_NAMEPARSLETS_H
#define THOUGHT_NAMEPARSLETS_H

#include "PrefixParslet.h"
#include "InfixParslet.h"
#include "Precedence.h"

#include "../nodes/NameNode.h"
#include "../nodes/CompoundNameNode.h"

namespace Thought {
	class NameParslet;
	class CompoundNameParslet;
};

class Thought::NameParslet : public PrefixParslet {
public:
	NodePtr parse(Parser* parser, Token tok) {
		return std::make_shared<NameNode>(tok.text);
	}
};

// Not just compound names, will also handle things like (3 + 4).complex
#define IS_NAME(x) dynamic_cast<NameNode*>(x.get()) || dynamic_cast<CompoundNameNode*>(x.get())
class Thought::CompoundNameParslet : public InfixParslet {
	NodePtr parse(Parser* parser, NodePtr left, Token tok) {
		auto right = parser->parseNode(getPrecedence());
		if(IS_NAME(right)) {
			return std::make_shared<CompoundNameNode>(left, right);
		}
		return NodePtr(); // Error out with a parser error!
	}

	int getPrecedence() {
		return Precedence::NAME;
	}
};

#endif // THOUGHT_NAMEPARSLETS_H