//
//  Searcher.cpp
//
//  by jiahuan.liu
//  11/24/2016
//
#include "Searcher.h"

Searcher::Searcher(int thNum, int qSize)
:_pool(thNum, qSize, false) {}

void Searcher::load(const std::string& indexPath, const std::string& docFilesPath)
{
    _impl.load(indexPath, docFilesPath);
}

void Searcher::start()
{
    _pool.start();
}

void Searcher::stop(bool saveIndex)
{
    _pool.stop();
    _pool.join();
}

void Searcher::doSearchAsyn(const std::vector<std::string>& query,
                            int k,
                            SearchCallback cb)
{
    //_pool.addTask([=]
    //{
        std::vector<result_t> res;
        std::vector<std::string> q = std::move(query);
        _impl.doSearch(std::move(q), k, res);
        cb(res);
    //});
}