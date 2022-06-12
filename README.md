# Search engine

This is a search engine which is a console application.

It searches in the files specified via config.json and requests via requests.json,
giving the search result in answers.json in default settings. It is possible to
set your own paths to the config.json and requests.json files, formatted according
to templates:
1) [Template config.json](https://github.com/cokoole/search_engine/blob/main/config.json).
2) [Template requests.json](https://github.com/cokoole/search_engine/blob/main/requests.json).

## Technology stack

### Needful:

* C++17
* CMake 3.21
* MinGW w64 9.0

### Connected
These libraries will be downloaded when building the project, connected in Cmake

* Lib [nlohmann/json](https://github.com/nlohmann/json).
* Lib [GTest](https://github.com/google/googletest).

## Launch of the project

```
git clone https://github.com/cokoole/search_engine
```

### Project Features

Normal operation is guaranteed only when using MinGW and g++ compiler