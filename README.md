#### Overview

This is a simple C++11 search engine using BM25. asynchronous search is done by thread pool and callback function. based on a crawler from [here](https://github.com/lucasjliu/ReutersCrawler), multi-thread utilities from [here](https://github.com/lucasjliu/web), and HTTP server from an open source project [handy](https://github.com/yedf/handy).

#### Key modules for searching

* **SearchBase**: data structure, type
* **SearchImpl**: implementaFon of key algorithm
* **Searcher**: module that connects the others
* **SearchServer**: definiFon of hRp response logic