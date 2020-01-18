# Persian and Arabic support for Cocos2dx

When using createWithTTF, we see that arabic and persian words are reverse and disconnected. 
I'm currently working on solution for this issue. For persian language it is working. 

## How to use

Place res content in your res folder and place Classes content in your Classes then use it in the following way:

```
//I recommend using this 
PersianLanguageSupport::getInstance()->translateAndNormalize("text1");

or 

PersianLanguageSupport::getInstance()->normalizeText("your custom Persian or Arabic text");
```

Because that cocos2d files are handled ascii format you may face issue that you write Arabic or persian but instead you see wrong text. (Unicode characters consist of more that one char.)

So I placed 3 json file in localization folder. You can set language for localization at start before starting your scene

```
Localization::getInstance()->setLanguage("fa");
```

Persian -> fa

Arabic -> ar

English -> en

### Example usage:
```
auto label = Label::createWithTTF(
    PersianLanguageSupport::getInstance()->translateAndNormalize("text1"), 
    "fonts/BYekan.ttf", 
    20
);
```

## Current bugs

Because I don't understand Arabic , if you are willing to fix its Arabic support please contact me to guide you or submit your fix pull request. 
## More update is coming to fix remaining issues
