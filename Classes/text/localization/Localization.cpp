#include "Localization.h"
#include <iostream>
#include <cocos2d.h>
#include <external/json/document.h>
#include <external/json/reader.h>
#include <assert.h>
#include <unordered_map>

Localization* Localization::instance = nullptr;

Localization::Localization() 
{
	if (supportedLanguages->size() > 0) {
		std::string jsonContent;
		std::string fileAddress;
		bool hasParseError;
		size_t findPositionResult;
		rapidjson::Document document;
		std::unordered_map<std::string, std::string> map;
		for (auto language : supportedLanguages) {
			fileAddress = "res/localization/" + language + ".json";
			assert(cocos2d::FileUtils::getInstance()->isFileExist(fileAddress)==true);
			jsonContent = cocos2d::FileUtils::getInstance()->getStringFromFile(fileAddress);
			findPositionResult = jsonContent.find_last_of("}");
			jsonContent = jsonContent.substr(0, findPositionResult + 1);
			findPositionResult = jsonContent.find_first_of("{");
			jsonContent = jsonContent.substr(findPositionResult, jsonContent.length());
			hasParseError = document.Parse<0>(jsonContent.c_str()).HasParseError();
			assert(hasParseError==false);
			map = std::unordered_map<std::string, std::string>();
			if (document.MemberBegin() != document.MemberEnd()) {
				for (auto member = document.MemberBegin(); member != document.MemberEnd(); member++) {
					map[member->name.GetString()] = member->value.GetString();
				}
			}
			localizationMap[language] = map;
		}
	}
}

Localization* Localization::getInstance() {
	if (!instance) {
		instance = new Localization();
	}
	return instance;
}

std::string Localization::translate(const std::string key) {
	return localizationMap[defaultLanguage][key];
}

void Localization::setLanguage(const std::string langCode) {
	defaultLanguage = langCode;
}