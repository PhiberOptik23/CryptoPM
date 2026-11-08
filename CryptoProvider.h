#pragma once
#include <string>

class CryptoProvider {
private:
    static std::string cachedKey;
    static std::string getHardwareFingerprint();

    // Единая логика XOR тут (приватная)
    static std::string xorCipher(const std::string& data);

public:
    // Публичные методы-обертки для понятности
    static std::string encrypt(const std::string& data);
    static std::string decrypt(const std::string& data);
};