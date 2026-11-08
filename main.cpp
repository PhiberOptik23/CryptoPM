#include <iostream>
#include <windows.h>
#include <shellapi.h>
#include <limits> 
#include <iterator>
#include <cstdio>

// Подключаем наши заголовочные файлы
#include "PasswordEntry.h"
#include "FileHandler.h"
#include "PasswordManager.h"
#include "TextHandler.h"

using namespace std;

// Глобальная константа имени файла
const string DEFAULT_FILENAME = "data.txt";

void run_tests();
// Вспомогательная функция для ввода строки с пробелами
string getStringInput(const string& prompt) {
    string input;
    cout << prompt;
    // cin >> ws пропускает ведущие пробелы, оставшиеся в буфере
    getline(cin >> ws, input);
    return input;
}

int main() {
    // Настройка консоли для русского языка
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Инициализация
    PasswordManager manager;
    TextHandler ui;
    IFileService* fileService = new FileHandler();

    // Автозагрузка базы при старте
    try {
        vector<PasswordEntry> data = fileService->load(DEFAULT_FILENAME, true);
        manager.setAllEntries(data);
        ui.printSuccess("База данных успешно загружена (" + to_string(data.size()) + " записей).");
    }
    catch (...) {
        ui.printInfo("Файл базы данных не найден или поврежден. Создана новая база.");
    }

    string s_mainChoice;
    char mainChoice;
    while (true) {
        ui.showMainMenu();
        cout << "Ваш выбор: ";
        cin >> s_mainChoice;
        mainChoice = s_mainChoice[0];

        // Обработка выхода из программы
        if (mainChoice == '0') {
            try {
                fileService->save(manager.getAllEntries(), DEFAULT_FILENAME, true);
                ui.printSuccess("База данных сохранена. До свидания!");
            }
            catch (const exception& e) {
                ui.printError(string("Ошибка при сохранении: ") + e.what());
            }
            break;
        }

        switch (mainChoice) {
        case '1': { // === ОСНОВНЫЕ ФУНКЦИИ ===
            system("cls");
            string s_subChoice;
            char subChoice = ' ';
            while (subChoice != '0') {
                ui.showFirstMenu();
                cout << "Ваш выбор: ";
                cin >> s_subChoice;
                subChoice = s_subChoice[0];

                if (subChoice == '0') break; // Выход из подменю

                switch (subChoice) {
                case '1': { // Добавить запись
                    string r = getStringInput("Имя ресурса: ");
                    string u = getStringInput("Имя пользователя: ");
                    string p = getStringInput("Пароль: ");
                    manager.addEntry(PasswordEntry(r, u, p));
                    ui.printSuccess("Запись добавлена.");
                    break;
                }
                case '2': { // Изменить запись
                    string r = getStringInput("Введите имя ресурса для поиска: ");
                    string u = getStringInput("Введите имя пользователя для поиска: ");

                    // 1. Ищем совпадения
                    vector<PasswordEntry> matches = manager.findSpecific(r, u);

                    if (matches.empty()) {
                        ui.printError("Записей не найдено.");
                    }
                    else {
                        PasswordEntry entryToEdit;
                        bool selected = false;

                        // 2. Если запись одна - берем её
                        if (matches.size() == 1) {
                            entryToEdit = matches[0];
                            selected = true;
                        }
                        // 3. Если несколько - даем выбор
                        else {
                            cout << "Найдено несколько записей:\n";
                            for (size_t i = 0; i < matches.size(); ++i) {
                                cout << "[" << i + 1 << "] Пароль: " << matches[i].getPassword()
                                    << " (Дата: " << matches[i].getFormattedDate() << ")" << endl;
                            }

                            int idx;
                            cout << "Введите номер записи для редактирования: ";
                            cin >> idx;

                            if (idx > 0 && idx <= matches.size()) {
                                entryToEdit = matches[idx - 1];
                                selected = true;
                            }
                            else {
                                ui.printError("Неверный номер.");
                            }
                        }

                        // 4. Редактируем выбранную
                        if (selected) {
                            cout << "--- Введите новые данные ---\n";
                            string newR = getStringInput("Новый ресурс: ");
                            string newU = getStringInput("Новый пользователь: ");
                            string newP = getStringInput("Новый пароль: ");

                            // Создаем новую запись, сохраняя старую дату создания
                            PasswordEntry newEntry(newR, newU, newP, entryToEdit.getTimestamp());

                            if (manager.replaceEntry(entryToEdit, newEntry)) {
                                ui.printSuccess("Запись успешно обновлена!");
                            }
                            else {
                                ui.printError("Ошибка при обновлении записи.");
                            }
                        }
                    }
                    break;
                }
                case '3': { // Удалить запись
                    string r = getStringInput("Имя ресурса для удаления: ");
                    string u = getStringInput("Имя пользователя для удаления: ");

                    vector<PasswordEntry> matches = manager.findSpecific(r, u);
                    if (!matches.empty()) {
                        // Удаляем первую найденную (или единственную)
                        manager.removeEntry(matches[0]);
                        ui.printSuccess("Запись удалена.");
                    }
                    else {
                        ui.printError("Запись не найдена.");
                    }
                    break;
                }
                case '4': { // Показать все
                    const auto& all = manager.getAllEntries();
                    if (all.empty()) {
                        ui.printInfo("База данных пуста.");
                    }
                    else {
                        for (const auto& e : all) {
                            e.printInfo();
                        }
                    }
                    break;
                }
                case '5': { // Поиск
                    cout << "Выберите тип поиска:\n1 - По имени ресурса\n2 - По имени пользователя\nВыбор: ";
                    char searchTypeChar;
                    cin >> searchTypeChar;

                    string q = getStringInput("Введите текст для поиска: ");
                    vector<PasswordEntry> results;

                    if (searchTypeChar == '1') {
                        results = manager.findByCriteria(q, SearchType::RESOURCE);
                    }
                    else if (searchTypeChar == '2') {
                        results = manager.findByCriteria(q, SearchType::USER);
                    }
                    else {
                        ui.printError("Неверный выбор типа поиска.");
                        break;
                    }

                    if (results.empty()) {
                        ui.printInfo("Ничего не найдено.");
                    }
                    else {
                        ui.printSuccess("Найдено записей: " + to_string(results.size()));
                        for (const auto& e : results) {
                            e.printInfo();
                        }
                    }
                    break;
                }
                case '6': { // Сортировка
                    cout << "Выберите тип сортировки:\n1 - По алфавиту (Ресурс)\n2 - По дате (Сначала новые)\nВыбор: ";
                    char sortCh;
                    cin >> sortCh;
                    if (sortCh == '1') {
                        manager.sortByResource();
                        ui.printSuccess("База отсортирована по алфавиту.");
                    }
                    else if (sortCh == '2') {
                        manager.sortByDate();
                        ui.printSuccess("База отсортирована по дате.");
                    }
                    else {
                        ui.printError("Неверный выбор.");
                    }
                    break;
                }
                default:
                    ui.printError("Неверный ввод пункта меню.");
                    break;
                }
            }
            system("cls");
            break;
        }

        case '2': { // === ТЕХНИЧЕСКИЕ ФУНКЦИИ ===
            system("cls");
            string s_subChoice;
            char subChoice = ' ';
            while (subChoice != '0') {
                ui.showSecondMenu();
                cout << "Ваш выбор: ";
                cin >> s_subChoice;
                subChoice = s_subChoice[0];

                if (subChoice == '0') break;

                switch (subChoice) {
                case '1': // Сохранить (Encrypted)
                    try {
                        fileService->save(manager.getAllEntries(), DEFAULT_FILENAME, true);
                        ui.printSuccess("База успешно сохранена (зашифрована).");
                    }
                    catch (const exception& e) { ui.printError(e.what()); }
                    break;

                case '2': // Загрузить (Encrypted)
                    try {
                        auto data = fileService->load(DEFAULT_FILENAME, true);
                        manager.setAllEntries(data);
                        ui.printSuccess("База успешно загружена.");
                    }
                    catch (...) { ui.printError("Ошибка загрузки."); }
                    break;

                default:
                    ui.printError("Неверный ввод.");
                    break;
                }
            }

            system("cls");
            break;
        }

        case '3': { // === СЛОЖНЫЕ ФУНКЦИИ ===
            system("cls");
            string s_subChoice;
            char subChoice = ' ';
            while (subChoice != '0') {
                ui.showThirdMenu();
                cout << "Ваш выбор: ";
                cin >> s_subChoice;
                subChoice = s_subChoice[0];

                if (subChoice == '0') break;

                switch (subChoice) {
                case '1': { // Очистка
                    string conf = getStringInput("Вы уверены, что хотите удалить ВСЕ записи? (y/n): ");
                    if (conf == "y" || conf == "Y") {
                        manager.clear();
                        ui.printSuccess("База данных полностью очищена.");
                    }
                    else {
                        ui.printInfo("Операция отменена.");
                    }
                    break;
                }
                case '2':
                    run_tests();
                    system("cls");
                    break;

                case '3': // Сохранить (Plain)
                    try {
                        fileService->save(manager.getAllEntries(), DEFAULT_FILENAME, false);
                        ui.printSuccess("База сохранена в открытом виде.");
                    }
                    catch (const exception& e) { ui.printError(e.what()); }
                    break;

                case '4': // Загрузить (Plain)
                    try {
                        auto data = fileService->load(DEFAULT_FILENAME, false);
                        manager.setAllEntries(data);
                        ui.printSuccess("База в открытом виде загружена.");
                    }
                    catch (...) { ui.printError("Ошибка загрузки."); }
                    break;

                default:
                    ui.printError("Неверный ввод.");
                    break;
                }
            }

            system("cls");
            break;
        }

        default:
            ui.printError("Неверный ввод в главном меню. Попробуйте снова.");
            break;
        }
    }

    delete fileService;
    return 0;
}

#include <cstdio>
#include <vector>

void run_tests() {
    TextHandler ui;
    ui.printInfo("\n=== ЗАПУСК РАСШИРЕННЫХ АВТОМАТИЧЕСКИХ ТЕСТОВ ===\n");

    const string TEST_FILE = "test_data.txt";
    PasswordManager testManager;
    FileHandler testFileHandler;

    // 1. Подготовка данных (9 записей)
    ui.printInfo("Шаг 1: Формирование тестовой базы (9 записей)\n");
    vector<PasswordEntry> initialEntries = {
        {"Youtube", "vloger", "yt123"},
        {"Github", "coder", "git_pass"},
        {"Amazon", "buyer", "shop_77"},
        {"Binance", "trader", "crypto_01"},
        {"Apple", "steve", "iCloud_pass"},
        {"Microsoft", "bill", "win_soft"},
        {"Steam", "gamer", "valve_99"},
        {"ВКонтакте", "Асадуллаев Гюльали", "СамыйЛучший123"},
        {"Одноклассники", "Гюльали Асадуллаев", "********"}
    };

    for (const auto& e : initialEntries) {
        testManager.addEntry(e);
    }
    ui.printSuccess("Записи добавлены в память.");
    ui.printTable(testManager.getAllEntries());

    // 2. Тест поиска
    ui.printInfo("\nШаг 2: Тестирование поиска по запросу 'soft' (Microsoft)\n");
    auto searchRes = testManager.findByCriteria("soft", SearchType::RESOURCE); // Используем универсальный поиск
    if (!searchRes.empty()) {
        ui.printSuccess("Совпадение найдено:");
        for (const auto& e : searchRes) e.printInfo();
    }
    else {
        ui.printError("Запись не найдена!");
    }

    // 3. Тест сортировки
    ui.printInfo("\nШаг 3: Тестирование сортировки по алфавиту (Ресурс)...\n");
    testManager.sortByResource();
    ui.printTable(testManager.getAllEntries());
    if (testManager.getAllEntries()[0].getResourceName() == "Amazon") {
        ui.printSuccess("Сортировка верна (Amazon — первая запись).");
    }

    // 4. Тест изменения
    ui.printInfo("\nШаг 4: Тестирование изменения пароля для 'Steam'...\n");
    auto steamMatch = testManager.findSpecific("Steam", "gamer");
    if (!steamMatch.empty()) {
        PasswordEntry updatedSteam("Steam", "gamer", "NEW_SAFE_PASS", steamMatch[0].getTimestamp());
        testManager.replaceEntry(steamMatch[0], updatedSteam);
        ui.printSuccess("Данные обновлены. Новая запись:");
        auto check = testManager.findSpecific("Steam", "gamer");
        check[0].printInfo();
    }

    // 5. Тест файловой системы и шифрования
    ui.printInfo("\nШаг 5: Проверка шифрования и сохранения в " + TEST_FILE + "...\n");
    try {
        testFileHandler.save(testManager.getAllEntries(), TEST_FILE, true);
        ui.printInfo("Файл создан. Загрузка данных для сверки...\n");

        auto loadedData = testFileHandler.load(TEST_FILE, true);
        ui.printInfo("Содержимое, считанное и расшифрованное из файла:\n");
        ui.printTable(loadedData);

        if (loadedData.size() == 7) {
            ui.printSuccess("Криптографическая проверка пройдена успешно.");
        }
    }
    catch (const exception& e) {
        ui.printError(string("Ошибка ФС: ") + e.what());
    }

    // 6. Тест удаления
    ui.printInfo("\nШаг 6: Удаление записи 'Github'...\n");
    auto gitMatch = testManager.findSpecific("Github", "coder");
    if (!gitMatch.empty()) {
        testManager.removeEntry(gitMatch[0]);
        ui.printInfo("Текущее количество записей: " + to_string(testManager.getAllEntries().size()) + "\n");
        if (testManager.getAllEntries().size() == 6) ui.printSuccess("Удаление выполнено корректно.");
    }

    // показываем все записи перед завершением тестового решения
    ui.printTable(testManager.getAllEntries());

    // Завершение
    ui.printInfo("\nОчистка временных данных... ");
    if (remove(TEST_FILE.c_str()) == 0) ui.printSuccess("ОК");

    ui.printInfo("\n=== ВСЕ ТЕСТЫ ЗАВЕРШЕНЫ УСПЕШНО ===\n");
    system("pause");
}