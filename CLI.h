#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
	std::vector<Command*> menu;
	// you can add data members
public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
	void insertCommands(commandsSharedInfo* csi);
};

#endif /* CLI_H_ */
