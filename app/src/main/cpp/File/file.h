//
// Created by Song on 2025/11/29.
//

#ifndef DOBBY_PROJECT_FILE_H
#define DOBBY_PROJECT_FILE_H

#include <fstream>
#include <string>
#include <memory>

class cMyfile{
public:
// 文件打开模式枚举
    enum class FileMode {
        noMode,
        Read,
        Write,
        Append,
        ReadWrite,
        Binary
    };

// 文件写入模式
    enum class WriteMode {
        noMode,
        Overwrite,  // 覆盖模式
        Append,     // 追加模式
        Truncate    // 截断模式
    };
private:
    // 防止拷贝
    cMyfile(const cMyfile&) = delete;
    cMyfile& operator=(const cMyfile&) = delete;
public:
    cMyfile(const std::string filePath=nullptr);
    ~cMyfile();

public:
    bool openFile(cMyfile::FileMode mode);
    bool closeFile();
    bool isOpen()const;
    size_t getFileSize();
    bool flush();
    bool write(const std::string& data);
    bool writeLine(const std::string& line);
private:
    std::shared_ptr<std::ofstream> m_stream ;
    std::string m_filePath;
};


#endif //DOBBY_PROJECT_FILE_H
