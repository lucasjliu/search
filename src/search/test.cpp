//
//  test.cpp
//
//  by jiahuan.liu
//  10/12/2016
//
#include "SearchServer.h"

void test_indexer();
void test_searcher();
void test_json();
void test_json_index();
void test_searchImpl();
void test_server();

int main()
{
    //LOG << "begin" << END;
    __TRY__
    test_server();
    //test_searchImpl();
    //test_json();
    //test_searcher();
    __CATCH__(Exception& e)
    LOG << e.what() << END;
    __END__
    //LOG << "end" << END;
}

void test_indexer()
{
    /*search::InvertedIndex index1;
    Indexer::loadHadoop(index1, "test_hadoop_index");
    Indexer::save(index1, "test_save_index");
    search::InvertedIndex index2;
    Indexer::load(index2, "test_save_index");
    assert(index1 == index2);*/
}

void test_searcher()
{
    Searcher searcher(4, 128);
    searcher.load("./data/lucene_index", "./data/docs");
    searcher.start();
    std::vector<result_t> res;
    searcher.doSearchAsyn({"china"}, 10, [](const std::vector<result_t>& res){LOG_INFO << res.size();});
    searcher.doSearchAsyn({"china", "economy"}, 10, [](const std::vector<result_t>& res){LOG_INFO << res[0].title;});
    searcher.stop();
}

void test_json()
{
    vres_t list =
    {
        {"title1", "url1"},
        {"title2", "url2"},
        {"title3", "url3"},
        {"title4", "url4"},
        {"title5", "url5"},
        {"title6", "url6"},
        {"title7", "url7"},
        {"title8", "url8"},
        {"title9", "url9"},
        {"title10", "url10"}
    };
    std::string json = search::toJson(list);
    std::cout << json << std::endl;
}

void test_json_index()
{
    search::InvertedIndex index;
    LOG_INFO << "begin loading";
    Indexer::loadJson(index, "data/lucene_index");
    LOG_INFO << "end loading";
    assert(index["1.19"].vhit.size() == 12);
    assert(index["braggadocious"].vhit.size() == 3);
    assert(index["salla"].vhit[0].docid == 10872);
    assert(index["salla"].vhit[0].freq == 2);
}

void test_searchImpl()
{
    SearchImpl impl;
    LOG_INFO << "begin loading";
    impl.load("data/lucene_index", "data/docs");
    LOG_INFO << "end loading";
    std::vector<result_t> result;
    
    LOG_INFO << "begin searching";
    impl.doSearch({"china"}, 10, result);
    LOG_INFO << "end searching";
    LOG_INFO << "begin searching";
    impl.doSearch({"china", "economy"}, 10, result);
    LOG_INFO << "end searching";
    int i;
}

void test_server()
{
    SearchServer ss(2, 2, 128);
    LOG_INFO << "loading files..";
    ss.init("data/lucene_index", "data/docs", "data/lucene_index");
    LOG_INFO << "finish loading.";
    ss.start();
}