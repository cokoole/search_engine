#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>
//
#include "SearchServer.h"

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queries_input, size_t ResponsesLimit) {
  std::vector<std::vector<RelativeIndex>> answer;

  for (auto& i: queries_input) {
    std::set<std::string> queries;
    std::stringstream ssBuf(i);
    std::map<size_t, size_t> absoluteRelevance;

    while (!ssBuf.eof()) {
      std::string buf;
      ssBuf >> buf;
      queries.emplace(buf);
    }

    for (auto& word: queries) {
      auto occurrences = _index.GetWordCount(word);

      for (auto& docId: occurrences) {
        if (absoluteRelevance.find(docId.doc_id) != absoluteRelevance.end()) {
          absoluteRelevance[docId.doc_id] += docId.count;
        } else {
          absoluteRelevance[docId.doc_id] = docId.count;
        }
      }
    }

    std::vector<RelativeIndex> relativeRelevance;

    auto maxElement = [](std::map<size_t, size_t> map) {
      size_t max = 0;
      for (auto& elem: map) {
        if (elem.second > max) {
          max = elem.second;
        }
      }

      return max;
    };
    auto max = maxElement(absoluteRelevance);
    relativeRelevance.reserve(absoluteRelevance.size());

    for (auto& it: absoluteRelevance) {
      relativeRelevance.push_back({it.first, (static_cast<float>(std::round(static_cast<float>(it.second) / max * 1000)) / 1000)});
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