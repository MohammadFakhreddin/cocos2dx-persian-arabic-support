#include "CommonOperators.h"

//TODO Add support for UTF8 as well 
std::vector<std::string> CommonOperators::split(std::string text, char splitChar) {
	auto outputArray = std::vector<std::string>();
	if (text.size() > 0) {
		bool isCurrentlyAddingToAWord = false;
		for (const auto& character : text) {
			if (character == '\n') {
				isCurrentlyAddingToAWord = false;
				outputArray.emplace_back(std::string(1,character));
			}
			else {
				if (isCurrentlyAddingToAWord) {
					if (character == splitChar) {
						isCurrentlyAddingToAWord = false;
					}
					else
					{
						outputArray.at(outputArray.size() - 1) += character;
					}
				}
				else
				{
					if (character == splitChar) {
						continue;
					}
					else
					{
						isCurrentlyAddingToAWord = true;
						outputArray.emplace_back(std::string(1, character));
					}
				}
			}
		}
	}
	return outputArray;
}

std::string CommonOperators::trim(std::string rawText) {
	if (rawText.length() == 0) {
		return rawText;
	}
	int start = -1;
	int end = -1;
	for (size_t i = 0; i < rawText.length(); i++) {
		if (rawText[i] != ' ') {
			if (start == -1) {
				start = i;
			}
			else {
				end = i;
			}
		}
	}
	if(end==-1){
		return "";
	}
	std::string parsedText = "";
	for (int i = start; i <= end; i++) {
		parsedText += rawText[i];
	}
	return parsedText;
}