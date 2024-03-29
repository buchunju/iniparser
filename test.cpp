/*****************************************************
** File    : test.cpp
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

#include <iostream>

#include "ini.h"

int main(int argc, char *argv[])
{
    try
    {
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
    }
    catch (IniError e)
    {
        cout << e.toString() << endl;
    }

    return 0;
}