#include "FileHandler.h"
#include "CryptoProvider.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

void FileHandler::save(const vector<PasswordEntry>& data, const string& filename, bool encrypted) {
    ofstream file(filename);
    if (!file.is_open()) throw runtime_error("Не удалось открыть файл!");

    for (const auto& entry : data) {
        string r = entry.getResourceName();
        string u = entry.getUserName();
        string p = entry.getPassword();
        string t = to_string(entry.getTimestamp()); // Сначала превращаем дату в строку

        if (encrypted) {
            // Шифруем ВСЕ поля, включая дату
            file << CryptoProvider::encrypt(r) << ","
                << CryptoProvider::encrypt(u) << ","
                << CryptoProvider::encrypt(p) << ","
                << CryptoProvider::encrypt(t) << "\n";
        }
        else {
            file << r << "," << u << "," << p << "," << t << "\n";
        }
    }
}

vector<PasswordEntry> FileHandler::load(const string& filename, bool encrypted) {
    vector<PasswordEntry> db;
    ifstream file(filename);
    if (!file.is_open()) return db;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string r, u, p, t_str;

        if (getline(ss, r, ',') && getline(ss, u, ',') &&
            getline(ss, p, ',') && getline(ss, t_str))
        {
            if (encrypted) {
                r = CryptoProvider::decrypt(r);
                u = CryptoProvider::decrypt(u);
                p = CryptoProvider::decrypt(p);
                t_str = CryptoProvider::decrypt(t_str); // Расшифровываем дату
            }

            time_t t = 0;
            try {
                t = stoll(t_str); // Парсим строку обратно в число
            }
            catch (...) { t = time(nullptr); }

            db.emplace_back(r, u, p, t);
        }
    }
    return db;
}