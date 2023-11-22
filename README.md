# Iniparser

> A simple ini file parser.

## Overview

| section            | name                     | value                       |
| ------------------ | ------------------------ | --------------------------- |
| Is enclosed in [ ] | Name that comes before = | Anything that comes after = |
|                    |                          |                             |

### Parser Summary

The constructor requires **_filename_** as an argument.

```c++

    IniParser(std::string filename);

```

Set the section name of stored values.

```c++

    void setSection(std::string section);

```

Incase of errors during parsing the ini file an exception is thrown.

```c++

    catch(IniError e){...}

```

Adding name/values pairs to ini.

```c++
    template <typename T>
    void addValue(std::string name, T value);

```

Getting values from ini file.

```c++

    int 		getInt(std::string name);
    std::string getString(std::string name);
    float 		getFloat(std::string name);
    double 		getDouble(std::string name);
    bool 		getBool(std::string name);

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
