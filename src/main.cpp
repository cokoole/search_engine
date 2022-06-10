#include <iostream>
//
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main() {

  try {
    // Creating a ConverterJSON object with a constructor with path parameters to JSON files
    ConverterJSON converter_json("../config.json", "../requests.json", "../answers.json");

    // Creating a InvertedIndex obj
    InvertedIndex inverted_index;

    /**
     * ** Database update with
     * Get.TextDocument() @return filepath from JSON config
    */
    inverted_index.UpdateDocumentBase(converter_json.GetTextDocuments());

    // Creating a SearchServer passing to constructor obj inverted_index
    SearchServer search_server(inverted_index);

    /** @output in JSON response at the specified path
     * ** Database update with
     * Get.TextDocument() @return filepath from JSON config
    */
    converter_json.putAnswers(
        search_server.search(
          converter_json.GetRequests(),
          converter_json.GetResponseLimit()
          )
        );

  } catch (std::invalid_argument &x) { // Exception Handling
    std::cout << x.what() << std::endl;
  }

  return 0;
}
