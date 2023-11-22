#include "ini.h"

#include <fstream>
#include <iostream>
#include <algorithm>

IniData::IniData(map<string, string> sectionData, string sectionName)
    : data(sectionData),
      sectionName(sectionName)
{
}

// get boolean value from the name within the section
bool IniData::getBoolean(string name)
{
    checkName(name);

    char c = tolower(data[name][0]);
    return (c == 'y' || c == '1' || c == 't') ? true : false;
}

string IniData::getString(string name)
{
    checkName(name);

    return data[name];
}

int IniData::getInt(string name)
{
    checkName(name);

    try
    {
        return stoi(data[name]);
    }
    catch (invalid_argument)
    {
        throw IniError(IniError::TYPE_MISMATCH, " the type for " + name + " is not an int.");
    }
}

float IniData::getFloat(string name)
{
    checkName(name);

    try
    {
        return stof(data[name]);
    }
    catch (invalid_argument)
    {
        throw IniError(IniError::TYPE_MISMATCH, " the type for " + name + " is not a float.");
    }
}
double IniData::getDouble(string name)
{
    checkName(name);

    try
    {
        return stod(data[name]);
    }
    catch (invalid_argument)
    {
        throw IniError(IniError::TYPE_MISMATCH, " the type for " + name + " is not a double.");
    }
}

void IniData::checkName(string name)
{
    if (data.find(name) == data.end())
    {
        throw IniError(IniError::NAME_MISSING, (name + " not in " + sectionName));
    }

    if (data[name].length() == 0)
    {
        throw IniError(IniError::VALUE_MISSING, ("value for " + name + " is missing"));
    }
}

//
IniError::IniError(IniError::TYPE type, string msg)
    : message(msg)
{
    string errorType;
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

string IniError::toString()
{
    return message;
}

//

IniParser::IniParser()
{
}

IniParser::~IniParser()
{
}

IniParser::IniParser(std::string filename)
{
    this->filename = filename;
}

void IniParser::addSection(std::string sectionName)
{
    currentSection = sectionName;
    parseTree[sectionName] = {};
}

void IniParser::removeSection(std::string sectionName)
{
    parseTree.erase(sectionName);
}

template <typename T>
void IniParser::addValue(string name, T value)
{
    if (currentSection == "")
    {
        throw IniError(IniError::SECTION_MISSING, "Section name not set");
    }

    string value_str = "";
    if constexpr (is_same<T, string>::value)
    {
        value_str = static_cast<string>(value);
    }
    else if constexpr (is_same<T, const char *>::value)
    {
        value_str = string(value);
    }
    else
    {
        value_str = to_string(value);
    }

    parseTree[currentSection][name] = value_str;
}

IniData IniParser::operator[](std::string sectionName)
{
    if (parseTree.find(sectionName) == parseTree.end())
    {
        throw IniError(IniError::SECTION_MISSING, sectionName + " not found.");
    }

    return IniData(parseTree[sectionName], sectionName);
}

void IniParser::parse()
{
    if (parseTree.size() > 0)
    {
        currentSection = "";
        parseTree.clear();
    }

    // load from file
    ifstream reader(this->filename);
    string line;
    while (getline(reader, line))
    {
        // remove spaces
        line.erase(remove_if(line.begin(), line.end(), iswspace), line.end());

        if (line.length() == 0)
            continue;

        // section
        if (line[0] == '[')
        {
            currentSection = line.substr(1, line.length() - 2);
            addSection(currentSection);
        }
        else if (line.find("=") != string::npos)
        {
            addValue(
                line.substr(0, line.find("=")),
                line.substr(line.find("=") + 1));
        }
        else
        {
            cerr << "invalid line: " << line << endl;
        }
    }

    reader.close();
}

void IniParser::parse(string filename)
{
    this->filename = filename;
    parse();
}

void IniParser::save()
{
    ofstream writer(this->filename);

    for (pair<string, map<string, string>> section : parseTree)
    {
        writer << "[" << section.first << "]" << endl;

        for (pair<string, string> data : section.second)
        {
            writer << data.first << "=" << data.second << endl;
        }
    }

    writer.close();
}

template void IniParser::addValue<int>(string, int);
template void IniParser::addValue<float>(string, float);
template void IniParser::addValue<double>(string, double);
template void IniParser::addValue<bool>(string, bool);
template void IniParser::addValue<string>(string, string);
template void IniParser::addValue<const char *>(string, const char *);
