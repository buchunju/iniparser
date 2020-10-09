#include "ini.h"

#include <unistd.h>

// Buffers.
static bool   b_buff = false;
static int    i_buff;
static float  f_buff;
static double d_buff;

iniparser::iniparser(std::string filename)
{
    this->filename = filename;
}

iniparser::~iniparser()
{

}

void iniparser::setSection(std::string section)
{
    this->current_section = section;
}

void iniparser::saveInt(std::string Item, int value)
{
    this->write_to_file(V_INT, Item, &value);
}

void iniparser::saveString(std::string Item, std::string value)
{
    this->write_to_file(V_STRING, Item, &value);
}

void iniparser::saveFloat(std::string Item, float value)
{
    this->write_to_file(V_FLOAT, Item, &value);
}

void iniparser::saveDouble(std::string Item, double value)
{
    this->write_to_file(V_DOUBLE, Item, &value);
}

void iniparser::saveBool(std::string Item, bool value)
{
    this->write_to_file(V_BOOL, Item, &value);
}

int iniparser::getInt(std::string Item)
{
    return *(int*)this->get_from_file(V_INT, Item);
}

std::string iniparser::getString(std::string Item)
{
    return *(std::string*)this->get_from_file(V_STRING, Item);
}

float iniparser::getFloat(std::string Item)
{
    return *(float*)this->get_from_file(V_FLOAT, Item);
}

double iniparser::getDouble(std::string Item)
{
    return *(double*)this->get_from_file(V_DOUBLE, Item);
}

bool iniparser::getBool(std::string Item)
{
    return *(bool*)this->get_from_file(V_BOOL, Item);
}

std::string iniparser::getLastError()
{
    return this->last_error;
}

int iniparser::write_to_file(iniparser::value_type m_type, std::string Item, void *value)
{
    this->last_error = NO_ERROR;

    if(this->current_section.length() == 0)
    {
        this->last_error = "Section not set.";
        return 1;
    }

    std::ifstream reader;
    std::ofstream writer;

    if(access(filename.c_str(), F_OK) == -1) // if file  doesnt exist
    {
        FILE *m_file = fopen(filename.c_str(), "w");
        fclose(m_file);
    }

    reader.open(filename);

    if(reader.is_open())
    {
        std::string line;
        std::string buffer;
        bool item_added    = false;
        bool section_found = false;

        while(std::getline(reader, line))
        {
            buffer += line + "\n";
            if(line.find("[" + this->current_section + "]") != std::string::npos) // found section
            {
                section_found = true;
                std::string item_line;

                while(std::getline(reader, item_line))
                {
                    if(item_line.find(Item) != std::string::npos)
                    {
                        buffer += gen_ouput(m_type, Item, value);
                        item_added = true;
                    }
                    else if(item_line.find("[") != std::string::npos && !item_added) // reached the next  section
                    {
                        buffer += gen_ouput(m_type, Item, value);
                        item_added = true;
                        buffer += item_line + "\n";
                    }
                    else
                    {         
                        buffer += item_line + "\n";
                    }
                    
                }
            }      
        }

        if(!item_added) // if not added. Probably reached end of file.
        {
            if(!section_found) // section not found in file.
                buffer += "[" + this->current_section + "]\n";

            buffer += gen_ouput(m_type, Item, value);
        }

        writer.open(filename);
        if(writer.is_open())
        {
            writer << buffer;
            writer.close();
        }
        else
        {
            this->last_error = "Failed to open ini file.";
        }
        

        return 0;
    }
    else {
        this->last_error = "Failed to open ini file: ";
        return 1;
    }
}

std::string iniparser::gen_ouput(value_type m_type, std::string Item, void* value)
{
    std::string buffer;
    switch (m_type)
    {
    case V_INT:
        buffer = Item + "=" + std::to_string(*(int*)value);
        break;
    case V_STRING:
        buffer = Item + "=" + *(std::string*)value;
        break;
    case V_FLOAT:
        buffer = Item + "=" + std::to_string(*(float*)value);
        break;
    case V_DOUBLE:
        buffer = Item + "=" + std::to_string(*(double*)value);
        break;
    case V_BOOL:
        buffer = Item + "=" + (((bool)value)? std::to_string(1) : std::to_string(0));
        break;
    }

    return buffer + "\n";
}

void* iniparser::get_from_file(value_type m_type, std::string Item)
{
    this->last_error = NO_ERROR;

    std::ifstream reader;

    reader.open(this->filename);

    if(reader.is_open())
    {
        std::string line;

        while(std::getline(reader , line))
        {
            if(line.find("[" + this->current_section + "]") != std::string::npos) // found section
            {
                std::string temp_line; 

                while(std::getline(reader, temp_line))
                {
                    if(temp_line.find(Item) != std::string::npos) // found item
                    {
                        int index = temp_line.find("=");
                        std::string data = temp_line.substr(index + 1);
                        return format_value(m_type, data);
                    }
                }
            }            
        }
    }
    else
    {
        this->last_error = "Failed to open ini file.";
    }
    
    return 0; // nullptr causes segfault.
}

void* iniparser::format_value(value_type typ, std::string value)
{ 

    void  *buff = nullptr;
    
    switch (typ)
    {
    case V_INT:
        i_buff = std::stoi(value);
        buff = &i_buff;
        break;
    case V_BOOL:
        b_buff = (std::stoi(value))? true: false;
        buff = &b_buff;
        break;
    case V_FLOAT:
        f_buff = std::stof(value);
        buff = &value;
        break;
    case V_DOUBLE:
        d_buff = std::stod(value);
        buff = &value;
        break;
    case V_STRING:
        buff = &value;
        break;
    }

    return buff;
}