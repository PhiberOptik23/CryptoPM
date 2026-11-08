#include "PasswordEntry.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

PasswordEntry::PasswordEntry() : timestamp(std::time(nullptr)) {}

PasswordEntry::PasswordEntry(string r, string u, string p, std::time_t t)
    : resource(r), user(u), password(p), timestamp(t) {
}

// получение значений
string PasswordEntry::getResourceName() const { return resource; }
string PasswordEntry::getUserName() const { return user; }
string PasswordEntry::getPassword() const { return password; }
time_t PasswordEntry::getTimestamp() const { return timestamp; }

// дата
std::string PasswordEntry::getFormattedDate() const {
    char buffer[26];
    ctime_s(buffer, sizeof(buffer), &timestamp);
    std::string dateStr(buffer);
    // Удаляем символ новой строки в конце, который добавляет ctime
    if (!dateStr.empty() && dateStr.back() == '\n') {
        dateStr.pop_back();
    }
    return dateStr;
}

// выставление значений
void PasswordEntry::setResourceName(const string& r) { resource = r; }
void PasswordEntry::setUserName(const string& u) { user = u; }
void PasswordEntry::setPassword(const string& p) { password = p; }
void PasswordEntry::setTimestamp(time_t t) { timestamp = t; }



// вывод информации
void PasswordEntry::printInfo() const {
    char buffer[26];
    ctime_s(buffer, sizeof(buffer), &timestamp);
    string dateStr(buffer);
    if (!dateStr.empty() && dateStr.back() == '\n') dateStr.pop_back();

    cout << "Ресурс:       " << resource << endl;
    cout << "Логин:        " << user << endl;
    cout << "Пароль:       " << password << endl;
    cout << "Дата создания: " << dateStr << endl;
    cout << "-----------------------------------" << endl;
}

bool PasswordEntry::operator==(const PasswordEntry& other) const {
    return resource == other.resource &&
        user == other.user &&
        password == other.password &&
        timestamp == other.timestamp;
}