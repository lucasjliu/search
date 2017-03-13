//
//  SearchServer.h
//
//  by jiahuan.liu
//  11/26/2016
//
#ifndef _SEARCHSERVER_H
#define _SEARCHSERVER_H

#include <vector>
#include <map>
#include <sstream>

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
    void init(std::string indexPath, std::string docFilesPath, std::string lindexPath);
    void start();
    void stop();
    ~SearchServer() {stop();}
private:
    Searcher    _searcher;
    //Searcher    _lsearcher;
};

class JsonUtil
{//TODO: move and forward
public:
    static std::string buildJsonObject(std::map<std::string, std::string>& mdata)
    {
        std::string sTemp;
        std::ostringstream sStr;
        sStr << "{";
        for(std::map<std::string, std::string>::iterator it = mdata.begin(); it != mdata.end(); it++)
        {
            sTemp = it->second;
            if(it == mdata.begin())
            {
                if(it->first == "list")
                {
                    sStr << "\"" << it->first << "\":" << sTemp;
                }
                else
                {
                    sStr << "\"" << it->first << "\":\"" << sTemp << "\"";
                }
            }
            else
            {
                if(it->first == "list")
                {
                    sStr << ",\"" << it->first << "\":" << sTemp;
                }
                else
                {
                    sStr << ",\"" << it->first << "\":\"" << sTemp << "\"";
                }
            }
        }
        sStr << "}";
        
        return sStr.str();
    }
    
    static std::string buildJsonTuple(std::map<std::string, std::string>& mdata)
    {
        std::string sTemp;
        std::ostringstream sStr;
        sStr << "{";
        for(std::map<std::string, std::string>::iterator it = mdata.begin(); it != mdata.end(); it++)
        {
            sTemp = it->second;
            if(it == mdata.begin())
            {
                sStr << "\"" << it->first << "\":\"" << sTemp << "\"";
            }
            else
            {
                sStr << ",\"" << it->first << "\":\"" << sTemp << "\"";
            }
        }
        sStr << "}";
        return sStr.str();
    }
    
    static std::string buildJsonList(std::vector<std::map<std::string, std::string>>& v_mData)
    {
        std::ostringstream sStr;
        sStr << "[";
        for(size_t i=0; i<v_mData.size(); i++)
        {
            if(i == 0)
            {
                sStr << buildJsonTuple(v_mData[i]);
            }
            else
            {
                sStr << "," << buildJsonTuple(v_mData[i]);
            }
        }
        sStr << "]";
        return sStr.str();
    }
};

#endif //_SEARCHSERVER_H