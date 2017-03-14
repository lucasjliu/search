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
    using ServerHandler = handy::HttpServer;
    using ServerPoolHandler = handy::MultiBase;  
public:
    SearchServer(int svrThNum = 2, int schThNum = 2, int schQSize = 64);
    void init(std::string indexPath, std::string docFilesPath);
    void start(int port = 2222);
    void stop();
    ~SearchServer() {stop();}
private:
    Searcher            _searcher;
    ServerHandler*      _server;
    ServerPoolHandler*  _svrPool;
};

#endif //_SEARCHSERVER_H