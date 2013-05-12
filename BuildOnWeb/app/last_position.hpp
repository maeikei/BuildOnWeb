#ifndef BOW_LAST_POSITION_HPP
#define BOW_LAST_POSITION_HPP
#include <string>
#include <list>
#include <map>
#include <memory>
using namespace std;

namespace leveldb {
    class DB;
}

namespace BOW {
    class LastPostion
    {
    public:
        LastPostion(const string &userid);
        const string get(void) const;
        void set(const string& path);
        ~LastPostion();
    private:
        const string userid_;
        shared_ptr<leveldb::DB> db_;
        static const string db_home_;
    };
}
#endif // BOW_LAST_POSITION_HPP
