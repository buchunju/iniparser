/*****************************************************
** File    : test.cpp
** Project : iniparser.
** Date    : 10-04-25
** Author  : Kenneth Buchunju
** Email   : buchunjukenneth@gmail.com
**
** Copyright 2020 Kenneth Buchunju <buchunjukenneth@gmail.com>
**
** MODIFIED : [22-11-23] rewrite
** MODIFIED : [10-04-25] minor changes
******************************************************/

#include <iostream>
#include <cassert>
#include <fstream>
#include <cmath>
#include "ini.h"

constexpr float FLOAT_EPSILON = 0.0001f;

void cleanupTestFile()
{
    std::remove("test.ini");
}

void testBasicOperations()
{
    std::cout << "=== Running testBasicOperations ===" << std::endl;

    IniParser parser("test.ini");

    // Test adding sections and values
    parser.addSection("User");
    parser.addValue("name", "John Doe");
    parser.addValue("age", 30);
    parser.addValue("balance", 1250.75);
    parser.addValue("active", true);

    // Test saving and reloading
    parser.save();
    parser.parse("test.ini");

    // Verify all values
    assert(parser["User"].getString("name") == "John Doe");
    assert(parser["User"].getInt("age") == 30);
    assert(std::abs(parser["User"].getFloat("balance") - 1250.75f) < FLOAT_EPSILON);
    assert(parser["User"].getBoolean("active") == true);

    std::cout << "PASSED" << std::endl;
}

void testSectionOperations()
{
    std::cout << "=== Running testSectionOperations ===" << std::endl;

    IniParser parser("test.ini");

    // Test multiple sections
    parser.addSection("Database");
    parser.addValue("host", "localhost");
    parser.addValue("port", 3306);

    parser.addSection("Settings");
    parser.addValue("logging", true);
    parser.addValue("timeout", 30.5);

    parser.save();
    parser.parse("test.ini");

    // Verify sections exist
    assert(parser["Database"].getString("host") == "localhost");
    assert(parser["Database"].getInt("port") == 3306);
    assert(parser["Settings"].getBoolean("logging") == true);
    assert(std::abs(parser["Settings"].getFloat("timeout") - 30.5f) < FLOAT_EPSILON);

    // Test section removal
    parser.removeSection("Settings");
    parser.save();
    parser.parse("test.ini");

    try
    {
        parser["Settings"].getBoolean("logging");
        assert(false && "Removed section should not be accessible");
    }
    catch (const IniError &e)
    {
        assert(std::string(e.toString()).find("not found") != std::string::npos);
    }

    std::cout << "PASSED" << std::endl;
}

void testErrorHandling()
{
    std::cout << "=== Running testErrorHandling ===" << std::endl;

    IniParser parser("test.ini");

    // Test missing section
    try
    {
        parser["NonExistent"].getString("key");
        assert(false && "Should throw for missing section");
    }
    catch (const IniError &e)
    {
        assert(std::string(e.toString()).find("not found") != std::string::npos);
    }

    // Test missing key
    parser.addSection("Test");
    parser.addValue("exists", "value");

    try
    {
        parser["Test"].getString("missing");
        assert(false && "Should throw for missing key");
    }
    catch (const IniError &e)
    {
        assert(std::string(e.toString()).find("not in") != std::string::npos);
    }

    // Test empty value
    parser.addValue("empty", "");
    try
    {
        parser["Test"].getString("empty");
        assert(false && "Should throw for empty value");
    }
    catch (const IniError &e)
    {
        assert(std::string(e.toString()).find("missing") != std::string::npos);
    }

    // Test type mismatch
    parser.addValue("not_a_number", "abc");
    try
    {
        parser["Test"].getInt("not_a_number");
        assert(false && "Should throw for type mismatch");
    }
    catch (const IniError &e)
    {
        assert(std::string(e.toString()).find("not an int") != std::string::npos);
    }

    std::cout << "PASSED" << std::endl;
}

void testFileOperations()
{
    std::cout << "=== Running testFileOperations ===" << std::endl;

    // Test empty file
    {
        std::ofstream out("test.ini");
        out.close();

        IniParser parser("test.ini");
        parser.parse();
        assert(parser.getSectionCount() == 0);
    }

    // Test malformed file
    {
        std::ofstream out("test.ini");
        out << "[Section]\n";
        out << "key_without_value\n";  // Invalid line
        out << "=value_without_key\n"; // Invalid line
        out.close();

        IniParser parser("test.ini");
        try
        {
            parser.parse();
            // Depending on your requirements, you might want to:
            // 1. Throw on malformed lines (recommended)
            // 2. Skip malformed lines with warning
            // Currently the test expects it to parse with warnings
            assert(parser.getSectionCount() == 1);
        }
        catch (...)
        {
            // If you choose to throw on malformed lines
            assert(true);
        }
    }

    std::cout << "PASSED" << std::endl;
}

void testEdgeCases()
{
    std::cout << "=== Running testEdgeCases ===" << std::endl;

    IniParser parser("test.ini");

    // Test empty section name
    try
    {
        parser.addSection("");
        assert(false && "Should reject empty section name");
    }
    catch (const IniError &e)
    {
        assert(true);
    }

    // Test empty key name
    parser.addSection("EdgeCases");
    try
    {
        parser.addValue("", "value");
        assert(false && "Should reject empty key name");
    }
    catch (const IniError &e)
    {
        assert(true);
    }

    // Test whitespace trimming
    parser.addValue("  padded_key  ", "  padded_value  ");
    parser.save();
    parser.parse("test.ini");
    assert(parser["EdgeCases"].getString("padded_key") == "padded_value");

    std::cout << "PASSED" << std::endl;
}

int main()
{
    try
    {
        testBasicOperations();
        testSectionOperations();
        testErrorHandling();
        testFileOperations();
        testEdgeCases();

        cleanupTestFile();
        std::cout << "\nALL TESTS PASSED SUCCESSFULLY!" << std::endl;
        return 0;
    }
    catch (const IniError &e)
    {
        std::cerr << "INI ERROR: " << e.toString() << std::endl;
        cleanupTestFile();
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
        cleanupTestFile();
        return 1;
    }
    catch (...)
    {
        std::cerr << "UNKNOWN ERROR" << std::endl;
        cleanupTestFile();
        return 1;
    }
}