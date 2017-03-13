//
//  Indexer.cpp
//
//  by jiahuan.liu
//  11/24/2016
//
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <assert.h>

#include "Indexer.h"
#include "Logger.h"
#include "Common.h"

using namespace search;

/*bool Indexer::load(search::InvertedIndex& index,
                   const std::string& filepath)
{
    std::ifstream in(filepath);
    
    if (in.fail())
    {
        LOG_ERROR << "load index file error.";
        throw Exception("load index file error.");
    }
    
    word_t      word;
    std::string line, title;
    
    while (getline(in, line))
    {
        if (line.empty() || ispunct(line[0]))
        {
            LOG_WARN << "skip abnormal hadoop index line (empty line): " << line;
            break;
        }
        if (!Common::divstr(line, "\t\r\n |", word, line))
        {
            LOG_WARN << "abnormal hadoop index line (no title field): " << line;
            break;
        }
        else if(!Common::divstr(line, "|", title, line))
        {
            LOG_WARN << "abnormal hadoop index line (no body field): " << line;
            title = std::move(line);
        }
        
        tuple_t             tuple;
        hit_t               hit;
        std::stringstream   ss;
        
        ss = std::move(std::stringstream(std::move(title)));
        hit.field = Field::TITLE;
        
        while (ss >> hit.docid >> hit.freq)
        {
            tuple.vhit.push_back(std::move(hit));
            tuple.ctf += hit.freq;
        }
        
        ss = std::move(std::stringstream(std::move(line)));
        hit.field = Field::BODY;
        
        while (ss >> hit.docid >> hit.freq)
        {
            tuple.vhit.push_back(std::move(hit));
            tuple.ctf += hit.freq;
        }
        
        index[word] = std::move(tuple);
    }
    
    in.close();
    
    return true;
}

bool Indexer::save(const search::InvertedIndex& index,
                   const std::string& filepath)
{
    std::ofstream out(filepath);
    
    if (out.fail())
    {
        LOG_ERROR << "save index file error.";
        return false;
    }
    
    for (auto& pair : index)
    {
        out << pair.first << "\t";
        Field curr = static_cast<Field>(0);
        for (auto& hit : pair.second.vhit)
        {
            if (curr != hit.field)
            {
                out << "|";
                curr = hit.field;
            }
            out << hit.docid << " " << hit.freq << " ";
        }
        out << std::endl;
    }
    
    out.close();
    
    return true;
}

bool Indexer::loadHadoop(search::InvertedIndex& index,
                         const std::string& filepath)
{
    std::ifstream in(filepath);
    
    if (in.fail())
    {
        LOG_ERROR << "load hadoop index file error.";
        in.close();
        return false;
    }
    
    word_t      word;
    std::string line, title;
    
    while (getline(in, line))
    {
        if (line.empty() || ispunct(line[0]))
        {
            LOG_WARN << "skip abnormal hadoop index line (empty line): " << line;
            break;
        }
        if (!Common::divstr(line, "\t\r\n |", word, line))
        {
            LOG_WARN << "abnormal hadoop index line (no title field): " << line;
            break;
        }
        else if(!Common::divstr(line, "|", title, line))
        {
            LOG_WARN << "abnormal hadoop index line (no body field): " << line;
            title = std::move(line);
        }
        
        tuple_t                 tuple;
        std::vector<doc_id_t>   vdoc;
        std::map<doc_id_t, int> ndoc;
        
        vdoc = std::move(Common::sepstr<doc_id_t>(title, " ", false));
        
        for (doc_id_t& id : vdoc)
        {
            tuple.ctf++;
            ndoc[id]++;
        }
        
        for (std::map<doc_id_t, int>::value_type& doc : ndoc)
        {
            tuple.vhit.emplace_back(doc.first,
                                    static_cast<doc_len_t>(doc.second),
                                    Field::TITLE);
        }
        
        ndoc.clear();
        
        vdoc = std::move(Common::sepstr<doc_id_t>(line, " ", false));
        
        for (doc_id_t& id : vdoc)
        {
            tuple.ctf++;
            ndoc[id]++;
        }
        
        for (std::map<doc_id_t, int>::value_type& doc : ndoc)
        {
            tuple.vhit.emplace_back(doc.first,
                                    static_cast<doc_len_t>(doc.second),
                                    Field::BODY);
        }
        
        index[word] = std::move(tuple);
    }
    
    in.close();
    
    return true;
}*/

bool Indexer::loadJson(search::InvertedIndex& index, const std::string& filepath)
{
    std::ifstream in(filepath);
    if (in.fail())
    {
        LOG_ERROR << "open json index file error.";
        return false;
    }
    
    doc_t tmpDoc;
    std::string word, line, key, val;
    std::string::size_type pos;
    
    getline(in, line);
    
    while(getline(in, line))
    {
        pos = 0;
        
        word = std::move(HtmlUtil::getBetween(line, pos, "\"", "\""));
        if (word.empty())
        {
            continue;
        }
        
        tuple_t tuple;
        
        getline(in, line);
        
        while (line == "{")
        {
            hit_t hit;
            
            getline(in, line);
            pos = 0;
            key = std::move(HtmlUtil::getBetween(line, pos, "\"", "\""));
            if (key != "docid")
            {
                LOG_ERROR << "error " + line;
            }
            val = std::move(HtmlUtil::getBetween(line, pos, "\"", "\""));
            hit.docid = Common::strto<doc_id_t>(std::move(val));
            
            getline(in, line);
            pos = 0;
            key = std::move(HtmlUtil::getBetween(line, pos, "\"", "\""));
            if (key != "field")
            {
                LOG_ERROR << "error " + line;
            }
            val = std::move(HtmlUtil::getBetween(line, pos, "\"", "\""));
            hit.field = val == "title" ? Field::TITLE : Field::BODY;
            
            getline(in, line);
            pos = 0;
            key = std::move(HtmlUtil::getBetween(line, pos, "\"", "\""));
            if (key != "freq")
            {
                LOG_ERROR << "error " + line;
            }
            val = std::move(HtmlUtil::getBetween(line, pos, "\"", "\""));
            hit.freq = Common::strto<doc_id_t>(std::move(val));
            
            getline(in, line);
            getline(in, line);
            
            tuple.ctf += hit.freq;
            //tuple.vhit.push_back(std::move(hit));
            tuple.vhit[hit.docid] = hit;
            
            getline(in, line);
        }
        
        /*sort(tuple.vhit.begin(), tuple.vhit.end(), [](const hit_t& h1, const hit_t& h2)
             {
                 if (h1.docid == h2.docid)
                 {
                     int alert = 0;
                 }
                 return h1.docid > h2.docid;
             });
        if (word == "china")
        {
            int alert = 0;
        }*/
        index[word] = std::move(tuple);
    }
    
    return true;
}