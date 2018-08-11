#ifndef __TOYDB__
#define __TOYDB__

#include <string>

namespace toy
{
typedef std::string KeyType;
typedef std::string ValType;

class IToyDB
{
  public:
    virtual ~IToyDB(){};
    virtual bool put(const KeyType &name, const ValType & blob) = 0;
    virtual bool get(const KeyType &name, ValType &blob) = 0;
};
} // namespace toy
#endif