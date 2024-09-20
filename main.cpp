#include "parser.h"

int main()
{
	Parser parser;
	std::string iString = "\\frac{1}{11}\\beta + 2\\beta  - \\frac{a}{\\pi}ab / cd\\alpha";
	iString.push_back('\0');
	int ret = parser.tokenize(iString);
	printf("ret = %d\n", ret);
	parser.printTokens();
	return 0;
}

