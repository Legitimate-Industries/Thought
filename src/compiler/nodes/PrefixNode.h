#ifndef THOUGHT_PREFIXNODE_H
#define THOUGHT_PREFIXNODE_H

#include "Node.h"

namespace Thought {
	class PrefixNode;
};

class Thought::PrefixNode : public Node {
	std::string op;
	NodePtr value;
public:
	PrefixNode(NodePtr n, std::string o) : value(n), op(o) {}
	virtual void print(std::ostream& o) const {
		o << op;
		value->print(o);
	}
};

#endif // THOUGHT_PREFIXNODE_H