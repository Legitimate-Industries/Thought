#ifndef THOUGHT_VALUE_H
#define THOUGHT_VALUE_H

namespace Thought {
	struct Value;
};

struct Thought::Value {

	// Used for GC
	int mark;

	Value* prev;
	Value* next;
};

#endif // THOUGHT_VALUE_H