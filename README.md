## Overview

This is a simple C++11 search engine using BM25.
Asynchronous search is done by thread pool and callback function.
Based on a crawler from [here](https://github.com/lucasjliu/ReutersCrawler), multi-thread utilities from [here](https://github.com/lucasjliu/web), 
and HTTP server from an project [handy](https://github.com/yedf/handy).

## Key modules for searching

* **SearchBase**: data structure, type
* **SearchImpl**: key algorithm implementation
* **Searcher**: module that connects the others
* **SearchServer**: definition of http response logic