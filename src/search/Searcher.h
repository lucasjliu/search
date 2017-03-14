//
//  Searcher.h
//
//  @brief: interface for searching worker instance
//          handle multi-thread and asynchronized request
//
//  by jiahuan.liu
//  11/24/2016
//
#ifndef _SEARCHER_H
#define _SEARCHER_H

#include "SearchImpl.h"
#include "ThreadPool.h"

class Searcher
{
public:
    typedef std::function<void(std::vector<result_t>&&)> SearchCallback;
    typedef SearchCallback* SearchCallbackPtr;
public:
    Searcher(int thNum, int qSize);
    void doSearchAsyn(const std::vector<std::string>& query,
                      int k,
                      SearchCallback cb);
    void load(const std::string& indexPath, const std::string& docFilesPath);
    void start();
    void stop(bool saveIndex = false);
    //~Searcher() {stop();}
private:
    ThreadPool  _pool;
    SearchImpl  _impl;
};

#endif //_SEARCHER_H