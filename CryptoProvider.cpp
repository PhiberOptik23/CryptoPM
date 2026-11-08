#include "CryptoProvider.h"
#include <windows.h>
#include <intrin.h>
#include <sstream>
#include <vector>
#include <iomanip> // Нужно для hex conversion

std::string CryptoProvider::cachedKey = "";

std::string CryptoProvider::getHardwareFingerprint() {
    return "HELLOWORLD";
}

// Приватный метод: Чистый XOR (работает с сырыми байтами)
std::string CryptoProvider::xorCipher(const std::string& data) {
    if (data.empty()) return "";
    std::string key = getHardwareFingerprint();
    std::string output = data;

    for (size_t i = 0; i < data.size(); i++) {
        output[i] = data[i] ^ key[i % key.size()];
    }
    return output;
}

// --- ПУБЛИЧНЫЕ МЕТОДЫ ---

// Шифруем + Превращаем в HEX строку (чтобы не было запятых в файле)
std::string CryptoProvider::encrypt(const std::string& data) {
    // 1. Сначала шифруем XOR
    std::string encryptedRaw = xorCipher(data);

    // 2. Переводим в HEX (например, символ '?' -> "3F")
    std::stringstream ss;
    ss << std::hex << std::setfill('0'); // Настраиваем поток на 16-ричный вывод

    for (unsigned char c : encryptedRaw) {
        ss << std::setw(2) << (int)c; // Записываем по 2 символа на байт
    }

    return ss.str(); // Возвращаем безопасную строку (например, "4A12B5...")
}

// Читаем HEX строку + Расшифровываем
std::string CryptoProvider::decrypt(const std::string& data) {
    if (data.empty()) return "";

    // 1. Переводим из HEX обратно в сырую строку байтов
    std::string rawData;
    rawData.reserve(data.length() / 2); // Оптимизация памяти

    for (size_t i = 0; i < data.length(); i += 2) {
        // Берем по 2 символа
        std::string byteString = data.substr(i, 2);
        // Конвертируем их в символ (char)
        char byte = (char)strtol(byteString.c_str(), nullptr, 16);
        rawData += byte;
    }

    // 2. Расшифровываем сырые байты через XOR
    return xorCipher(rawData);
}