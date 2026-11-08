#include "PasswordManager.h"
#include <algorithm>

using namespace std;

void PasswordManager::addEntry(const PasswordEntry& entry) {
    entries.push_back(entry);
}

// Теперь удаляем КОНКРЕТНЫЙ объект (на случай дубликатов)
bool PasswordManager::removeEntry(const PasswordEntry& entryToRemove) {
    auto it = find(entries.begin(), entries.end(), entryToRemove);
    if (it != entries.end()) {
        entries.erase(it);
        return true;
    }
    return false;
}

// Заменяем старую запись на новую
bool PasswordManager::replaceEntry(const PasswordEntry& oldEntry, const PasswordEntry& newEntry) {
    // Ищем точное совпадение старой записи
    auto it = find(entries.begin(), entries.end(), oldEntry);
    if (it != entries.end()) {
        *it = newEntry; // Присваиваем новую
        return true;
    }
    return false;
}

// Поиск по конкретному полю (Ресурс или Юзер)
vector<PasswordEntry> PasswordManager::findByCriteria(const string& query, SearchType type) const {
    vector<PasswordEntry> results;
    copy_if(entries.begin(), entries.end(), back_inserter(results),
        [&](const PasswordEntry& e) {
            if (type == SearchType::RESOURCE)
                return e.getResourceName().find(query) != string::npos;
            else
                return e.getUserName().find(query) != string::npos;
        });
    return results;
}

// Поиск для редактирования (Ресурс + Юзер)
vector<PasswordEntry> PasswordManager::findSpecific(const string& res, const string& user) const {
    vector<PasswordEntry> results;
    copy_if(entries.begin(), entries.end(), back_inserter(results),
        [&](const PasswordEntry& e) {
            return e.getResourceName() == res && e.getUserName() == user;
        });
    return results;
}

// ... Сортировки и прочее остаются такими же ...
void PasswordManager::sortByResource() {
    sort(entries.begin(), entries.end(), [](const PasswordEntry& a, const PasswordEntry& b) {
        return a.getResourceName() < b.getResourceName();
        });
}
void PasswordManager::sortByDate() {
    sort(entries.begin(), entries.end(), [](const PasswordEntry& a, const PasswordEntry& b) {
        return a.getTimestamp() > b.getTimestamp();
        });
}
const vector<PasswordEntry>& PasswordManager::getAllEntries() const { return entries; }
void PasswordManager::setAllEntries(const vector<PasswordEntry>& newEntries) { entries = newEntries; }
void PasswordManager::clear() { entries.clear(); }