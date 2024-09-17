#include <iostream>
#include "hello.h"

int Hello::printHello()
{
	std::cout << this->msg << std::endl;
	return 0;
}

