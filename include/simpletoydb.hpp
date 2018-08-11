#ifndef __SIMPLE_TOY_DB__
#define __SIMPLE_TOY_DB__

#include "toydb.hpp"
#include <map>
#include <fstream>

namespace simpletoydb
{
class CSimpleToydb : public toy::IToyDB
{
    const int MAX_FILE_SIZE = 1024;
    // The db will store offset of the k-v pair in the file
    // this map needs to be populated every time db is started
    typedef int OffSet;
    std::map<toy::KeyType, OffSet> index;
    const char * DB_FILENAME = "simpletoy.db";
    const char * DB_FILENAME_BACK = "simpletoy.db.back";
    std::fstream fs;

    int filesize = 0;
    bool compaction_in_progress = false;

    
    // storing values in memory, remove later
    std::map<toy::KeyType, toy::ValType> values;

  public:
    bool put(const toy::KeyType &name, const toy::ValType & blob);
    bool get(const toy::KeyType &name, toy::ValType &blob);

    CSimpleToydb();
    ~CSimpleToydb();

    void printoffsets();
    void printvalues();
    void compact();
  private:
    void init();
    void openDbFile();
    void readDbFile();
    bool put_internal(const toy::KeyType &name, const toy::ValType & blob);
    bool get_internal(const toy::KeyType &name, toy::ValType &blob);
};
} // namespace simpletoydb

#endif