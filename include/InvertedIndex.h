#ifndef SEARCH_ENGINE_INCLUDE_INVERTEDINDEX_H_
#define SEARCH_ENGINE_INCLUDE_INVERTEDINDEX_H_

#include <unordered_map>

struct Entry {
  size_t doc_id, count;
// This operator is required for test scenarios
  bool operator ==(const Entry& other) const {
    return (doc_id == other.doc_id &&
        count == other.count);
  }
};

class InvertedIndex {
 public:
  InvertedIndex() = default;
/**
 * Update or fill in the database of documents on which we will search
 * @param input_docs document content
*/
  void UpdateDocumentBase(const std::vector<std::string>& input_docs);
/**
 * ** The method determines the number of occurrences of the word word in
 * the loaded database documents
 * @param word the word whose occurrence frequency is to be determined
 * @return returns a prepared list with word frequency
*/
  std::vector<Entry> GetWordCount(const std::string& word);
 private:
  std::vector<std::string> mDocs;
  std::unordered_map<std::string, std::vector<Entry>> freq_dictionary;
};

#endif //SEARCH_ENGINE_INCLUDE_INVERTEDINDEX_H_
