/*
todo:
1. main user input [90%]
1.1. proper menu handling [60%]
1.2. more approachable look[80%]
1.3. exceptions handling[40%]

2. file access [100%]
2.1. file security [0%]


3. encryption/decryption algorithm [20%]

4. denuvo [0%]

5. convert to classes [1%]
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <shellapi.h>
using namespace std;

const struct PasswordEntry
{
private:
    string resource;
    string user;
    string password;
public:
    PasswordEntry() { resource = ""; user = ""; password = ""; }
    PasswordEntry(string r, string u, string p) {
        resource = r;
        user = u;
        password = p;
    }

    string getResourceName() {
        return resource;
    }

    string getUserName() {
        return user;
    }

    string getPassword() {
        return password;
    }
};

void addPassword(vector<PasswordEntry>& data);
vector<PasswordEntry> removePassword(vector<PasswordEntry>& data);
bool checkIfExistsInDB(vector<PasswordEntry>& data, string field, char c);
bool matchesEntryRtoU(const vector<PasswordEntry>& data, string field1, string field2);
string encryptDecrypt(string toEncrypt);
void saveToCSV(const vector<PasswordEntry>& db, const string& filename);
vector<PasswordEntry> loadFromCSV(string& filename);
string filename_i = "data.txt";
string& db_filename = filename_i;
char exit_flag = 'n';

void showMenu();


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");
    cout << "\033[32m*   *   *   *[ CRYPTO-PM ]*   *   *   *\033[0m" << endl;
    showMenu();
    string choice_i;
    char choice;
    cout << "\033[32mВыберите пункт: \033[0m";
    cin >> choice_i;
    choice = choice_i[0];
    vector<PasswordEntry> data = loadFromCSV(db_filename);
    const vector<PasswordEntry>& db_ref = data;
    int k;

    while (true) {
        switch (choice) {
            case '1':
                cout << "lol!" << endl;
                break;
            case '2':
                cout << "Сколько записей необходимо добавить? ";
                cin >> k;
                for (int i = 0; i < k; i++) {
                    //cout << "Новый ресурс под номером [" << i+1 << "]:" << endl;
                    addPassword(data);
                }
                saveToCSV(data, db_filename);
                break;
            case '3':
                data = removePassword(data);
                saveToCSV(data, db_filename);
                break;
            case '4':
                saveToCSV(data, db_filename);
                break;
            case '5':
                cout << "Сохраненные пароли:" << endl;
                for (PasswordEntry entry : data) {
                    cout << "\033[32mРесурс: " << entry.getResourceName() << "\033[0m\nИмя пользователя: " + entry.getUserName() <<
                        "\nПароль: " + entry.getPassword() << "\n" << endl;
                }
                break;
            case '6':
                ShellExecute(0, 0, L"https://www.google.com/", 0, 0, SW_SHOW);
                break;
            case '0':
                saveToCSV(data, db_filename);
                return 0;
                break;
            default:
                cout << "\033[31mНеверный ввод! Попробуйте снова\033[0m" << endl;
        }
        cout << "\033[32mВыберите пункт: \033[0m";
        cin >> choice_i;
        choice = choice_i[0];
    }

}

void addPassword(vector<PasswordEntry>& data)
{
    string resource_name;
    cout << "Введите имя ресурса:";
    std::getline(std::cin >> std::ws, resource_name);
    string user_name;
    cout << "Введите имя пользователя на ресурса:";
    std::getline(std::cin >> std::ws, user_name);
    string password;
    cout << "Введите пароль:";
    std::getline(std::cin >> std::ws, password);
    cout << "\n";
    PasswordEntry entry = PasswordEntry(resource_name, user_name, password);
    data.push_back(entry);
}

vector<PasswordEntry> removePassword(vector<PasswordEntry>& data) {
    string resource_name;
    cout << "Введите имя ресурса, который хотите удалить: ";
    std::getline(std::cin >> std::ws, resource_name);
    if (!checkIfExistsInDB(data, resource_name, 'r')) {
        cout << "\033[31mЗапись с таким именем ресурса не была найдена в базе данных!\nВозвращаюсь к вызову пункта меню.\033[0m" << endl;
        return data;
    }
    string user_name;
    cout << "Для подтверждения введите имя пользователя на ресурсе: ";
    std::getline(std::cin >> std::ws, user_name);
    if (!matchesEntryRtoU(data,resource_name,user_name)) {
        cout << "Возвращаюсь к вызову пункта меню." << endl;
        return data;
    }
    
    vector<PasswordEntry> newData;
    for (PasswordEntry e : data) {
        if (e.getResourceName() == resource_name && e.getUserName() == user_name) {
            continue;
        }
        else {
            newData.push_back(e);
        }
    }
    cout << "Запись была успешно удалена!\n";
    
    return newData;

}
bool checkIfExistsInDB(vector<PasswordEntry>& data, string field, char c) {
    bool contains = false;
    for (PasswordEntry e : data) {
        switch (c) {
            case 'r':
                if (e.getResourceName() == field) { 
                    contains = true;
                    return contains;
                }
                break;
            case 'u':
                if (e.getUserName() == field) {
                    contains = true;
                    return contains;
                }
                break;
            case 'p':
                if (e.getPassword() == field) {
                    contains = true;
                    return contains;
                }
                break;
        }
    }
    return false;
}

bool matchesEntryRtoU(const vector<PasswordEntry>& data, string field1, string field2) 
{
    for (PasswordEntry e : data) {
        if (e.getResourceName() == field1) {
            if (e.getUserName() == field2) {
                return true;
            }
            else {
                cout << "\033[31mВведенные поля не совпадают!\033[0m" << endl;
                return false;
            }
        }
    }
    cout << "\033[31mНе было найдено данной записи в базе данных!\033[0m" << endl;
    return false;
}

string encryptDecrypt(string toEncrypt) {
    char key = 'K';
    string output = toEncrypt;

    for (int i = 0; i < toEncrypt.size(); i++) {
        output[i] = toEncrypt[i] ^ key;
    }

    return output;
}

void saveToCSV(const vector<PasswordEntry>& db, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Не удалось сохранить файл " << filename << "\n";
        return;
    }

    for (PasswordEntry entry : db) {
        file << encryptDecrypt(entry.getResourceName()) << ","
            << encryptDecrypt(entry.getUserName()) << ","
            << encryptDecrypt(entry.getPassword()) << "\n";
    }
    // файл закроется автоматически
}


vector<PasswordEntry> loadFromCSV(string& filename) {
    vector<PasswordEntry> db;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Файл ещё не был создан, возвращаю чистую базу данных." << endl;
        return db; // пустой вектор
    }

    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        string resource, user, password;

        //читаем поля, разделённые запятыми
        if (getline(ss, resource, ',') &&
            getline(ss, user, ',') &&
            getline(ss, password, ',')) {
            db.emplace_back(encryptDecrypt(resource), encryptDecrypt(user), encryptDecrypt(password));
        }
    }
    return db;
}

void showMenu() {
    cout << "1 - загрузить базу данных вручную\n2 - добавить новые записи в базу данных" <<
        "\n3 - удалить запись из базы данных\n4 - сохранить новые пароли вручную"<<
        "\n5 - показать базу данных паролей\n0 - выход" << endl;
}
