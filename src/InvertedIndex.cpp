#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <algorithm>
#include <mutex>
//
#include "InvertedIndex.h"

auto InvertedIndex::findEntryDictionary(const std::string& word, size_t doc_id) {
  auto entryVector = freq_dictionary.find(word);
  Entry *entry = nullptr;

  if (entryVector != freq_dictionary.end())
    for (Entry& entryFind : entryVector->second) {
      if (entryFind.doc_id == doc_id) {
        entry = &entryFind;
        break;
      }
    }

  return entry;
}

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& inputDocsPath) {
  // Cleaning up old data
  mDocs.clear();
  freq_dictionary.clear();

  // Filling mDocs with new texts
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

  std::mutex mutexRecord;
  std::vector<std::thread> vectorThread;

  // word indexing in multithreading mode
  for(size_t i = 0; i < mDocs.size(); i++){
    vectorThread.emplace_back([this, i, &mutexRecord](){
      std::istringstream doc(mDocs[i]);

      std::string word;
      while (doc >> word){
        auto entry  = findEntryDictionary(word, i);

        if (entry != nullptr) {
          std::lock_guard<std::mutex> guard(mutexRecord);
          ++entry->count;
        } else {
          std::lock_guard<std::mutex> guard(mutexRecord);
          freq_dictionary[word].push_back({i, 1});
        }
      }
    });
  }

  for(auto& thread : vectorThread) thread.join();
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
  if (mDocs.empty()) {
    std::cout << "Docs is empty!";
    return {};
  }

  // find for words in a dictionary
  if (freq_dictionary.find(word) != freq_dictionary.end()) {
    return freq_dictionary.find(word)->second;
  }

  return {};
}