/*****************************************************
** File    : ini.cpp
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

#include "ini.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <stdexcept>

IniData::IniData(const std::unordered_map<std::string, std::string> &sectionData, const std::string &sectionName)
    : data(sectionData), sectionName(sectionName) {}

bool IniData::getBoolean(const std::string &name) const
{
    checkName(name);
    char c = std::tolower(data.at(name)[0]);
    return (c == 'y' || c == '1' || c == 't');
}

std::string IniData::getString(const std::string &name) const
{
    checkName(name);
    return data.at(name);
}

int IniData::getInt(const std::string &name) const
{
    checkName(name);
    try
    {
        return std::stoi(data.at(name));
    }
    catch (const std::invalid_argument &e)
    {
        throw IniError(IniError::TYPE_MISMATCH, "The type for " + name + " is not an int.");
    }
    catch (const std::out_of_range &e)
    {
        throw IniError(IniError::TYPE_MISMATCH, "The value for " + name + " is out of range for an int.");
    }
}

float IniData::getFloat(const std::string &name) const
{
    checkName(name);
    try
    {
        return std::stof(data.at(name));
    }
    catch (const std::invalid_argument &e)
    {
        throw IniError(IniError::TYPE_MISMATCH, "The type for " + name + " is not a float.");
    }
    catch (const std::out_of_range &e)
    {
        throw IniError(IniError::TYPE_MISMATCH, "The value for " + name + " is out of range for a float.");
    }
}

double IniData::getDouble(const std::string &name) const
{
    checkName(name);
    try
    {
        return std::stod(data.at(name));
    }
    catch (const std::invalid_argument &e)
    {
        throw IniError(IniError::TYPE_MISMATCH, "The type for " + name + " is not a double.");
    }
    catch (const std::out_of_range &e)
    {
        throw IniError(IniError::TYPE_MISMATCH, "The value for " + name + " is out of range for a double.");
    }
}

void IniData::checkName(const std::string &name) const
{
    if (data.find(name) == data.end())
    {
        throw IniError(IniError::NAME_MISSING, name + " not in " + sectionName);
    }

    if (data.at(name).empty())
    {
        throw IniError(IniError::VALUE_MISSING, "Value for " + name + " is missing");
    }
}

IniError::IniError(TYPE type, const std::string &msg) : message(msg)
{
    std::string errorType;
    switch (type)
    {
    case SECTION_MISSING:
        errorType = "SECTION MISSING";
        break;
    case NAME_MISSING:
        errorType = "NAME MISSING";
        break;
    case VALUE_MISSING:
        errorType = "VALUE MISSING";
        break;
    case TYPE_MISMATCH:
        errorType = "TYPE MISMATCH";
        break;
    default:
        errorType = "UNKNOWN";
        break;
    }

    message = "\nError: [" + errorType + "] " + message;
}

std::string IniError::toString() const
{
    return message;
}

IniParser::IniParser() {}

IniParser::IniParser(const std::string &filename) : filename(filename) {}

IniParser::~IniParser() {}

void IniParser::addSection(const std::string &sectionName)
{
    if (parseTree.find(sectionName) != parseTree.end())
    {
        throw IniError(IniError::SECTION_MISSING, "Section '" + sectionName + "' already exists.");
    }
    currentSection = sectionName;
    parseTree[sectionName] = {};
}

void IniParser::removeSection(const std::string &sectionName)
{
    parseTree.erase(sectionName);
}

// Helper function to check if the current section is set
void IniParser::ensureCurrentSection() const
{
    if (currentSection.empty())
    {
        throw IniError(IniError::SECTION_MISSING, "Section name not set. Cannot add value.");
    }
}

void IniParser::ensureCurrentSection(const std::string &name) const
{
    if (currentSection.empty())
    {
        throw IniError(IniError::SECTION_MISSING, "Section name not set. Cannot add value: " + name);
    }
}

template <typename T>
void IniParser::addValue(const std::string &name, const T &value)
{
    ensureCurrentSection(name);
    if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, const char *> || std::is_same_v<T, char *>)
    {
        parseTree[currentSection][name] = std::string(value);
    }
    else
    {
        parseTree[currentSection][name] = std::to_string(value);
    }
}

IniData IniParser::operator[](const std::string &sectionName) const
{
    auto it = parseTree.find(sectionName);
    if (it == parseTree.end())
    {
        throw IniError(IniError::SECTION_MISSING, "Section '" + sectionName + "' not found.");
    }

    return IniData(it->second, sectionName);
}

std::string trim(const std::string &str)
{
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

void IniParser::parse()
{
    if (parseTree.size() > 0)
    {
        currentSection.clear();
        parseTree.clear();
    }

    std::ifstream reader(filename);
    if (!reader.is_open())
    {
        throw IniError(IniError::SECTION_MISSING, "Unable to open file: " + filename);
    }

    std::string line;
    while (std::getline(reader, line))
    {
        line = trim(line);

        if (line.empty())
            continue;

        if (line[0] == '[')
        {
            currentSection = trim(line.substr(1, line.length() - 2));
            addSection(currentSection);
        }
        else if (line.find('=') != std::string::npos)
        {
            auto delimiterPos = line.find('=');
            auto key = trim(line.substr(0, delimiterPos));
            auto value = trim(line.substr(delimiterPos + 1));
            addValue(key, value);
        }
        else
        {
            std::cerr << "Invalid line: " << line << std::endl;
        }
    }

    reader.close();
}

void IniParser::parse(const std::string &filename)
{
    this->filename = filename;
    parse();
}

void IniParser::save() const
{
    std::ofstream writer(filename);
    if (!writer.is_open())
    {
        throw IniError(IniError::SECTION_MISSING, "Unable to open file for writing: " + filename);
    }

    for (const auto &section : parseTree)
    {
        writer << "[" << section.first << "]" << std::endl;
        for (const auto &data : section.second)
        {
            writer << data.first << "=" << data.second << std::endl;
        }
    }

    writer.close();
}

template void IniParser::addValue<int>(const std::string &, const int &);
template void IniParser::addValue<float>(const std::string &, const float &);
template void IniParser::addValue<double>(const std::string &, const double &);
template void IniParser::addValue<bool>(const std::string &, const bool &);
template void IniParser::addValue<std::string>(const std::string &, const std::string &);