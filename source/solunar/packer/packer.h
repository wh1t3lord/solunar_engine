#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

class PackFileWriter
{
public:
    PackFileWriter();

    void initializePaths();

    void createPack(const std::string& name);
    void createPack(const std::string& name, const std::string& fileList);

    void writeCompressedFiles(const std::vector<std::string>& files);
    void writeFiles(const std::vector<std::string>& files);

    std::vector<char> readFileToVector(const std::string& filename);

private:
    std::vector<std::string> m_files;

    std::filesystem::path m_currentPath;
    std::filesystem::path m_currentDataPath;

    FILE* m_file;
};

extern PackFileWriter g_packFileWriter;