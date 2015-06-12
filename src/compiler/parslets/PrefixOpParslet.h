#ifndef THOUGHT_PREFIXOPPARSLET_H
#define THOUGHT_PREFIXOPPARSLET_H

#include "PrefixParslet.h"

#include "../nodes/PrefixNode.h"

namespace Thought {
	class PrefixOpParslet;
};

class Thought::PrefixOpParslet : public PrefixParslet {
	int prec;
public:
	PrefixOpParslet(int p) : prec(p) {}
	NodePtr parse(Parser* parser, Token tok) {
		auto v = parser->parseNode(prec);
		return std::make_shared<PrefixNode>(v, tok.text);
	}
};

#endif // THOUGHT_PREFIXOPPARSLET_H