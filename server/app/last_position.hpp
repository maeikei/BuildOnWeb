#ifndef BOW_LAST_POSITION_HPP
#define BOW_LAST_POSITION_HPP
#include <string>
#include <list>
#include <map>
using namespace std;

namespace leveldb {
    class DB;
}

namespace BOW {
    class LastPostion
    {
    public:
        LastPostion(const string &userid);
        ~LastPostion();
    private:
        const string userid_;
        leveldb::DB *db_;
    };
}
#endif // BOW_LAST_POSITION_HPP
