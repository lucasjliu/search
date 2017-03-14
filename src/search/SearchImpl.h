//
//  SearchImpl.h
//
//  by jiahuan.liu
//  11/26/2016
//
#ifndef _SEARCHIMPL_H
#define _SEARCHIMPL_H

#include <vector>
#include <queue>
#include <cmath>

#include "SearchBase.h"
#include "Indexer.h"

using namespace search;

namespace search
{
    struct id_score_t
    {
        doc_id_t    docid;
        score_t     score;
        id_score_t()
        :docid(0), score(0) {}
        id_score_t(doc_id_t doc, score_t s)
        :docid(doc), score(s) {}
        bool operator<(const id_score_t& o) const;
    };
    
    struct result_t
    {
        url_t       url;
        std::string title;
        
        result_t(const url_t& u, const std::string& t);
        result_t(url_t&& u, std::string&& t);
        result_t(const result_t& r);
        result_t(result_t&& r);
        result_t& operator=(result_t&& r);
    };
    
    typedef std::vector<word_t> vterm_t;
    
    typedef std::vector<doc_id_t> vdoc_t;
    
    typedef std::vector<result_t> vres_t;
}

class SearchImpl : public Noncopyable
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
    void doSearch(const std::vector<std::string>& query,
                  int k,
                  std::vector<result_t>& res);
    void load(const std::string& indexPath, const std::string& docFilesPath);
    void saveIndex();
    
private:
    result_t _genResult(doc_id_t docid);
    
private:
    InvertedIndex   _index;
    DocCollection   _docs;
    std::string     _indexFilePath;
    
private:
    class Alg
    {
    public:
        using ScoreAggrFunc = std::function<void((score_t&, const score_t))>;
        using TermScoreFunc = std::function<score_t(const word_t&, const hit_t& hit, const InvertedIndex&, const DocCollection&)>;
        struct score_alg_t
        {
            TermScoreFunc term_score_func;
            ScoreAggrFunc score_aggr_func;
        };
        using QueryProcessFunc = std::function<vdoc_t(const vterm_t&, int, score_alg_t, const InvertedIndex&, const DocCollection&)>;
        
        static void score_aggr_add(score_t& s1, const score_t s2);
        static score_t term_score_bm25(const word_t& term,
                                       const hit_t& hit,
                                       const InvertedIndex& index,
                                       const DocCollection& docs);
        static vdoc_t doc_a_time(const vterm_t& query,
                                 int k,
                                 const score_alg_t& score_func,
                                 const InvertedIndex& index,
                                 const DocCollection& docs);
        static vdoc_t term_a_time(const vterm_t& query,
                                  int k,
                                  const score_alg_t& score_func,
                                  const InvertedIndex& index,
                                  const DocCollection& docs);
        template<typename T>
        static void getTopk(std::vector<T>& v, int k);
    };
    using ScoreAggrFunc = Alg::ScoreAggrFunc;
    using TermScoreFunc = Alg::TermScoreFunc;
    using QueryProcessFunc = Alg::QueryProcessFunc;
    using ScoreAlg = Alg::score_alg_t;
};

template<typename T>
void SearchImpl::Alg::getTopk(std::vector<T>& v, int k)
{
    std::priority_queue<T> q(v.begin(), v.end());
    std::vector<T> r;
    while (k--)
    {
        if (q.empty()) break;
        r.push_back(q.top());
        q.pop();
    }
    v.swap(r);
}

#endif //_SEARCHIMPL_H