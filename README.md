###  Search

A small website searcher over a subset of Wikipedia pages

> [!NOTE]
> I am focusing on the algorithms used in search, not necessarily the problem of extreme scale, 
so I will assume the site data is small enough for it to live entirely in memory. 
This is an interesting problem however and I may implement this in the future.

### Decisions

#### Link Filtering 
We can only use a subset of wikipedia pages for testing, so pages will have outgoing links to 
pages that may not be part of the processed data set. 

For page ranking, we should not count these. Instead, we first mark all pages of interest
ahead of time and only afterwards parse all links, keeping only those that have been marked

We also will filter all namespaced links like "Talk:page", which is a discussion board

We also do not discriminate case and turn space->"_"


### Packages
- logging [spdlog](https://github.com/gabime/spdlog)
- test framework [gtest](https://google.github.io/googletest/)
- xml parser [pugixml](https://github.com/zeux/pugixml)
- opt handler [cxxopts](https://github.com/jarro2783/cxxopts)
- regex [boost](https://www.boost.org/)

### Running

To build for release
```bash
mkdir build && cd build
cmake .. -DBuild=R
make -j8
```

Ensure the dataset is installed, checkout wikipedia's xml [data](https://dumps.wikimedia.org/)

To run cli
```
./bin/cli [path_to_xml] [num_pages_to_process]
```

### Notes

#### Search

[Google Search overview](https://developers.google.com/search/docs/fundamentals/how-search-works)

[Google Search docs](https://developers.google.com/search/docs)

But mostly from the book "Information Retrieval: Implementing and Evaluating Search Engine"

Building a query for the index
- Identify entities. Eg: Statue of Liberty is all 1 entity. 
- Scrub out meaningless stop words. Eg: Photograph "of" cat
- Expand terms to synonyms. Eg: car dealership += auto dealership

The index yields some large set of relevant and quality results

Ranking the results:
- Uniqueness
- Relative importance
- Location


#### Wikipedia
Subset of Wikipedia [dataset](https://dumps.wikimedia.org/enwiki/20251220/)
> specifically enwiki-20251220-pages-meta-current1.xml-p1p41242.bz2 299.8 MB

Documentation for [dumps](https://en.wikipedia.org/wiki/Wikipedia:Database_download#Database_schema)

Schema [docs](https://www.mediawiki.org/wiki/Help:Export#Export_format)

Example wikipedia [article](https://en.wikipedia.org/wiki/Anarchism)

Wikipedia xml rules
- page url is defined by redirect element if it exists, otherwise by title
- page url \<space\> -> "_"
- internal linkes are of the form \[\[LINK]] with \[\[link | text]] where text is displayed
- links may be of the form \[\[namespace:link]] as well, we will ignore namespaced links
- links may also have \[\[link\#section]] and for book keeping we will ignore section
- files are of form \[\[file:name | text]]

Extra rules we will ignore:
- templates are of form \{\{type : parameters}} <- could be links
- external url is of form \[link]



### Todo
- Handle non ascii chars
- Parse wikipedia data graph
- Basic cli
- Page ranking

### Extra Ideas
- Allow namespaces and cluster them into a group
- Web crawler 
- Web Frontend, RPC backend
