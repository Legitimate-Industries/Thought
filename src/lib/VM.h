#ifndef THOUGHT_VM_H
#define THOUGHT_VM_H

#include "Value.h"
#include <base/Array.h>
#include <base/Stack.h>
#include <base/Block.h>
#include <base/Ref.h>
#include <string>
#include <cstdint>

namespace Thought {
class VM;
};

class Thought::VM {
	Array<Value*> stack;
	Value* first;

	struct CallFrame {
		int ip; // instruction pointer
		int bp; // base pointer (where the stack starts for us)
		// Figure out how to store and retrieve code blocks.
		Ref<Block> code; // Reference to the block of code we are
	};

	Stack<CallFrame> frames;

	int getDepth() { return frames.size(); }
	bool isDone() { return getDepth() == 0; }
	CallFrame& top() { return frames.peek(); }

	void popFrame();

	Value* retrieve(const CallFrame& frame, int p) { return stack[frame.bp + p]; }
	void store(const CallFrame& frame, int p, Value* val) { stack[frame.bp + p] = val; }

	void markAll();
	void mark(Value*);
	void sweep();

	Value* createValue(Value::Type);

	void push(Value*, int = -1);
public:
	using InstSize = std::uint32_t;
	VM();
	~VM();

	void callBlock(Ref<Block>, int);

	// "create" methods automagically push their values to the stack
	Value* createDouble(double);
	Value* createBool(bool);
	Value* createString(const char*);
	Value* createString(const char*, int);

	Value* pop(int = -1);
	void clear(); // Clears the stack

	// Call this to start the mark-sweep collector
	void gc() { markAll(); sweep(); }

	// Make the VM uncopyable, so that we don't magically get 2 VM's running their GC's on the same objects
	// That is asking for segfaults.
	VM(const VM&) = delete;
	VM& operator=(const VM&) = delete;
};

#endif // THOUGHT_VM_H