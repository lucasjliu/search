//
//  SearchBase.cpp
//
//  by jiahuan.liu
//  11/23/2016
//
#include "SearchBase.h"

using namespace search;

doc_t::doc_t(const url_t& u, const std::string& t, const doc_len_t& l)
:url(u), title(t), len(l) {}

doc_t::doc_t(url_t&& u, std::string&& t, doc_len_t&& l)
{
    url = std::forward<url_t>(u);
    title = std::forward<std::string>(t);
    len = std::forward<doc_len_t>(l);
}

doc_t::doc_t(const doc_t& docfile)
{
    //path = docfile.path;
    url = docfile.url;
    title = docfile.title;
    len = docfile.len;
}

doc_t::doc_t(doc_t&& docfile)
{
    //path = std::forward<std::string>(docfile.path);
    url = std::forward<url_t>(docfile.url);
    title = std::forward<std::string>(docfile.title);
    len = std::forward<doc_len_t>(docfile.len);
}

doc_t& doc_t::operator=(const doc_t& docfile)
{
    //path = docfile.path;
    url = docfile.url;
    title = docfile.title;
    len = docfile.len;
    return (*this);
}

doc_t& doc_t::operator=(doc_t&& docfile)
{
    //path = std::forward<std::string>(docfile.path);
    url = std::forward<url_t>(docfile.url);
    title = std::forward<std::string>(docfile.title);
    len = std::forward<doc_len_t>(docfile.len);
    return (*this);
}

bool doc_t::empty()
{
    return url.empty() && title.empty();
}

std::string doc_t::toStr()
{
    return url + " " + title;
}