#include "include/simpletoydb.hpp"
#include <iostream>
#include <fstream>

using namespace std;
using namespace simpletoydb;

#define GUARD_COMPACTION(ret) \
    if(compaction_in_progress) \
    { \
        cout << "compaction_in_progress" << endl; \
        return ret; \
    } 

CSimpleToydb::CSimpleToydb()
{
    init();
}

void CSimpleToydb::init()
{
    openDbFile();
    readDbFile();
}

void CSimpleToydb::readDbFile()
{
    // read file and create index
    //if (fs.peek() != std::ifstream::traits_type::eof())
    {
        while (!fs.eof() && fs.good())
        {
            int size = 1;
            int entrysize = 0;

            toy::KeyType k;
            fs >> size;
            if (!fs.good())
            {
                break;
            }
            entrysize += 4;
            k.resize(size);
            fs.read(&*k.begin(), size);
            entrysize += size;
            int offset = fs.tellg();

            toy::ValType v;
            fs >> size;
            entrysize += 4;
            v.resize(size);
            fs.read(&*v.begin(), size);
            entrysize += size;
            entrysize += 1;
            filesize += entrysize;

            values[k] = v;
            index[k] = offset;
            
            std::cout << k << " => offset(" << index[k] << "), value(" << values[k] << ")" << std::endl;
        }
        // reset the fs to end of file
        fs.seekp(std::ios_base::beg, std::ios_base::end);
        fs.clear();
        cout << "offset after opening " << fs.tellp() << endl;
        cout << "file size : " << filesize << endl;
    }
}

void CSimpleToydb::openDbFile()
{
    fs.open(DB_FILENAME, std::fstream::in | std::fstream::out | std::fstream::app);
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
            cout << "db file created... " << endl;
        }
    }
    else
    {
        cout << "db file already exists" << endl;
        // read file and create index
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
    GUARD_COMPACTION(false);
    return put_internal(name,value);
}
bool CSimpleToydb::put_internal(const toy::KeyType &name, const toy::ValType &value)
{
    // get the current offset
    fs.seekp(std::ios_base::beg, std::ios_base::end);
    int offset = fs.tellp();
    cout << "offset before write " << offset << endl;

    int entrysize = 0;
    fs << name.size();
    entrysize += 4;
    fs.write(name.c_str(), name.size());
    entrysize += name.size();

    offset = fs.tellp();
    fs << value.size();
    entrysize += 4;
    fs.write(value.c_str(), value.size());
    entrysize += value.size();

    fs << std::endl;
    entrysize += 1;


    filesize += entrysize;

    cout << "offset after write " << fs.tellp() << endl;
    cout << "file size : " << filesize << endl;
    fs.flush();
    index[name] = offset;
    values[name] = value;
    return true;
}

bool CSimpleToydb::get(const toy::KeyType &name, toy::ValType & value)
{
    GUARD_COMPACTION(false);
    return get_internal(name,value);
}
bool CSimpleToydb::get_internal(const toy::KeyType &name, toy::ValType & value)
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

void CSimpleToydb::compact()
{
    compaction_in_progress = true;
    // backup file
    if(std::rename(DB_FILENAME, DB_FILENAME_BACK))
    {
        std::perror("Error renaming");
        cout << "could not rename the file... " << endl;
        return;
    }
    fs.close();
    // make a copy of values map
    std::map<toy::KeyType, toy::ValType> copy(values);
    // clear index map
    // clear values map
    values.clear();
    index.clear();
    filesize = 0;
    // create DB file again
    openDbFile();
    // for each value in values map, put(k,v)
    for (auto &t : copy)
    {
        put_internal(t.first, t.second);
    }
    if(0 != std::remove(DB_FILENAME_BACK))
    {
        cout << "could not delete the backup file" << endl;
    }
    
    compaction_in_progress = false;
}

void CSimpleToydb::printoffsets()
{
    GUARD_COMPACTION();
    for (auto &t : index)
        std::cout << t.first << "\t=>\t"
                  << t.second << " "
                  << "\n";
}

void CSimpleToydb::printvalues()
{
    GUARD_COMPACTION();
    for (auto &t : values)
        std::cout << t.first << "\t=>\t"
                  << t.second << " "
                  << "\n";
}
