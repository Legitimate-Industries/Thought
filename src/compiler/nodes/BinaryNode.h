#ifndef THOUGHT_BINARY_NODE_H
#define THOUGHT_BINARY_NODE_H

#include "Node.h"
#include <string>

namespace Thought {
	class BinaryNode;
};

// Stores binary operators of all types
class Thought::BinaryNode : public Thought::Node {
	std::string op;
	NodePtr left;
	NodePtr right;
public:
	BinaryNode(NodePtr l, NodePtr r, std::string o) : left(l), right(r), op(o) {}
	virtual void print(std::ostream& o) const {
		o << "(";
		left->print(o);
		o << " " << op << " ";
		right->print(o);
		o << ")";
	}
};

#endif // THOUGHT_BINARY_NODE_H