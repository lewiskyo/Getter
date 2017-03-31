#include <iostream>
#include <string>
#include "../getter/message.h"

using namespace std;

class Example
{

public:
	Example(string);

private:
	string args;
	
};

Example::Example(string args)
{
	this->args = args;
}


extern "C"
{
	void *create(string args)
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

		cout << "Example Dispatch" << endl;
	}

	void destroy()
	{

	}
}