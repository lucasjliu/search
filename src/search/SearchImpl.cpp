//
//  SearchImpl.cpp
//
//  by jiahuan.liu
//  11/26/2016
//
#include <assert.h>
#include <cmath>

#include "SearchImpl.h"

using namespace search;

bool id_score_t::operator<(const id_score_t& o) const
{
    return (score != o.score ? score < o.score : docid < o.docid);
}

result_t::result_t(const url_t& u, const std::string& t)
:url(u), title(t) {}

result_t::result_t(url_t&& u, std::string&& t)
{
    url = std::forward<url_t>(u);
    title = std::forward<std::string>(t);
}

result_t::result_t(result_t&& r)
{
    url = std::forward<url_t>(r.url);
    title = std::forward<std::string>(r.title);
}

result_t::result_t(const result_t& r)
{
    url = r.url;
    title = r.title;
}

result_t& result_t::operator=(result_t&& r)
{
    url = std::forward<url_t>(r.url);
    title = std::forward<std::string>(r.title);
    return (*this);
}

void SearchImpl::doSearch(const std::vector<std::string>& query,
              int k,
              std::vector<result_t>& res)
{
    QueryProcessFunc doc_at_a_time = &Alg::doc_a_time;
    ScoreAlg bm25 = {Alg::term_score_bm25, Alg::score_aggr_add};
    
    vdoc_t vdoc = std::move(doc_at_a_time(query, k, bm25, _index, _docs));
    
    for (doc_id_t doc : vdoc)
    {
        res.emplace_back(_genResult(doc));
    }
}

void SearchImpl::load(const std::string& indexPath, const std::string& docFilesPath)
{
    Indexer::loadJson(_index, indexPath);
    _indexFilePath = indexPath;
    _docs.load(docFilesPath);
}

void SearchImpl::saveIndex()
{
    //Indexer::save(_index, _indexFilePath);
}

result_t SearchImpl::_genResult(doc_id_t docid)
{
    return {_docs.select(docid).url, _docs.select(docid).title};
}

void SearchImpl::Alg::score_aggr_add(score_t& s1, const score_t s2)
{
    s1 += s2;
}

score_t SearchImpl::Alg::term_score_bm25(const word_t& term,
                                         const hit_t& hit,
                                         const InvertedIndex& index,
                                         const DocCollection& docs)
{
    doc_len_t dl = docs.select(hit.docid).len;
    score_t k1 = 1.2; //2.0
    score_t b = 0.75;
    score_t wi = log(((double)docs.num - (double)hit.freq + 0.5) / ((double)hit.freq + 0.5));
    score_t ri =  hit.freq * (k1 + 1) / (hit.freq + k1*(1 - b + b * dl/docs.avgdl));
    return wi * ri;
}

vdoc_t SearchImpl::Alg::doc_a_time(const vterm_t& query,
                                   int k,
                                   const score_alg_t& score_alg,
                                   const InvertedIndex& index,
                                   const DocCollection& docs)
{
    std::vector<id_score_t> vscore;
    
    doc_id_t doc = 0, N = docs.num;
    vdoc_t curr(query.size(), 0);
    id_score_t doc_score;
    
    while (doc < N)
    {
        doc_score.docid = doc;
        doc_score.score = 0;
        for (size_t i = 0; i < query.size(); ++i)
        {
            /*doc_id_t currDocId = index.find(term)->second.vhit[curr[i]].docid;
            if (currDocId < doc)
            {
                while (index.find(term)->second.vhit[++curr[i]].docid < doc);
                currDocId = index.find(term)->second.vhit[curr[i]].docid;
            }
            if (currDocId == doc)
            {
                doc_len_t freq = index.find(term)->second.vhit[++curr[i]].freq;
                score_t term_score = score_alg.term_score_func(term, doc, freq, index, docs);
                score_alg.score_aggr_func(doc_score.score, term_score);
            }*/
            const word_t& term = query[i];
            auto itr = index.find(term);
            if (itr != index.end())
            {
                const auto& tuple = itr->second;
                if (tuple.hasHit(doc))
                {//if hit found, compute term score and aggregate, and then store to vscore
                    for (const auto& hit : tuple.getHits(doc))
                    {
                        score_t term_score = score_alg.term_score_func(term, hit, index, docs);
                        score_alg.score_aggr_func(doc_score.score, term_score);
                    }
                    
                }
            }
        }
        if (doc_score.score > 0)
        {
            //if no random access to index, then here should keep 
            // a vscore with size N
            vscore.emplace_back(doc_score);
        }
        doc++;
    }
    
    getTopk(vscore, k);
    
    vdoc_t ret;
    for (id_score_t& id_score : vscore)
    {
        ret.emplace_back(id_score.docid);
    }
    return ret;
}