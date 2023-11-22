/*****************************************************
** File    : ini.h
** Project : iniparser.
** Date    : 09-10-20
** Author  : Kenneth Buchunju
** Email   : buchunjukenneth@gmail.com
**
** MODIFIED : [22-11-23] rewrite
**
** Copyright 2020 Kenneth Buchunju <buchunjukenneth@gmail.com>
**
******************************************************/

#ifndef INIPARSER_H
#define INIPARSER_H

#include <string>
#include <map>

using namespace std;

// Ini Errors
class IniError
{
public:
    enum TYPE
    {
        SECTION_MISSING = 0xa,
        NAME_MISSING,
        VALUE_MISSING,
        TYPE_MISMATCH,
        TYPE_LEN // should not reach here
    };

    IniError(TYPE type, string msg);

    string toString();

private:
    string message;
};

// Ini section Data
class IniData
{
public:
    IniData(map<string, string> sectionData, string sectionName);
    bool getBoolean(string name);
    string getString(string name);
    int getInt(string name);
    float getFloat(string name);
    double getDouble(string name);

private:
    void checkName(string name);
    map<string, string> data;
    string sectionName;
};

// Ini file parser
class IniParser
{
public:
    IniParser();
    IniParser(string filename);
    ~IniParser();

    // parse file
    void parse(string filename);
    void parse();

    // sections
    void addSection(string sectiionName);
    void removeSection(string sectionName);

    // add values
    template <typename T>
    void addValue(string name, T value);

    // overloaded operators
    IniData operator[](string);

    // save to file
    void save();

private:
    map<string, map<string, string>> parseTree;
    string filename;
    string currentSection;
};

#endif // INIPARSER_H
