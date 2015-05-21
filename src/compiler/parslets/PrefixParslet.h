#ifndef THOUGHTC_PREFIXPARSELET_H
#define THOUGHTC_PREFIXPARSELET_H

#include "../nodes/Node.h"

namespace Thought {
class Parser;
class PrefixParslet {
public:
	virtual ~PrefixParslet() {}
	virtual Node* parse(Parser*, Token) = 0;
};
};

#endif // THOUGHTC_PREFIXPARSELET_H