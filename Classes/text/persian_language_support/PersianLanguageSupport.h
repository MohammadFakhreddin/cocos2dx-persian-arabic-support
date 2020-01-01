#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <cocos2d.h>

namespace PersianLanguageSupportConstants {
	namespace JsonKeys {
		static constexpr char unicode[] = "unicode";
		static constexpr char connectionBreakers[] = "connectionBreakers";
		static constexpr char persianNumbers[] = "persianNumbers";
		static constexpr char arabicNumbers[] = "arabicNumbers";
	}
	static constexpr char resourceAddress[] = "res/persian_language_support_res/characters.json";
};

class PersianLanguageSupport {
public:
	static PersianLanguageSupport* getInstance();
	std::string normalizeText(const std::string&);
	std::string translateAndNormalize(const std::string&);
private:
	PersianLanguageSupport();

	bool isPersian(const std::string&);
	std::string normalizeWord(const std::string&);

	static PersianLanguageSupport* instance;
	
	std::vector<char> arabicNumbers = {};
	std::vector<char> persianNumbers = {};
	std::unordered_map<std::string, std::string> startChars = {};
	std::unordered_map<std::string, std::string> middleChars = {};
	std::unordered_map<std::string, std::string> endChars = {};
	std::unordered_map<std::string, std::string> singleChars = {};
	std::unordered_map<std::string, bool> connectionBreakerChars = {};
};