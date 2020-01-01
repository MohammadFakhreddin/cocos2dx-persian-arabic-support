#include "PersianLanguageSupport.h"
#include <cocos2d.h>
#include <cassert>
#include <external/json/document.h>
#include <external/json/reader.h>
#include "utils/common_operators/CommonOperators.h"
#include <string>
#include <unordered_map>
#include <stack>
#include "text/localization/Localization.h"

PersianLanguageSupport* PersianLanguageSupport::instance;

PersianLanguageSupport* PersianLanguageSupport::getInstance() {
	if (!instance) {
		instance = new PersianLanguageSupport();
	}
	return instance;
}

PersianLanguageSupport::PersianLanguageSupport() {
	rapidjson::Document document;
	{//Extracting json content
		assert(cocos2d::FileUtils::getInstance()->isFileExist(PersianLanguageSupportConstants::resourceAddress));
		auto jsonContent = cocos2d::FileUtils::getInstance()->getStringFromFile(PersianLanguageSupportConstants::resourceAddress);
		auto findPositionResult = jsonContent.find_last_of('}');
		jsonContent = jsonContent.substr(0, findPositionResult + 1);
		findPositionResult = jsonContent.find_first_of('{');
		jsonContent = jsonContent.substr(findPositionResult, jsonContent.length());
		{//Filling document
			auto hasParseError = document.Parse<0>(jsonContent.c_str()).HasParseError();
			assert(!hasParseError);
		}
	}
	{//Filling documents info
		assert(document.HasMember(PersianLanguageSupportConstants::JsonKeys::unicode)==true);
		assert(document.HasMember(PersianLanguageSupportConstants::JsonKeys::connectionBreakers)==true);
		assert(document.HasMember(PersianLanguageSupportConstants::JsonKeys::persianNumbers)==true);
		assert(document.HasMember(PersianLanguageSupportConstants::JsonKeys::arabicNumbers)==true);
		std::vector<std::string> rawValueArray;
		{//unicode
			auto unicodeCharacter = document[PersianLanguageSupportConstants::JsonKeys::unicode].GetArray();
			assert(unicodeCharacter.Size() > 0);
			for (auto& rawUnicodeString : unicodeCharacter) {
				rawValueArray = CommonOperators::split(rawUnicodeString.GetString(), ' ');
				assert(rawValueArray.size() == 4);
				singleChars[rawValueArray[0]] = rawValueArray[0];
				startChars[rawValueArray[0]] = rawValueArray[1];
				middleChars[rawValueArray[0]] = rawValueArray[2];
				endChars[rawValueArray[0]] = rawValueArray[3];
			}
		}
		{//connection breaker
			const auto& rawConnectionBreakers = document[PersianLanguageSupportConstants::JsonKeys::connectionBreakers].GetArray();
			assert(rawConnectionBreakers.Size() > 0);
			std::string character;
			for (auto& rawConnectionBreaker : rawConnectionBreakers) {
				character = CommonOperators::trim(rawConnectionBreaker.GetString());
				connectionBreakerChars[character] = true;
			}
		}
		{//persian number
			const auto& rawPersianNumbers = document[PersianLanguageSupportConstants::JsonKeys::persianNumbers].GetArray();
			assert(rawPersianNumbers.Size() == 10);
			std::string character;
			for (auto& rawPersianNumber : rawPersianNumbers) {
				character = CommonOperators::trim(rawPersianNumber.GetString());
				persianNumbers.emplace_back(character.at(0));
			}
		}
		{//arabic number
			const auto& rawArabicNumbers = document[PersianLanguageSupportConstants::JsonKeys::arabicNumbers].GetArray();
			assert(rawArabicNumbers.Size() == 10);
			std::string character;
			for (auto& rawArabicNumber : rawArabicNumbers) {
				character = CommonOperators::trim(rawArabicNumber.GetString());
				arabicNumbers.emplace_back(character.at(0));
			}
		}
	}
}

std::string PersianLanguageSupport::normalizeText(const std::string& rawText) {
	std::string parsedText = CommonOperators::trim(rawText);
	if (parsedText.length() <= 0) {
		return parsedText;
	}
	//bool isRtl = isPersian(parsedText);
	auto words = CommonOperators::split(parsedText, ' ');
	if (words.size() == 0) {
		return parsedText;
	}
	parsedText = "";
	std::stack<std::string> persianWordBuffer;
	for (const auto& word : words) {
		if (isPersian(word)) {
			persianWordBuffer.push(word);
		}
		else {
			while (persianWordBuffer.size()>0)
			{
				parsedText += normalizeWord(persianWordBuffer.top()) + ' ';
				persianWordBuffer.pop();
			}
			parsedText += word + ' ';
		}
	}
	while (persianWordBuffer.size() > 0)
	{
		parsedText += normalizeWord(persianWordBuffer.top()) + ' ';
		persianWordBuffer.pop();
	}
	return parsedText;
}

std::string PersianLanguageSupport::translateAndNormalize(const std::string& textKey)
{
	return normalizeText(Localization::getInstance()->translate(textKey));
}

bool PersianLanguageSupport::isPersian(const std::string& text) {
	cocos2d::StringUtils::StringUTF8 utf8Text = text;
	auto firstChar = (utf8Text.getString().at(0)._char);
	auto foundChar = singleChars[firstChar];
	return foundChar!= "";
}

std::string PersianLanguageSupport::normalizeWord(const std::string& rawWord) {
	assert(rawWord.length() > 0);
	if (isPersian(rawWord)==false) {
		return rawWord;
	}
	cocos2d::StringUtils::StringUTF8 utf8RawWord = rawWord ;
	auto wordVector = utf8RawWord.getString();
	std::string parsedWord = "";
	std::string foundCharacter;
	bool isPreviousCharacterABreakerChar;
	size_t i = 0;
	for (auto vectorPosition = 0; vectorPosition < wordVector.size(); vectorPosition++) {
		i = wordVector.size() - vectorPosition - 1;
		if (i == 0) {
			foundCharacter = startChars[wordVector[i]._char];
			if (foundCharacter != "") {
				parsedWord += foundCharacter;
			}
			else
			{
				parsedWord += wordVector[i]._char;
			}
		}
		else
		{
			isPreviousCharacterABreakerChar = connectionBreakerChars[wordVector[i - 1]._char] == true;
			if (isPreviousCharacterABreakerChar) {
				if (i == wordVector.size() - 1) {
					foundCharacter = singleChars[wordVector[i]._char];
				}
				else {
					foundCharacter = startChars[wordVector[i]._char];
				}
			}
			else
			{
				if (i == wordVector.size() - 1)
				{
					foundCharacter = endChars[wordVector[i]._char];
				}
				else {
					foundCharacter = middleChars[wordVector[i]._char];
				}
			}
			if (foundCharacter != "") {
				parsedWord += foundCharacter;
			}
			else
			{
				parsedWord += wordVector[i]._char;
			}
		}
	}
	return parsedWord;
}