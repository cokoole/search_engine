#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
//
#include "InvertedIndex.h"

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& inputDocsPath) {
  mDocs.clear();
  freq_dictionary.clear();
  for (auto& path: inputDocsPath) {
    std::ifstream file(path);
    if (file.is_open()) {
      std::string doc;
      std::getline(file, doc);
      mDocs.push_back(doc);
      file.close();
    } else {
      std::cout << "Path file missing: " << path << std::endl;
    }
  }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
  if (mDocs.empty()) {
    std::cout << "Docs is empty!";
    return {};
  }

  if (freq_dictionary.find(word) != freq_dictionary.end()) {
    return freq_dictionary.find(word)->second;
  }

  std::vector<Entry> result;

  size_t i = 0;
  for (auto& it: mDocs) {
    std::stringstream doc(it);
    std::string buf;
    size_t number = 0;

    while (!doc.eof()) {
      doc >> buf;
      if (buf == word)
        ++number;
    }

    if (number > 0)
      result.push_back({i, number});

    ++i;
  }

  freq_dictionary.emplace(word, result);

  return result;
}