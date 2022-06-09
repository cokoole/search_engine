#ifndef SEARCH_ENGINE_INCLUDE_CONVERTERJSON_H_
#define SEARCH_ENGINE_INCLUDE_CONVERTERJSON_H_

#include <string>
//
#include "nlohmann/json.hpp"
#include "RelativeIndex.h"

// Class to work with JSON
class ConverterJSON {
 public:
  ConverterJSON() = default;
/**
* File content retrieval method
* @return Returns a list with the contents of the files listed
* in config.json
*/
  std::vector<std::string> GetTextDocuments();
/**
* The method reads the max_responses field to determine the limit
* number of responses per request
* @return
*/
  size_t GetResponsesLimit();
/**
* Method for receiving requests from the requests.json file
* @return returns a list of requests from the requests.json file
*/
  std::vector<std::string> GetRequests();
/**
* Put search results in answers.json file
*/
  void putAnswers(std::vector<std::vector<RelativeIndex>>
                  answers);

 private:
  // File paths
  const std::string pathConfig = "../config.json";
  const std::string pathRequests = "../requests.json";
  const std::string pathAnswers = "../answers.json";

  // Increments the string with the id of the file
  static void incrementId(std::string& idFile);
  // Generic method for extracting desired array from file
  static std::vector<std::string> GetVectorString(const std::string& path, const std::string& find);
};

#endif //SEARCH_ENGINE_INCLUDE_CONVERTERJSON_H_