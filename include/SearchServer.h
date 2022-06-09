#ifndef SEARCH_ENGINE_INCLUDE_SEARCHSERVER_H_
#define SEARCH_ENGINE_INCLUDE_SEARCHSERVER_H_

#include <vector>
#include <string>
#include "InvertedIndex.h"
#include "RelativeIndex.h"

class SearchServer {
 public:
/**
* @param idx a reference to the class is passed to the class constructor
* Inverted_Index
* so that SearchServer can find out the frequency of words found in request
*/
  explicit SearchServer(InvertedIndex& idx) : _index(idx){ };
/**
* Search query processing method
* @param queries_input search queries taken from the file
requests.json
* @return returns a sorted list of relevant responses for
* given requests
*/
  std::vector<std::vector<RelativeIndex>> search(
      const std::vector<std::string>& queries_input,
      size_t ResponsesLimit = 5);
 private:
  InvertedIndex _index;
};

#endif //SEARCH_ENGINE_INCLUDE_SEARCHSERVER_H_
