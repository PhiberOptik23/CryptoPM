#pragma once
#include <string>
#include <vector>
#include "PasswordEntry.h"

class TextHandler {
public:
    void showMainMenu();
    void showFirstMenu();
    void showSecondMenu();
    void showThirdMenu();

    void printSuccess(const std::string& msg);
    void printError(const std::string& msg);
    void printInfo(const std::string& msg);

    // Вывод списка записей
    void printTable(const std::vector<PasswordEntry>& data);
};