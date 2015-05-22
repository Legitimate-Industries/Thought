#ifndef THOUGHT_VALUE_H
#define THOUGHT_VALUE_H

#include <cstdint>

namespace Thought {
	struct Value;
};

struct Thought::Value {
	Value() : refs(0) {}
	~Value() { refs = 0; }

	// Used for GC
	int mark;

	Value* prev;
	Value* next;

	void addRef() {
		refs++;
	}

	void removeRef() {
		refs--;
		if(refs <= 0)
			delete this;
	}
private:
	std::size_t refs;
};

#endif // THOUGHT_VALUE_H