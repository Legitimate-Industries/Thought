#ifndef THOUGHT_REF_H
#define THOUGHT_REF_H

namespace Thought {
	template<typename T>
	class Ref;
};

// Reference-linked wrapper for an object
template<typename T>
class Thought::Ref {
	T* obj;
	mutable const Ref<T>* mPrev;
	mutable const Ref<T>* mNext;

	void link(const Ref<T>& other) {
		if(!other.isNull()) {
			obj = other.obj;

			mPrev = other.mPrev;
			mNext = &other;

			other.mPrev = this;
			mPrev->mNext = this;
		}
	}
public:
	Ref() : Ref(nullptr) {}
	explicit Ref(T* o) : obj(o), mPrev(this), mNext(this) {}
	Ref(const Ref<T>& other) : obj(nullptr), mPrev(this), mNext(this) {
		if(&other != this)
			link(other);
	}

	~Ref() { clear(); }

	T& operator*() const { return *obj; }
	T* operator->() const { return obj; }
	bool operator ==(const Ref<T>& other) const { return obj == other.obj; }
    bool operator !=(const Ref<T>& other) const { return obj != other.obj; }
    Ref<T>& operator=(const Ref<T>& other) {
    	if(&other != this) {
    		clear();
    		link(other);
    	}

    	return *this;
    }

    bool isNull() const { return obj == nullptr; }

    void clear() {
    	if(mNext != this) {
    		mPrev->mNext = mNext;
    		mNext->mPrev = mPrev;

    		mPrev = this;
    		mNext = this;
    	} else if(obj != nullptr) {
    		delete obj;
    	}

    	obj = nullptr;
    }
};

#endif // THOUGHT_REF_H