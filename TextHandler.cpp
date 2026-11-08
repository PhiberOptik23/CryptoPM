#include "TextHandler.h"
#include <iostream>

using namespace std;

void TextHandler::showMainMenu() {
    cout << "\033[32m* * * *[ CRYPTO-PM ]* * * *\033[0m" << endl;
    cout << "1 - Основные функции\n2 - Технические функции\n3 - Сложные функции\n0 - Выход" << endl;
}

void TextHandler::showFirstMenu() {
    cout << "\033[36m[ Основные функции ]\033[0m" << endl;
    cout << "1 - Добавить запись\n2 - Изменить запись\n3 - Удалить запись\n"
        << "4 - Показать все\n5 - Поиск\n6 - Сортировка\n0 - Назад\n";
}

void TextHandler::showSecondMenu() {
    cout << "\033[36m[ Технические функции ]\033[0m" << endl;
    cout << "1 - Сохранить базу записей (С шифрованием)\n2 - Загрузить базу записей (С шифрованием)\n"
        << "0 - Назад\n";
}

void TextHandler::showThirdMenu() {
    cout << "\033[36m[ Сложные функции ]\033[0m" << endl;
    cout << "1 - Очистить базу\n2 - Тестовые сценарии\n3 - Сохранить базу записей (Без шифрования)"<<
        "\n4 - Загрузить базу записей(Без шифрования)\n0 - Назад\n";
}

void TextHandler::printSuccess(const std::string& msg) {
    cout << "\033[32m[OK] " << msg << "\033[0m" << endl;
}

void TextHandler::printError(const std::string& msg) {
    cout << "\033[31m[ERR] " << msg << "\033[0m" << endl;
}

void TextHandler::printInfo(const std::string& msg) {
    cout << "\033[33m" << msg << "\033[0m";
}

void TextHandler::printTable(const std::vector<PasswordEntry>& data) {
    if (data.empty()) {
        cout << "Список пуст." << endl;
        return;
    }
    cout << "--------------------------------------------------------\n";
    for (const auto& e : data) {
        cout << "Ресурс: " << e.getResourceName()
            << " | Логин: " << e.getUserName()
            << " | Пароль: " << e.getPassword()
            << "\nДата:   " << e.getFormattedDate() << endl;
        cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - \n";
    }
}