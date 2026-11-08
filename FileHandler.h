#pragma once
#include <vector>
#include <string>
#include "PasswordEntry.h"

// Интерфейс (Абстрактный класс)
class IFileService {
public:
    virtual void save(const std::vector<PasswordEntry>& data, const std::string& filename, bool encrypted) = 0;
    virtual std::vector<PasswordEntry> load(const std::string& filename, bool encrypted) = 0;
    virtual ~IFileService() {}
};

class FileHandler : public IFileService {
public:
    void save(const std::vector<PasswordEntry>& data, const std::string& filename, bool encrypted) override;
    std::vector<PasswordEntry> load(const std::string& filename, bool encrypted) override;
};