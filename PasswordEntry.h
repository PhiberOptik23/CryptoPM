#pragma once
#include <string>
#include <ctime>

class PasswordEntry {
private:
    std::string resource;
    std::string user;
    std::string password;
    std::time_t timestamp;

public:
    PasswordEntry();
    PasswordEntry(std::string r, std::string u, std::string p, std::time_t t = std::time(nullptr));

    // получение значений
    std::string getResourceName() const;
    std::string getUserName() const;
    std::string getPassword() const;
    std::time_t getTimestamp() const;
    // метод для получения отформатированной даты (строкой)
    std::string getFormattedDate() const;

    // выставление значений
    void setResourceName(const std::string& r);
    void setUserName(const std::string& u);
    void setPassword(const std::string& p);
    void setTimestamp(std::time_t t);

    // метод вывода информации о себе
    void printInfo() const;

    // оператор сравнения (нужен для точного поиска при редактировании)
    bool operator==(const PasswordEntry& other) const;
};