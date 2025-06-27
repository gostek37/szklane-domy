#pragma once
#include "command.h"
#include <algorithm>

/**
* \brief A class storing previous commands for undoing.
*
* \details The class manages the commands in memory, runs them and allows for undoing. It has an array of fixed size that works like a stack, with the diffrence that
* after "overflowing" it starts overwriting the oldest commands.
*/
class CommandHistory {
public:
	CommandHistory() : current_(0), total_(0), commands_() {};

	void runCommand(ICommand* command);
	void undo();
private:
	static const unsigned int MAX_HISTORY_SIZE;
	unsigned int current_;
	unsigned int total_;
	ICommand* commands_[128];
};
