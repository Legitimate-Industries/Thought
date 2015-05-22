#ifndef THOUGHT_VALUE_H
#define THOUGHT_VALUE_H

#include <cstdint>

namespace Thought {
	struct Value;
};

class Thought::Value {
	// Make values only constructable by a VM Object
	friend class VM;
	Value() : refs(1) {}
public:
	enum Type {
		DOUBLE,
		BOOL,
		STRING,
		TABLE,
		BLOCK, // Code-side function
		NATIVE, // C++-side function
		ARRAY
	} type;

	union {
		double v_double;
		bool v_bool;
	};

	// Used for GC
	int mark;

	Value* prev;
	Value* next;

	void retain() {
		refs++;
	}

	void release() {
		refs--;
		if(refs <= 0)
			delete this;
	}
private:
	std::size_t refs;
};

#endif // THOUGHT_VALUE_H