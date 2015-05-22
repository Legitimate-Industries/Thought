#ifndef THOUGHT_VM_H
#define THOUGHT_VM_H

#include "ThoughtValue.h"
#include <base/Array.h>
#include <cstdint>

namespace Thought {
class VM;
};

class Thought::VM {
	Array<Value*> stack;
	Value* first;

	void markAll();
	void mark(Value*);
	void sweep();

	// Called by both Refs and GC to get rid of unused values
	void destroy(Value*);
public:
	using InstSize = std::uint32_t;
	VM();
};

#endif // THOUGHT_VM_H