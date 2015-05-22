#include "ThoughtVM.h"
using namespace Thought;

// Macros for extracting arguments from bytecode
#define DECODE_INST(x) (x >> 24) & 0xff
#define DECODE_A(x) (x >> 16) & 0xff
#define DECODE_B(x) (x >> 8) & 0xff
#define DECODE_C(x) (x & 0xff)
#define DECODE_D(x) (x & 0xffff)

VM::VM() : first(nullptr) {
}

VM::~VM() {
	// If pure GC, we simply would GC here, but we aren't
}

void VM::markAll() {
	// Mark everything reachable

	// Marking the stack
	for(int i = 0; i < stack.size(); i++) {
		mark(stack[i]);
	}
}

void VM::mark(Value* val) {
	// Mark all proper things for the value
	if(val->mark) return;

	val->mark = 1;
}

void VM::sweep() {
	Value** live = &first;
	while(*live) {
		if(!(*live)->mark) {
			Value* unreach = *live;

			*live = unreach->next;
			// Correct the pointers
			unreach->prev->next = *live;
			(*live)->prev = unreach->prev;

			// Destroy the unreachable
			delete unreach;
		} else {
			(*live)->mark = 0;
			live = &(*live)->next;
		}
	}
}