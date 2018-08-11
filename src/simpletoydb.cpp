#include "include/simpletoydb.hpp"
#include <iostream>
#include <fstream>

using namespace std;
using namespace simpletoydb;

CSimpleToydb::CSimpleToydb()
{
    fs.open(DB_FILENAME, std::fstream::in | std::fstream::out);
    // if file doesnt exists
    if (!fs)
    {
        // create file
        cout << "creating db file " << DB_FILENAME << endl;
        fs.open(DB_FILENAME, std::fstream::in | std::fstream::out | std::fstream::app);
        if (!fs || !fs.is_open())
        {
            cout << "could not open the db file " << DB_FILENAME << endl;
        }
        else
        {
            cout << "db file opened... " << endl;
        }
    }
    else
    {
        cout << "db file already exists" << endl;
        // read file and create index
        if (fs.peek() != std::ifstream::traits_type::eof())
        {
            while (!fs.eof() && fs.good())
            {
                int size = 1;

                toy::KeyType k;
                fs >> size;
                if (!fs.good())
                {
                    break;
                }
                k.resize(size);
                fs.read(&*k.begin(), size);
                int offset = fs.tellg();

                toy::ValType v;
                fs >> size;
                v.resize(size);
                fs.read(&*v.begin(), size);

                values[k] = v;
                index[k] = offset;
                
                std::cout << k << " => offset(" << index[k] << "), value(" << values[k] << ")" << std::endl;
            }
            // reset the fs to end of file
            fs.seekp(std::ios_base::beg, std::ios_base::end);
            fs.clear();
            cout << "offset after opening " << fs.tellp() << endl;
        }
    }
}

CSimpleToydb::~CSimpleToydb()
{
    fs.close();
}
/**
 * 1. seek to the end of file
 * 2. write key size
 * 3. write key contents
 * 4. get the value of offset now, as this offset will be stored in index
 * 5. write value size
 * 6. write value contents
 * 
**/
bool CSimpleToydb::put(const toy::KeyType &name, const toy::ValType &value)
{
    // get the current offset
    fs.seekp(std::ios_base::beg, std::ios_base::end);
    int offset = fs.tellp();
    cout << "offset before write " << offset << endl;

    fs << name.size();
    fs.write(name.c_str(), name.size());

    offset = fs.tellp();
    fs << value.size();
    fs.write(value.c_str(), value.size());

    fs << std::endl;

    cout << "offset after write " << fs.tellp() << endl;
    fs.flush();
    index[name] = offset;
    values[name] = value;
    return true;
}

bool CSimpleToydb::get(const toy::KeyType &name, toy::ValType & value)
{
    auto offset = index.find(name);
    if(offset != index.end())
    {
        fs.seekg(offset->second, std::ios::beg);
        int size = 1;
        fs >> size;
        value.resize(size);
        fs.read(&*value.begin(), size);
    }
    return true;
}

void CSimpleToydb::printoffsets()
{
    for (auto &t : index)
        std::cout << t.first << "\t=>\t"
                  << t.second << " "
                  << "\n";
}

void CSimpleToydb::printvalues()
{
    for (auto &t : values)
        std::cout << t.first << "\t=>\t"
                  << t.second << " "
                  << "\n";
}
