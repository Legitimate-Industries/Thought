#include "VM.h"
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
	// Actually, forget the user. They shouldn't keep our values after we die.
	// To forcefully collect all objects, we simply sweep without marking anything.
	// sweep();
}

Value* VM::createValue(Value::Type type) {
	Value* newVal = new Value;
	newVal->mark = 0;
	newVal->type = type;

	newVal->next = first;
	if(first != nullptr)
		first->prev = newVal;
	newVal->prev = nullptr;

	first = newVal;
	return newVal;
}

Value* VM::createDouble(double val) {
	auto newval = createValue(Value::DOUBLE);
	newval->v_double = val;
	return newval;
}

Value* VM::createBool(bool val) {
	auto newval = createValue(Value::BOOL);
	newval->v_bool = val;
	return newval;
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

			// Correct the pointers
			if(unreach->next != nullptr)
				unreach->next->prev = unreach->prev;
			if(unreach->prev != nullptr)
				unreach->prev->next = unreach->next;

			*live = unreach->next;

			// Destroy the unreachable
			delete unreach;
		} else {
			(*live)->mark = 0;
			live = &(*live)->next;
		}
	}
}

void VM::callBlock(Ref<Block> block, int args) {
	frames.push(CallFrame {0, stack.size() - args, block});
}

void VM::popFrame() {
	frames.pop();
}