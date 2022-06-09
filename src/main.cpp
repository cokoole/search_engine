#include <iostream>
//
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main() {
  ConverterJSON converter_json;
  InvertedIndex inverted_index;

  try {
    auto docsPath = converter_json.GetTextDocuments();
    auto requests = converter_json.GetRequests();

    inverted_index.UpdateDocumentBase(docsPath);
    SearchServer search_server(inverted_index);

    converter_json.putAnswers(search_server.search(requests, converter_json.GetResponsesLimit()));
  } catch (std::invalid_argument &x) {
    std::cout << x.what() << std::endl;
  }

  return 0;
}
