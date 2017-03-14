//
//  SearchServer.cpp
//
//  by jiahuan.liu
//  11/26/2016
//
#include "SearchServer.h"
#include "Util.h"

using namespace std;

handy::HttpServer* server = nullptr;
handy::MultiBase* base = nullptr;

std::string search::toJson(const vres_t& vres)
{
    std::string ret;
    
    std::vector<std::map<std::string, std::string>> list;
    std::map<std::string, std::string> tuple, object;
    
    for (const result_t& res : vres)
    {
        tuple.clear();
        tuple["title"] = res.title;
        tuple["url"] = res.url;
        list.push_back(tuple);
    }
    
    object["n"] = Common::tostr(vres.size());
    object["list"] = JsonUtil::buildJsonList(list);
    
    return JsonUtil::buildJsonObject(object);
}

SearchServer::SearchServer(int svrThNum, int schThNum, int schQSize)
:_searcher(schThNum, schQSize)
{
    stop();
    base = new handy::MultiBase(svrThNum);
    server = new handy::HttpServer(base);
}

void SearchServer::init(std::string indexPath, std::string docFilesPath)
{
    _searcher.load(indexPath, docFilesPath);
    _searcher.start();
}

void SearchServer::stop()
{
    if (server)
    {
        delete server;
    }
    if (base)
    {
        delete base;
    }
}

void SearchServer::start()
{
    int r = server->bind("127.0.0.1", 2222);
    exitif(r, "bind failed %d %s", errno, strerror(errno));

    server->onGet("/", [this](const handy::HttpConnPtr& con) 
    {
        string uri = con.getRequest().query_uri;
        string v = con.getRequest().version;
        size_t pos;
        if ((pos = uri.find("?")) == std::string::npos)
        {
            con.sendFile("html/search.html");
        }
        else
        {
            string q = std::move(uri.substr(pos+1));
            vector<string> query = std::move(Common::sepstr<std::string>(q, "+", false));
            
            _searcher.doSearchAsyn(query, 10, [&con, v](std::vector<result_t>&&res)
            {
                handy::HttpResponse resp;
                resp.body = search::toJson(std::forward<std::vector<result_t>>(res));
                con.sendResponse(std::move(resp));

                if (v == "HTTP/1.0") 
                {
                    con->close();
                }
            });
        }
    });

    /*server->onGet("/hadoop", [this](const handy::HttpConnPtr& con) 
    {
        string uri = con.getRequest().query_uri;
        string v = con.getRequest().version;
        size_t pos;
        if ((pos = uri.find("?")) == std::string::npos)
        {
            con.sendFile("html/search.html");
        }
        else
        {
            string q = std::move(uri.substr(pos+1));
            vector<string> query = std::move(Common::sepstr<std::string>(q, "+", false));
            
            _searcher.doSearchAsyn(query, 10, [&con, v](const std::vector<result_t>&res)
            {
                handy::HttpResponse resp;
                resp.body = search::toJson(res);
                con.sendResponse(resp);

                if (v == "HTTP/1.0") 
                {
                    con->close();
                }
            });
        }
    });

    server->onGet("/lucene", [this](const handy::HttpConnPtr& con) 
    {
        string uri = con.getRequest().query_uri;
        string v = con.getRequest().version;
        size_t pos;
        if ((pos = uri.find("?")) == std::string::npos)
        {
            con.sendFile("html/search.html");
        }
        else
        {
            string q = std::move(uri.substr(pos+1));
            vector<string> query = std::move(Common::sepstr<std::string>(q, "+", false));
            _searcher.doSearchAsyn(query, 10, [&con, v](const std::vector<result_t>&res)
            {
                handy::HttpResponse resp;
                resp.body = search::toJson(res);
                con.sendResponse(resp);

                if (v == "HTTP/1.0") 
                {
                    con->close();
                }
            });
        }
    });*/

    server->onGet("/search.css", [this](const handy::HttpConnPtr& con) 
    {
        string v = con.getRequest().version;
        con.sendFile("html/search.css");
        if (v == "HTTP/1.0") 
        {
            con->close();
        }
    });

    handy::Signal::signal(SIGINT, [&]{base->exit();});
    handy::Signal::signal(SIGTSTP, [&]{base->exit();});
    base->loop();
}