#pragma once

#include <string>
#include <unordered_map>

namespace Localizaion {
	namespace Languages {
		static constexpr char en[] = "en";
		static constexpr char fa[] = "fa";
		static constexpr char ar[] = "ar";
	}
}

class Localization
{
public:
	Localization();
	static Localization* getInstance();
	std::string translate(const std::string key);
	void setLanguage(const std::string langCode);
private:
	std::string defaultLanguage = Localizaion::Languages::en;
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> localizationMap = {};
	static Localization* instance;
	std::string supportedLanguages[3] = {
		Localizaion::Languages::fa,
		Localizaion::Languages::en,
		Localizaion::Languages::ar
	};
};