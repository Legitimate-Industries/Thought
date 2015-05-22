#ifndef THOUGHT_STACK_H
#define THOUGHT_STACK_H

#include "Array.h"
#include <cassert>

namespace Thought {
	template<typename T>
	class Stack;
};

// TODO: move from asserts to exceptions
#define ASSERT_RANGE(v, m) assert(v >= 0 && v < m);

template<typename T>
class Thought::Stack {
	Array<T> stack;
public:
	int size() { return stack.size(); }
	bool isEmpty() { return stack.isEmpty(); }

	void push(const T& item) {
		stack.add(item);
	}

	T pop() {
		assert(!isEmpty() && "Cannot pop an empty stack");
		T item = stack[-1];
		stack.removeAt(-1);
		return item;
	}

	T& peek() {
		assert(!isEmpty() && "Cannot peek into an empty stack");
		return stack[-1];
	}

	T& operator[](int idx) {
		ASSERT_RANGE(idx, size());
		return stack[-1 - idx];
	}

	const T& operator[](int idx) const {
		ASSERT_RANGE(idx, size());
		return stack[-1 - idx];
	}

	T* begin() { return stack.begin(); }
	T* end() { return stack.end(); }
};

#undef ASSERT_RANGE

#endif // THOUGHT_STACK_H