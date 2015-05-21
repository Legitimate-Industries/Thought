#ifndef THOUGHTC_PRECEDENCE_H
#define THOUGHTC_PRECEDENCE_H

struct Precedence
{
	static const int ASSIGNMENT = 1;
	static const int ADDITION = 3;
	static const int SUBTRACTION = 3;
	static const int MULTIPLICATION = 4;
	static const int DIVISION = 4;
	static const int CONCATENATION = 5;
	static const int POWER = 6;
	static const int PREFIX = 7;
	static const int POSTFIX = 8;
	static const int METHOD = 9;
	static const int CALL = 10;
};

#endif // THOUGHTC_PRECEDENCE_H