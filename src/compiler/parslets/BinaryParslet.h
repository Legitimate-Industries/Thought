#ifndef THOUGHT_BINARYPARSLET_H
#define THOUGHT_BINARYPARSLET_H

#include "InfixParslet.h"

#include "../nodes/BinaryNode.h"

namespace Thought {
	class BinaryParslet;
};

class Thought::BinaryParslet : public InfixParslet {
	int prec;
	bool right;
public:
	BinaryParslet(int p, bool r) : prec(p), right(r) {}
	NodePtr parse(Parser* parser, NodePtr node, Token t) {
		NodePtr r = parser->parseNode(right ? prec - 1 : prec);
		return std::make_shared<BinaryNode>(node, r, t.text);
	}

	int getPrecedence() {
		return prec;
	}
};
 
#endif // THOUGHT_BINARYPARSLET_H