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
    assert(status.ok());
    return path;
}

void LastPostion::set(const string& path)
{
    leveldb::Status status;
    status = db_->Put(leveldb::WriteOptions(), userid_, path);
    assert(status.ok());
}

LastPostion::~LastPostion()
{
}

