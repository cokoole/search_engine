#ifndef SEARCH_ENGINE_INCLUDE_RELATIVEINDEX_H_
#define SEARCH_ENGINE_INCLUDE_RELATIVEINDEX_H_

struct RelativeIndex{
  std::size_t doc_id;
  float rank;
  // This operator is required for test scenarios
  bool operator ==(const RelativeIndex& other) const {
    return (doc_id == other.doc_id && rank == other.rank);
  }
};


#endif //SEARCH_ENGINE_INCLUDE_RELATIVEINDEX_H_
