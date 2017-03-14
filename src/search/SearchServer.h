//
//  SearchServer.h
//
//  by jiahuan.liu
//  11/26/2016
//
#ifndef _SEARCHSERVER_H
#define _SEARCHSERVER_H

#include "Searcher.h"
#include "handy/handy.h"

using namespace search;

namespace search
{
    std::string toJson(const vres_t& vres);
}

class SearchServer
{
public:
    SearchServer(int svrThNum = 2, int schThNum = 2, int schQSize = 64);
    void init(std::string indexPath, std::string docFilesPath);
    void start();
    void stop();
    ~SearchServer() {stop();}
private:
    Searcher    _searcher;
};

#endif //_SEARCHSERVER_H