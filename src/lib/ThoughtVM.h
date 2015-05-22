#ifndef THOUGHT_VM_H
#define THOUGHT_VM_H

#include "ThoughtValue.h"
#include <base/Array.h>
#include <base/Block.h>
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
public:
	using InstSize = std::uint32_t;
	VM();
	~VM();

	// Make the VM uncopyable, so that we don't magically get 2 VM's running their GC's on the same objects
	// That is asking for segfaults.
	VM(const VM&) = delete;
	VM& operator=(const VM&) = delete;
};

#endif // THOUGHT_VM_H