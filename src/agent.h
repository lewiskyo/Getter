#ifndef AGENT_H
#define AGENT_H

#include <queue>
#include <string>

#include "module.h"
#include "lock.h"

class Agent {
	public:
		/* Point to the service instance */
		void *instance;
		Module* mod;
		std::string name;

		std::queue<std::string> preparedQueue; 
		std::queue<std::string> processingQueue; 

		// prepared_queue
		CasLock preparedLock;

		bool isSchedule; 
		bool isInit;  
		int msgCount; 
		int sessionId;
		std::string param;

	public:
		Agent(std::string name, std::string param);  
		bool init();
};


#endif
