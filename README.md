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

To run cli (assuming in build/)
```
./bin/cli [path_to_xml] [num_pages_to_process]
```

To run test suite (assuming in build/)
```bash
ctest
```


### Todo + Ideas
See [Workboard](https://trello.com/b/f1pLMycT/search) for progress and backlog of 
ideas for features

Private for now
