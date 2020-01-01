#pragma once
 
#include <string>
#include <vector>
#include <assert.h>

class CommonOperators {
public:
	std::vector<std::string> static split(std::string text, char splitChar);
	std::string static trim(std::string rawInput);
};