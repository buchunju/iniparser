/*****************************************************
** File    : test.cpp
** Project : iniparser.
** Date    : 09-10-20
** Author  : Kenneth Buchunju
** Email   : buchunjukenneth@gmail.com
**
** MODIFIED : [22-11-23] rewrite
** MODIFIED : [10-04-25] updated test
**
** Copyright 2020 Kenneth Buchunju <buchunjukenneth@gmail.com>
**
******************************************************/

#include <iostream>
#include <cassert>
#include "ini.h"

void testSaveAndParse()
{
    IniParser parser("test.ini");

    // Save to file
    parser.addSection("Books");
    parser.addValue("title", "Harry Potter");
    parser.addValue("year", 2017);
    parser.addValue("price", 19.99);
    parser.addValue("available", true);
    parser.save();

    // Parse from file
    parser.parse("test.ini");

    // Assertions
    assert(parser["Books"].getString("title") == "Harry Potter");
    assert(parser["Books"].getInt("year") == 2017);
    assert(parser["Books"].getFloat("price") == 19.99f);
    assert(parser["Books"].getBoolean("available") == true);

    std::cout << "testSaveAndParse passed!" << std::endl;
}

void testMissingSection()
{
    IniParser parser("test.ini");

    try
    {
        parser["NonExistent"].getString("key");
        assert(false); // Should not reach here
    }
    catch (const IniError &e)
    {
        assert(e.toString().find("Section 'NonExistent' not found") != std::string::npos);
    }

    std::cout << "testMissingSection passed!" << std::endl;
}

void testMissingKey()
{
    IniParser parser("test.ini");
    parser.addSection("Movies");
    parser.addValue("title", "Inception");
    parser.save();

    parser.parse("test.ini");

    try
    {
        parser["Movies"].getString("director");
        assert(false); // Should not reach here
    }
    catch (const IniError &e)
    {
        assert(e.toString().find("not in Movies") != std::string::npos);
    }

    std::cout << "testMissingKey passed!" << std::endl;
}

int main()
{
    try
    {
        testSaveAndParse();
        testMissingSection();
        testMissingKey();
    }
    catch (const IniError &e)
    {
        std::cerr << "Test failed: " << e.toString() << std::endl;
        return 1;
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}