#include <iostream>
#include <string>
#include "../Tools/Message.h"

using namespace std;

class Example
{

public:
	Example(string);

private:
	string args;
	
};

Example::Example(std::string args)
{
	this->args = args;
}


extern "C"
{
	void *create(std::string args)
	{
		Example* example = new Example(args);
		return example;
	}

	bool init()
	{
		return true;
	}

	void dispatch(void* actor, Message* msg)
	{
		Example* example = (Example*)actor;

		std::cout << "Example Dispatch" << std::endl;
	}

	void destroy()
	{

	}
}