//
//  Util.h
//
//  by jiahuan.liu
//  10/31/2016
//

#ifndef _UTIL_H
#define _UTIL_H

#include <mutex>
#include <set>
#include <deque>
#include <fstream>
#include <sstream>

#include <vector>
#include <map>

#include "Common.h"
#include "Logger.h"
#include "Exception.h"

class HtmlUtil
{
public:
    struct Exception : ::Exception
    {
        Exception(const std::string& strerr) : ::Exception(strerr) {}
        Exception(const std::string& strerr, int ierr)
        : ::Exception(strerr, ierr) {}
        ~Exception() noexcept {}
    };
public:
    //getLine
    typedef std::string::size_type Pos;
    static Pos skipTo(const std::string& content, Pos& pos, std::string tag)
    {
        if (pos == std::string::npos) return pos;
        return (pos = content.find(tag, pos));
    }
    static Pos skip(const std::string& content, Pos& pos, std::string tag)
    {
        if (pos == std::string::npos) return pos;
        pos = content.find(tag, pos);
        if (pos != std::string::npos)
            pos += tag.length();
        return pos;
    }
    static std::string getBetween(const std::string& content, Pos& pos,
                                  const std::string& begTag, const std::string& endTag)
    {
        if (pos == std::string::npos) return "";
        skip(content, pos, begTag);
        std::string sRet;
        if (pos != std::string::npos)
        {
            Pos pos1 = content.find(endTag, pos);
            if (pos1 != std::string::npos)
            {
                sRet = content.substr(pos, pos1 - pos);
                pos1 += endTag.length();
            }
            pos = pos1;
        }
        return sRet;
    }
    static std::string getBetween(const std::string& content, const Pos& p,
                                  const std::string& begTag, const std::string& endTag)
    {
        Pos pos = p;
        return getBetween(content, pos, begTag, endTag);
    }
    static void filter(std::string& sUrl)
    {
        std::stringstream ss(sUrl);
        std::string sTmp;
        sUrl.clear();
        while (ss >> sTmp)
        {
            sUrl += sTmp;
        }
    }
    static void preClean(std::string& content, std::set<std::string> tags)
    {
        
    }
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

#endif