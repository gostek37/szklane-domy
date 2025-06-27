#include "command-history.h"

const unsigned int CommandHistory::MAX_HISTORY_SIZE = 128;

void CommandHistory::runCommand(ICommand* command) {
	// runs the command, and aborts it if it failed
	if (!(command->execute())) {
		delete command;
		return;
	}

	// deletes commands if over the limit
	if (total_ >= MAX_HISTORY_SIZE && commands_[current_]) {
		delete commands_[current_];
	}

	// assigns to the current, then increments
	commands_[current_++] = command;
	
	current_ = current_ % MAX_HISTORY_SIZE;
	total_ = std::min(++total_, MAX_HISTORY_SIZE);
}

void CommandHistory::undo() {
	if (total_ <= 0) {
		return;
	}

	commands_[--current_]->undo();
	delete commands_[current_];

	--total_;
}