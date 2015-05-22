#ifndef THOUGHT_ARRAY_H
#define THOUGHT_ARRAY_H

#include <cassert>

// TODO: move from asserts to exceptions

namespace Thought {
template<typename T>
class Array;
};

#define ASSERT_RANGE(v, m) assert(v >= 0 && v < m);

template<typename T>
class Thought::Array {
	int mNext;
	int mSize;
	T* mArray;

	const int MIN_CAPACITY = 1;
	const int GROW_FACTOR = 2;

	void ensureSize(int size) {
		if(mSize >= size) return; // We don't need to do anything
		int _size = (mSize < MIN_CAPACITY ? MIN_CAPACITY : mSize);
		while(_size < size) {
			_size *= GROW_FACTOR;
		}
		T* newArray = new T[_size];
		for(int i = 0; i < mSize; i++) {
			newArray[i] = mArray[i];
		}
		if(mArray != nullptr)
			delete[] mArray;
		mArray = newArray;
		mSize = _size;
	}
public:
	Array() : mNext(0), mSize(0), mArray(nullptr) {}
	Array(int siz) : Array() { ensureSize(siz); }
	Array(int siz, const T& fillWidth) : Array() {
		ensureSize(siz);
		for(int i = 0; i < siz; i++) {
			mArray[i] = fillWidth;
		}
		mNext = siz;
	}
	Array(const Array<T>& other) : Array() { append(other); }
	~Array() { clear(); }

	int size() { return mNext; }
	bool isEmpty() { return mNext == 0; }
	int indexOf(const T& other) {
		for(int i = 0; i < mNext; i++) {
			if(mArray[i] == other) return i;
		}
		return -1;
	}
	int capacity() { return mSize; }
	void add(const T& val) {
		ensureSize(mNext + 1);
		mArray[mNext++] = val;
	}
	void append(const Array<T>& arr) {
		ensureSize(mNext + arr.mNext);
		for(int i = 0; i < arr.mNext; i++) {
			mArray[mNext++] = arr[i];
		}
	}
	void clear() {
		if(mArray != nullptr) delete[] mArray;
		mNext = mSize = 0;
	}
	void removeAt(int index) {
		if(index < 0) index = mNext + index;
		ASSERT_RANGE(index, mNext);

		for(int i = index; i < mNext- 1; i++) {
			mArray[i] = mArray[i + 1];
		}

		mArray[mNext- 1] = T();
		mNext--;
	}
	void truncate(int size) {
		assert(size >= 0 && "Cannot truncate to a negative value");
		if(size >= mNext) return;
		for(int i = size; i < mNext; i++) {
			mArray[i] = T();
		}
		mNext = size;
	}

	Array<T>& operator=(const Array<T>& other) {
		if(&other != this) {
			clear();
			append(other);
		}
		return *this;
	}

	T& operator[](int index) {
		if(index < 0) index = mNext + index;
		ASSERT_RANGE(index, mNext);
		return mArray[index];
	}

	const T& operator[](int index) const {
		if(index < 0) index = mNext + index;
		ASSERT_RANGE(index, mNext);
		return mArray[index];
	}

	void reverse() {
		for(int i = 0; i < mNext / 2; i++) {
			T temp = mArray[i];
			mArray[i] = mArray[mNext -i -1];
			mArray[mNext -i -1] = temp;
		}
	}
};

#undef ASSERT_RANGE

#endif // THOUGHT_ARRAY_H