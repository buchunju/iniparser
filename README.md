# Iniparser

> A simple ini file parser.

## Overview

| section            | Key                      | value                       |
| ------------------ | ------------------------ | --------------------------- |
| Is enclosed in [ ] | key that comes before = | Anything that comes after = |
|                    |                          |                             |

### Parser Summary

The constructor requires **_filename_** as an argument.

```c++

    IniParser(std::string filename);

```

Set the section key of stored values.

```c++

    void setSection(std::string section);

```

Incase of errors during parsing the ini file an exception is thrown.

```c++

    catch(IniError e){...}

```

Adding key/values pairs to ini.

```c++
    template <typename T>
    void addValue(std::string key, T value);

```

Getting values from ini file.

```c++

    int   getInt(std::string key);
    std::string getString(std::string key);
    float   getFloat(std::string key);
    double   getDouble(std::string key);
    bool   getBool(std::string key);

```

### EXAMPLE

An example of adding and accessing values without saving to file.

```c++

    IniParser parser();

    parser.setSection("Colors");
    parser.addValue("Favourite", "Black");

    std::cout << "My favoutite color is: " << parser["Colors"].getString("Favourite") << std::endl;


```

Saving and parsing an ini file. This should be placed in a 'try catch' to catch exceptions that may be thrown.

```c++
    IniParser parser("test.ini");

    //
    // example save to file
    parser.addSection("Books");
    parser.addValue("title", "Harry Potter");
    parser.addValue("year", 2017);
    parser.save();

    //
    // example parse from file
    parser.parse("test.ini");

    cout << "Section: Books" << endl;
    cout << "title:  " << parser["Books"].getString("title") << endl;
    cout << "year: " << parser["Books"].getInt("year") << endl;
```
