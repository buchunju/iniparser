/*****************************************************
** File    : ini.h
** Project : iniparser.
** Date    : 09-10-20
** Author  : Kenneth Buchunju
** Email   : buchunjukenneth@gmail.com
**
** MODIFIED : [22-11-23] rewrite
** MODIFIED : [10-04-25] minor changes
**
** Copyright 2020 Kenneth Buchunju <buchunjukenneth@gmail.com>
**
******************************************************/

#ifndef INIPARSER_H
#define INIPARSER_H

#include <string>
#include <unordered_map>
#include <stdexcept>

class IniError
{
public:
    enum TYPE
    {
        SECTION_MISSING = 0xa,
        NAME_MISSING,
        VALUE_MISSING,
        TYPE_MISMATCH,
        TYPE_LEN
    };

    IniError(TYPE type, const std::string &msg);

    std::string toString() const;

private:
    std::string message;
};

class IniData
{
public:
    IniData(const std::unordered_map<std::string, std::string> &sectionData, const std::string &sectionName);
    bool getBoolean(const std::string &key) const;
    std::string getString(const std::string &key) const;
    int getInt(const std::string &key) const;
    float getFloat(const std::string &key) const;
    double getDouble(const std::string &key) const;

private:
    void checkName(const std::string &key) const;
    std::unordered_map<std::string, std::string> data;
    std::string sectionName;
};

class IniParser
{
public:
    IniParser();
    explicit IniParser(const std::string &filename);
    ~IniParser();

    void parse(const std::string &filename);
    void parse();

    void addSection(const std::string &sectionName);
    void removeSection(const std::string &sectionName);
    size_t getSectionCount() const;

    template <typename T>
    void addValue(const std::string &key, const T &value);
    void addValue(const std::string &key, const char *value);

    IniData operator[](const std::string &sectionName) const;

    void save() const;

private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> parseTree;
    std::string filename;
    std::string currentSection;
    void ensureCurrentSection() const;
    void ensureCurrentSection(const std::string &key) const;
};

#endif // INIPARSER_H
