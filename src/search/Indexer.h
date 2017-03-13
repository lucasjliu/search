//
//  Indexer.h
//
//  by jiahuan.liu
//  11/24/2016
//
#ifndef _INDEXER_H
#define _INDEXER_H

#include "SearchBase.h"
#include "Util.h"

class Indexer
{
public:
    struct Exception: public ::Exception
    {
        Exception(const std::string& strerr) : ::Exception(strerr) {}
        Exception(const std::string& strerr, int ierr)
        : ::Exception(strerr, ierr) {}
        ~Exception() noexcept {}
    };
public:
    /*static bool load(search::InvertedIndex& index,
                     const std::string& filepath);

    static bool save(const search::InvertedIndex& index,
                     const std::string& filepath);
    
    static bool loadHadoop(search::InvertedIndex& index,
                           const std::string& filepath);*/
    
    static bool loadJson(search::InvertedIndex& index,
                         const std::string& filepath);
};

#endif //_INDEXER_H