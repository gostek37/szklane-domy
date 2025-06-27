#pragma once
#include <iostream>

struct VariableData {
	std::string name;
	double value;

	VariableData(const std::string& name, double value):
		name(name),
		value(value)
	{};
};