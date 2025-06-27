#pragma once

/**
* \brief Classic command pattern.
*
* \details The commands that implement this interface are made for undoing. Their only purpose is calling a method, and having a way to undo it.
*/
class ICommand {
public:
	virtual bool execute() = 0;
	virtual bool undo() = 0;
};