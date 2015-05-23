#ifndef THOUGHT_VALUEREF_H
#define THOUGHT_VALUEREF_H

#include <cstdint>

// Do we move reference count out of the values, and into the references?

namespace Thought {
	class ValueRef;
	class ValueWeakRef;

	class ValueBaseRef;

	class Value;
};

// Provides a value holding and destroying mechanism, along with reference counting
class Thought::ValueBaseRef {
protected:
	ValueBaseRef(Value* v) : value(v), refs(0) {}
	Value* value;

	std::size_t refs;
public:
	virtual ~ValueBaseRef() {
		destroyValue();
	}

	void destroyValue() {
		if(value != nullptr) {
			value->destroy();
			value = nullptr;
		}
	}

	void retain() {
		refs++;
	}

	void release() {
		refs--;
		if(refs <= 0)
			destroyValue();
	}
};

class Thought::ValueRef : public Thought::ValueBaseRef {
public:
	ValueRef(Value* v) : ValueBaseRef(v) { retain(); }
};

class Thought::ValueWeakRef : public Thought::ValueBaseRef {
public:
	ValueWeakRef(Value* v) : ValueBaseRef(v) {}
};

#endif // THOUGHT_VALUEREF_H