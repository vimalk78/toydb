#ifndef __SIMPLE_TOY_DB__
#define __SIMPLE_TOY_DB__

#include "toydb.hpp"
#include <map>
#include <fstream>

namespace simpletoydb
{
class CSimpleToydb : public toy::IToyDB
{
    // The db will store offset of the k-v pair in the file
    // this map needs to be populated every time db is started
    typedef int OffSet;
    std::map<toy::KeyType, OffSet> index;
    const char * DB_FILENAME = "simpletoy.db";
    std::fstream fs;

    
    // storing values in memory, remove later
    std::map<toy::KeyType, toy::ValType> values;

  public:
    bool put(const toy::KeyType &name, const toy::ValType & blob);
    bool get(const toy::KeyType &name, toy::ValType &blob);

    CSimpleToydb();
    ~CSimpleToydb();

    void printoffsets();
    void printvalues();
};
} // namespace simpletoydb

#endif