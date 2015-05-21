#include "AmaStringTable.h"
#include <algorithm>
using namespace Ama;

StringID StringTable::addString(std::string s) {
	auto iter = std::find(mStrings.begin(), mStrings.end(), s);
	if(iter != mStrings.end()) {
		return StringID(iter - mStrings.begin());
	}
	mStrings.push_back(s);
	return mStrings.size() - 1;
}

StringID StringTable::findID(std::string s) {
	auto iter = std::find(mStrings.begin(), mStrings.end(), s);
	if(iter != mStrings.end()) {
		return StringID(iter - mStrings.begin());
	}
	return addString(s);
}

std::string StringTable::getString(StringID id) {
	return mStrings[id];
}