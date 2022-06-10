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
      std::stringstream doc;

      doc << mDocs[i];

      while (true){
        std::string word;

        doc >> word;

        if(word.empty()) return;

        // Find for Entry by key "word" in a dictionary
        auto entry = std::find_if(freq_dictionary[word].begin(), freq_dictionary[word].end(), [&i](Entry& element){
          return element.doc_id == i;
        });

        // Adding a word or incrementing a counter
        if(entry == freq_dictionary[word].end()){
          mutexRecord.lock();
          freq_dictionary[word].push_back({i, 1});
          mutexRecord.unlock();
        } else{
          mutexRecord.lock();
          ++entry->count;
          mutexRecord.unlock();
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