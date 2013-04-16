#include "last_position.hpp"
using namespace BOW;


#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>


#include <cassert>
#include "leveldb/db.h"

#define DEBUG_PARAM
#define DEBUG_GET
#define DEBUG_SET

void dump_bt(void);


const string LastPostion::db_home_("/opt/BuildOnWeb/leveldb");

LastPostion::LastPostion(const string &userid)
:userid_(userid)
,db_()
{
#ifdef DEBUG_PARAM
    std::cout << __func__ <<":userid_=" <<  userid_ << endl;
#endif
    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, db_home_ + "/last_position.db", &db);
    assert(status.ok());
    db_ = shared_ptr<leveldb::DB>(db);
}

const string LastPostion::get(void) const
{
    string path;
    leveldb::Status status;
    status = db_->Get(leveldb::ReadOptions(), userid_, &path);
    if(status.IsNotFound())
    {
#ifdef DEBUG_GET
        std::cout << __func__ <<":IsNotFound=" <<  path << endl;
#endif
        status = db_->Put(leveldb::WriteOptions(), userid_, "c_cxx/helloworld/main.c");
        assert(status.ok());
        status = db_->Get(leveldb::ReadOptions(), userid_, &path);
    }
    assert(status.ok());
#ifdef DEBUG_GET
    std::cout << __func__ <<":path=" <<  path << endl;
#endif
    return path;
}

void LastPostion::set(const string& path)
{
    leveldb::Status status;
    status = db_->Put(leveldb::WriteOptions(), userid_, path);
    assert(status.ok());
#ifdef DEBUG_SET
    string new_path;
    status = db_->Get(leveldb::ReadOptions(), userid_, &new_path);
    std::cout << __func__ <<":path=" <<  path << endl;
    std::cout << __func__ <<":new_path=" <<  new_path << endl;
    dump_bt();
#endif
}

LastPostion::~LastPostion()
{
}


#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void dump_bt(void)
{
    const size_t SIZE = 1024;
    void *buffer[SIZE];
    char **strings;
    
    int nptrs = backtrace(buffer, SIZE);
    printf("backtrace() returned %d addresses\n", nptrs);
    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        perror("backtrace_symbols");
        return;
    }
    for(int i = 0; i < nptrs; i++)
    {
        printf("%s\n", strings[i]);
    }
    free(strings);
}
