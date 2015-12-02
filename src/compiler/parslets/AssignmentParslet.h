#ifndef ASSIGNMENT_PARSLET_H
#define ASSIGNMENT_PARSLET_H

#include "InfixParslet.h"

#include "../nodes/AssignmentNode.h"
#include "../nodes/NameNode.h"
#include "../nodes/CompoundNameNode.h"

#include "Precedence.h"

// Name helper macro
#define IS_NAME(x) dynamic_cast<NameNode*>(x.get()) || dynamic_cast<CompoundNameNode*>(x.get())

namespace Thought {
	class AssignmentParslet;
};

class Thought::AssignmentParslet : public InfixParslet {
	NodePtr parse(Parser* parser, NodePtr left, Token tok) {
		std::cout << tok.text << std::endl;

		auto val = parser->parseNode(getPrecedence());
		if(IS_NAME(left)) {
			return std::make_shared<AssignmentNode>(left, val, tok.text == "->");
		}
		return NodePtr(); // Make a compile error mechanism!
	}

	int getPrecedence() {
		return Precedence::ASSIGNMENT;
	}
};

#endif // ASSIGNMENT_PARSLET_H