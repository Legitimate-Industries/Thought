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

void VM::markAll() {
	// Mark everything reachable
}

void VM::mark(Value* val) {
	// Mark all proper things for the value
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
			destroy(unreach);
		} else {
			(*live)->mark = 0;
			live = &(*live)->next;
		}
	}
}

void VM::destroy(Value* val) {
	delete val;
	val = nullptr;
}