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
		unsigned int ip; // instruction pointer
		unsigned int bp; // base pointer (where the stack starts for us)
		// Figure out how to store and retrieve code blocks.
		Ref<Block> code; // Reference to the block of code we are

		CallFrame() {}
		CallFrame(unsigned int i, unsigned int b, Ref<Block> c) : ip(i), bp(b), code(c) {}
	};

	Stack<CallFrame> frames;

	int getDepth() { return frames.size(); }
	bool isDone() { return getDepth() == 0; }
	CallFrame& top() { return frames.peek(); }

	void popFrame();

	Value* retrieve(const CallFrame& frame, int p) { return stack[frame.bp + p]; }
	void store(const CallFrame& frame, int p, Value* val) { stack.insert(val, frame.bp + p); }

	void markAll();
	void mark(Value*);
	void sweep();

	Value* createValue(Value::Type);
	void run(); // Runs the frame stack until isDone
public:
	using InstSize = std::uint32_t;
	VM();
	~VM();

	void callBlock(Ref<Block>, unsigned int);

	Value* createDouble(double);
	Value* createBool(bool);
	Value* createString(const char*);
	Value* createString(const char*, int);
	Value* createTable(Value* = nullptr);

	Value* pop(int = -1);
	Value* peek(int = -1);
	void push(Value*, int = -1);
	void clear(); // Clears the stack

	// Call this to start the mark-sweep collector
	void gc() { markAll(); sweep(); }

	// Make the VM uncopyable, so that we don't magically get 2 VM's running their GC's on the same objects
	// That is asking for segfaults.
	VM(const VM&) = delete;
	VM& operator=(const VM&) = delete;

	void dump(); // Debug dumping of the current state of the VM to cout
};

#endif // THOUGHT_VM_H