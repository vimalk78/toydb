#include <iostream>
#include <sstream>
#include <string.h>
#include <thread>
#include "include/simpletoydb.hpp"
using namespace std;

void prompt(toy::IToyDB *db)
{
    bool running = true;
    while (running)
    {
        cout << "\n put/get/index/values/init/quit \n >> ";
        string command;
        std::getline(std::cin, command);
        istringstream iss(command);
        string cmdname;
        iss >> cmdname;
        if (cmdname == "init")
        {
            db->put("name", "Vimal");
            db->put("age", "ancient");
            db->put("occupation", "jobless");
        }
        if (cmdname == "put")
        {
            string name;
            iss >> name;
            string value;
            iss >> value;
            db->put(name, value);
        }
        if (cmdname == "get")
        {
            string name;
            iss >> name;
            string value;
            db->get(name, value);
            cout << "value is : " << value << endl;
        }
        if (cmdname == "index")
        {
            simpletoydb::CSimpleToydb *sdb = dynamic_cast<simpletoydb::CSimpleToydb *>(db);
            sdb->printoffsets();
        }
        if (cmdname == "values")
        {
            simpletoydb::CSimpleToydb *sdb = dynamic_cast<simpletoydb::CSimpleToydb *>(db);
            sdb->printvalues();
        }
        if (cmdname == "quit")
        {
            running = false;
        }
    }
}

int main(int argc, char const *argv[])
{
    toy::IToyDB *db = new simpletoydb::CSimpleToydb();
    cout << "This is a toy db. This will be fun." << endl;
    thread p(prompt, db);
    p.join();
    delete db;
    return 0;
}