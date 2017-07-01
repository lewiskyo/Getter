#include <iostream>
#include <string>
#include "../Tools/Message.h"

using namespace std;

class Center
{

public:
	Center(string);

private:
	string args;
	
};

Center::Center(string args)
{
	this->args = args;
}


extern "C"
{
	void *create(string args)
	{
		Center* center = new Center(args);
		return center;
	}

	bool init()
	{
		return true;
	}

	void dispatch(void* actor, Message* msg)
	{
		Center* center = (Center*)actor;

		cout << "Center Dispatch" << endl;
	}

	void destroy()
	{

	}
}