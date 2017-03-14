//
//  SearchBase.h
//
//  @brief: basic data structure and type definition for searching
//
//  by jiahuan.liu
//  11/23/2016
//
#ifndef _SEARCHBASE_H
#define _SEARCHBASE_H

#include <string>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <utility>
#include <fstream>

#include "ThreadSync.h"
#include "Common.h"
#include "Logger.h"

namespace search
{
    typedef uint32_t    doc_id_t;
    typedef uint32_t    word_id_t;
    typedef std::string word_t;
    typedef uint16_t    doc_len_t;
    typedef float       score_t;
    typedef std::string url_t;
    
    enum Field
    {
        TITLE,
        BODY,
    };
    
    //word hits in a doc. arranged by field and counted by frequency
    struct hit_t 
    {
        doc_id_t    docid;
        doc_len_t   freq;   //term frequence in a doc
        Field       field;  //url, title, body, etc
        hit_t() {}
        hit_t(doc_id_t doc, doc_len_t f, Field fd) :docid(doc), freq(f), field(fd) {}
        bool operator==(const hit_t& o) const {return docid == o.docid && freq == o.freq && field == o.field;}
    };

    //statistical information of a word in the whole collection
    struct tuple_t 
    {
        doc_id_t            ctf;        //term frequency in collection
        //std::vector<hit_t>  vhit;       //list of word-doc hits
        std::unordered_map<doc_id_t, hit_t> vhit;
        tuple_t() :ctf(0) {}
        bool hasHit(doc_id_t i) const {return vhit.find(i) != vhit.end();}
        const hit_t& getHit(doc_id_t i) const {return vhit.find(i)->second;}
        bool operator==(const tuple_t& o) const {return ctf == o.ctf && vhit == o.vhit;}
    };
    
    typedef std::unordered_map<word_t, tuple_t> InvertedIndex;
    
    //information of a document
    struct doc_t
    {
        //std::string path;
        url_t       url;
        std::string title;
        doc_len_t   len;
        
        doc_t() :len(0) {}
        doc_t(const url_t& u, const std::string& t, const doc_len_t& l);
        doc_t(url_t&& u, std::string&& t, doc_len_t&& l);
        doc_t(const doc_t& docfile);
        doc_t(doc_t&& docfile);
        doc_t& operator=(doc_t&& docfile);
        doc_t& operator=(const doc_t& docfile);
        
        bool empty();
        std::string toStr();
    };
    
    template<typename I, typename T>
    class collection_base
    {
    public:
        typedef I   index_t;
        typedef T   value_t;
        typedef T*  ptr_t;
    public:
        virtual void insert(const index_t& index, value_t&& value) = 0;
        virtual void insert(const index_t& index, const value_t& value) = 0;
        virtual const value_t& select(const index_t& index) const = 0;
        virtual value_t& operator[](const index_t& index) = 0;
        virtual void clear() = 0;
    };
    
    template<typename I, typename T>
    class collection : public collection_base<I, T>, public ThreadLock
    {
    public:
        typedef typename collection_base<I, T>::index_t index_t;
        typedef typename collection_base<I, T>::value_t value_t;
        typedef typename collection_base<I, T>::ptr_t   ptr_t;
    public:
        virtual void insert(const index_t& index, value_t&& value)
        {
            Lock(*this);
            if (_vbuf.size() <= index)
            {
                _vbuf.resize(index + 1);
            }
            _vbuf[index] = std::forward<value_t>(value);
        }
        virtual void insert(const index_t& index, const value_t& value)
        {
            Lock(*this);
            if (_vbuf.size() <= index)
            {
                _vbuf.resize(index + 1);
            }
            _vbuf[index] = value;
        }
        virtual const value_t& select(const index_t& index) const
        {
            return _vbuf[index];
        }
        virtual value_t& operator[](const index_t& index)
        {
            return _vbuf[index];
        }
        virtual void clear()
        {
            _vbuf.clear();
        }
    private:
        std::vector<value_t> _vbuf;
    };
    
    class DocCollection : public collection<doc_id_t, doc_t>
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
        DocCollection()
        :avgdl(0), num(0) {}
        virtual void save(std::string filepath) const
        {
            
        }
        virtual void load(std::string filepath)
        {
            avgdl = 0;
            num = 0;
            clear();
            
            unsigned long long sum = 0;
            doc_t tmpDoc;
            doc_id_t tmpId;
            std::vector<std::string> tmpv;
            std::string line, newLine;
            char ch;
            
            std::fstream in(filepath);
            if (in.fail())
            {
                LOG_FATAL << "document record file loading error:" << filepath;
                throw Exception("document record file loading error.");
            }
            
            while (getline(in, newLine))
            {
                if (newLine.length() == 4 || newLine.empty())
                {
                    continue;
                }
                line += newLine; //incase a line is divided to multiple accidently

                //remove noise and check errors
                if (line.find("| Reuters") != std::string::npos)
                {
                    size_t pos = line.find("| Reuters");
                    line.erase(pos, strlen("| Reuters"));
                }
                tmpv = std::move(Common::sepstr<std::string>(line, "|"));

                //lengtn should be 6, otherwise treat as exception
                if (tmpv.size() > 6)
                {
                    std::cout << line << std::endl;
                }
                if (tmpv.size() < 6)
                {
                    continue;
                }
                
                tmpId = Common::strto<doc_id_t>(std::move(tmpv[0]));
                tmpDoc.url = std::move(tmpv[1]);
                tmpDoc.title = std::move(tmpv[2]);
                tmpDoc.len = Common::strto<doc_len_t>(std::move(tmpv[4])) +
                    Common::strto<doc_len_t>(std::move(tmpv[5]));
                
                sum += tmpDoc.len;
                num++;
                insert(tmpId, std::move(tmpDoc));
                line.clear();
            }
            
            avgdl = (long double)sum / num;
        }
        score_t avgdl; //average docutment lengeh, for BM25
        doc_id_t num; //document number
    };
}

#endif //_SEARCHBASE_H