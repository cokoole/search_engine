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
  std::vector<std::vector<RelativeIndex>> answer;

  for (auto& request: queries_input) {
    std::set<std::string> queryWords;
    std::stringstream ssBuf(request);
    std::unordered_map<size_t, size_t> absoluteRelevance;

    while (true) {
      std::string buf;

      ssBuf >> buf;

      if (buf.empty()) break;

      queryWords.emplace(buf);
    }

    for (auto& word: queryWords) {
      auto occurrences = _index.GetWordCount(word);

      for (auto& docId: occurrences) {
        if (absoluteRelevance.find(docId.doc_id) != absoluteRelevance.end()) {
          absoluteRelevance[docId.doc_id] += docId.count;
        } else {
          absoluteRelevance[docId.doc_id] = docId.count;
        }
      }
    }

    using pair_type = decltype(absoluteRelevance)::value_type;

    auto max = std::max_element(absoluteRelevance.begin(),
                                absoluteRelevance.end(),
                                [] (const pair_type & p1, const pair_type & p2) {
                                  return p1.second < p2.second;
                                }
    );

    std::vector<RelativeIndex> relativeRelevance;

    for (auto& it: absoluteRelevance) {
      relativeRelevance.push_back({
        it.first,
        (static_cast<float>(std::round(static_cast<float>(it.second) / max->second * 1000)) / 1000
        )
      });
    }

    answer.push_back(relativeRelevance);
  }

  for (auto& i: answer) {
    std::sort(i.begin(), i.end(), [](RelativeIndex &a, RelativeIndex &b){
      return a.rank > b.rank;
    });
  }

  for (auto& it: answer) {
    while (it.size() > ResponsesLimit) {
      it.pop_back();
    }
  }

  return answer;
}