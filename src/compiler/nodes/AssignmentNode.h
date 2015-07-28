#ifndef ASSIGNMENT_NODE_H
#define ASSIGNMENT_NODE_H

#include "Node.h"

namespace Thought {
	class AssignmentNode;
}

class Thought::AssignmentNode : public Node {
	NodePtr name;
	NodePtr value;

	bool global;
public:
	AssignmentNode(NodePtr n, NodePtr v, bool g = false) {
		name = n; value = v; global = g;
	}

	void print(std::ostream& o) const {
		o << "<<";
		name->print(o);
		o << (global ? " -> " : " = ");
		value->print(o);
		o << ">>";
	}

	NodePtr getName() { return name; }
	NodePtr getValue() { return value; }
	bool isGlobal() { return global; }
};

#endif // ASSIGNMENT_NODE_H