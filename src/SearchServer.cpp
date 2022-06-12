#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <unordered_map>
//
#include "SearchServer.h"

std::vector<std::vector<RelativeIndex>> SearchServer::search(
    const std::vector<std::string> &queries_input,
    size_t ResponsesLimit) {
  std::vector<std::vector<RelativeIndex>> result;

  // Query Processing
  for (auto& request: queries_input) {
    std::set<std::string> queryWords;
    // Request buffer for parsing
    std::istringstream ssBuf(request);
    std::unordered_map<size_t, size_t> absoluteRelevance;

    // Parsing word requests
    while (true) {
      std::string buf;

      ssBuf >> buf;

      if (buf.empty()) break;

      queryWords.emplace(buf);
    }

    // Filling in the absolute relevance of words
    for (auto& word: queryWords) {
      auto entry = _index.GetWordCount(word);

      for (auto& docId: entry) {
        if (absoluteRelevance.find(docId.doc_id) != absoluteRelevance.end()) {
          absoluteRelevance[docId.doc_id] += docId.count;
        } else {
          absoluteRelevance[docId.doc_id] = docId.count;
        }
      }
    }

    using pair_type = decltype(absoluteRelevance)::value_type;

    // Finding the maximum number of found words in a document
    auto max = std::max_element(absoluteRelevance.begin(),
                                absoluteRelevance.end(),
                                [] (const pair_type & p1, const pair_type & p2) {
                                  return p1.second < p2.second;
                                }
    );

    std::vector<RelativeIndex> relativeRelevance;

    // Calculating Relative Relevance for Queries
    for (auto& it: absoluteRelevance) {
      relativeRelevance.push_back({
        it.first,
        (static_cast<float>(std::round(static_cast<float>(it.second) / max->second * 1000)) / 1000
        )
      });
    }

    result.push_back(relativeRelevance);
  }

  // Sort by relevance
  for (auto& i: result) {
    std::sort(i.begin(), i.end(), [](RelativeIndex &a, RelativeIndex &b){
      return a.rank > b.rank ? true : a.rank == b.rank ? a.doc_id < b.doc_id : false;;
    });
  }

  // Removing unnecessary requests
  for (auto& it: result) {
    while (it.size() > ResponsesLimit) {
      it.pop_back();
    }
  }

  return result;
}