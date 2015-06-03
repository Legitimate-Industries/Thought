#ifndef THOUGHT_NUMBER_NODE_H
#define THOUGHT_NUMBER_NODE_H

#include "Node.h"

namespace Thought {
	class NumberNode;
};

class Thought::NumberNode : public Thought::Node {
	double value;
public:
	NumberNode(double v) : value(v) {}
	virtual void print(std::ostream& o) const {
		o << value;
	}
};

#endif // THOUGHT_NUMBER_NODE_H