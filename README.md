###  Search

A small website searcher over a subset of Wikipedia pages

> [!NOTE]
> I am focusing on the algorithms used in search, not necessarily the problem of extreme scale, 
so I will assume the site data is small enough for it to live entirely in memory. 
This is an interesting problem however and I may implement this in the future.

### Notes
Subset of Wikipedia [dataset](https://dumps.wikimedia.org/enwiki/20251220/)
> specifically enwiki-20251220-pages-meta-current1.xml-p1p41242.bz2 299.8 MB

Documentation for [dumps](https://en.wikipedia.org/wiki/Wikipedia:Database_download#Database_schema)

Schema [docs](https://www.mediawiki.org/wiki/Help:Export#Export_format)

### Packages
- logging [spdlog](https://github.com/gabime/spdlog)
- test framework [gtest](https://google.github.io/googletest/)
- xml parser [pugixml](https://github.com/zeux/pugixml)
- opt handler [cxxopts](https://github.com/jarro2783/cxxopts)

### Todo
- Parse wikipedia data graph
- Basic cli
- Web crawler (optional)
- Frontend
- Page ranking
