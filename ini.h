/*****************************************************
** File    : ini.h
** Project : iniparser.
** Date    : 09-10-20
** Author  : Kenneth Buchunju
** Email   : buchunjukenneth@gmail.com
**
** Copyright 2020 Kenneth Buchunju <buchunjukenneth@gmail.com>
**
******************************************************/

#ifndef INIPARSER_H
#define INIPARSER_H

#include <string>
#include <fstream>

#define NO_ERROR ""

class iniparser{
public:
    /**
     * @brief constructor
     * @param filename name and possibly path of the ini file.
     */
    iniparser(std::string filename);

    ~iniparser();

    /**
     * @brief set section name.
     * @param section name as a string.
     */
    void setSection(std::string section);

    /**
     * @brief save an integer value
     * @param Item name as string.
     * @param value value of the item.
     */
    void saveInt(std::string Item, int value);

    /**
     * @brief save a string value
     * @param Item name as string.
     * @param value value of the item.
     */
    void saveString(std::string Item, std::string value);

    /**
     * @brief save a float value
     * @param Item name as string.
     * @param value value of the item.
     */
    void saveFloat(std::string Item, float value);

    /**
     * @brief save a double value
     * @param Item name as string.
     * @param value value of the item.
     */
    void saveDouble(std::string Item, double value);

    /**
     * @brief save a boolean value
     * @param Item name as string.
     * @param value value of the item.
     */
    void saveBool(std::string Item, bool value);

    /**
     * @brief get an integer value from ini file.
     * @param Item name of item.
     * @return the integer value of the item.
     */
    int getInt(std::string Item);

    /**
     * @brief get a string value from ini file.
     * @param Item name of item.
     * @return the string value of the item.
     */
    std::string getString(std::string Item);

    /**
     * @brief get a float value from ini file.
     * @param Item name of item.
     * @return the float value of the item.
     */
    float getFloat(std::string Item);

    /**
     * @brief get a double value from ini file.
     * @param Item name of item.
     * @return the double value of the item.
     */
    double getDouble(std::string Item);

    /**
     * @brief get a boolean value from ini file.
     * @param Item name of item.
     * @return the boolean value of the item.
     */
    bool getBool(std::string Item);

    std::string getLastError();

private:

    std::string filename; // Filename of the ini file.

    std::string current_section; // name of the current section.

    std::string last_error; // Store the error from input/output operations.

    enum value_type{
        V_INT    = 0xa,
        V_STRING = 0xb,
        V_FLOAT  = 0xc,
        V_DOUBLE = 0xd,
        V_BOOL   = 0xe
    };

    void* format_value(value_type typ, std::string value); // format value to its desired type;

    std::string gen_ouput(value_type m_type, std::string Item, void* value); // format ouput.

    int write_to_file(value_type m_type, std::string Item, void* value); // Write to ini file.

    void* get_from_file(value_type m_type, std::string Item); // Read to ini file.

};


#endif //INIPARSER_H
