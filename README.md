Iniparser
==========

> A simple ini file parser.

Overview
---------

     section       |            item           |           value
------------------ | ------------------------- | --------------------------
Is enclosed in []  | Name that comes before =  | Anything that comes after =


### API Summary ###

The constructor requires ___filename___ as an argument.

```c++

iniparser(std::string filename);

```

Set the section name of stored values.

```c++

void setSection(std::string section);

```

Incase of errors in the read or write operations.
```c++

std::string getLastError();

```


Saving data/values to ini files.

```c++

void saveInt(std::string Item, int value);
void saveString(std::string Item, std::string value);
void saveFloat(std::string Item, float value);
void saveDouble(std::string Item, double value);
void saveBool(std::string Item, bool value);

```

Getting values from ini files.

```c++

int 		getInt(std::string Item);
std::string getString(std::string Item);
float 		getFloat(std::string Item);
double 		getDouble(std::string Item);
bool 		getBool(std::string Item);

```

### EXAMPLE ###

An example of saving and retrieving values in an ini file.

```c++

iniparser m_parser("settings/options.ini");

m_parser.setSection("Colors");

m_parser.saveString("Favourite", "Black");

std::cout << "My favoutite color is: " << m_parser.getString("Favourite") << std::endl;


```

