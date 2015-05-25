#ifndef THOUGHT_TABLE_H
#define THOUGHT_TABLE_H

namespace Thought {
	class Table;

	class Value;
};

// Represents a table in Thought
class Thought::Table {
	// Make it VM-only constructable
	friend class VM;
	Value* _self; // This is the value that we are. Mainly used for parentage.

	Table(Value* v) : _self(v) {}
public:
};

#endif // THOUGHT_TABLE_H