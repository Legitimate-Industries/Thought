#ifndef AMAC_NODE_H
#define AMAC_NODE_H

#include <iostream>
#include "../Visitor.h"
#include <memory>
#include <string>

enum class NodeType
{
	ASSIGN,
	BLOCK,
	CALL,
	DOUBLE,
	INTEGER,
	NAME,
	OPERATOR,
	POSTFIX,
	PREFIX,
	STRING,
	DECLARATION
};

#define NTOS(n) case NodeType::n: return #n;
static std::string nt_to_string(NodeType n) {
	switch(n) {
		NTOS(ASSIGN)NTOS(BLOCK)NTOS(CALL)NTOS(DOUBLE)NTOS(INTEGER)NTOS(NAME)
		NTOS(OPERATOR)NTOS(POSTFIX)NTOS(PREFIX)NTOS(STRING)NTOS(DECLARATION)
	};
}
#undef NTOS

class Node
{
public:
	virtual ~Node() {}
	virtual void print(std::ostream&) const = 0;
	virtual void visit(Visitor*) = 0;
	virtual NodeType getType() = 0;
};

using NodePtr = std::shared_ptr<Node>;

#endif // AMAC_NODE_H