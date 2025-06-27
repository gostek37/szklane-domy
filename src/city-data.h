#pragma once
#include <iostream>
#include "variable-data.h"
#include "vector"
#include "data-parser.h"

/**
* \brief Data of the city
*
* \details The CityData includes such fields as money or people. In the future it will also be able to load custom variables defined in json (such as education).
*/
struct CityData {
	int money;
	unsigned int people;
	std::vector<VariableData> globalVariables;
	std::vector<VariableData> localVariables;

	CityData(int money, unsigned int people) : money(money), people(people) {
		const auto& variableData = DataParser::getVariableData();
		globalVariables = variableData[0];
		localVariables = variableData[1];
	};
};