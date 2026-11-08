#pragma once
#include <vector>
#include <algorithm>
#include <iterator>
#include "PasswordEntry.h"

// Перечисление для типа поиска
enum class SearchType { RESOURCE, USER };

class PasswordManager {
private:
    std::vector<PasswordEntry> entries;

public:
    void addEntry(const PasswordEntry& entry);
    bool removeEntry(const PasswordEntry& entryToRemove); // Удаляем конкретную запись

    // Общий метод замены записи (принимает старую и новую)
    bool replaceEntry(const PasswordEntry& oldEntry, const PasswordEntry& newEntry);

    // Новый поиск по критерию
    std::vector<PasswordEntry> findByCriteria(const std::string& query, SearchType type) const;

    // Поиск точных совпадений (Ресурс + Юзер) для редактирования
    std::vector<PasswordEntry> findSpecific(const std::string& res, const std::string& user) const;

    void sortByResource();
    void sortByDate();

    const std::vector<PasswordEntry>& getAllEntries() const;
    void setAllEntries(const std::vector<PasswordEntry>& newEntries);
    void clear();
};