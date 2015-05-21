#ifndef THOUGHTC_PREFIXPARSELET_H
#define THOUGHTC_PREFIXPARSELET_H

#include "../nodes/Node.h"

class Parser;
class PrefixParslet {
public:
	virtual ~PrefixParslet() {}
	virtual Node* parse(Parser*, Token) = 0;
};

#endif // THOUGHTC_PREFIXPARSELET_H