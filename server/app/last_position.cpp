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


LastPostion::LastPostion(const string &userid)
:userid_(userid)
{
#ifdef DEBUG_PARAM
    std::cout << __func__ <<":userid_=" <<  userid_ << endl;
#endif
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db_);
    assert(status.ok());
}

LastPostion::~LastPostion()
{
    delete db_;
}

