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
  ConverterJSON(std::string pathConfig,
                std::string pathRequests,
                std::string pathAnswers);
/**
* File content retrieval method
* @return Returns a list with the contents of the files listed
* in config.json
*/
  std::vector<std::string> GetTextDocuments();
/**
* The method reads the max_responses field to determine the limit
* number of responses per request
*/
  size_t GetResponseLimit() const;
/**
* Method for receiving mRequests from the mRequests.json file
* @return returns a list of mRequests from the mRequests.json file
*/
  std::vector<std::string> GetRequests();
/**
* Put search results in answers.json file
*/
  void putAnswers(std::vector<std::vector<RelativeIndex>>
                  answers);

  void setPathConfig(std::string path);
  void setPathRequests(std::string path);
  void setPathAnswers(std::string path);

 private:
  // File paths
  std::string mPathConfig = "../config.json";
  std::string mPathRequests = "../requests.json";
  std::string mPathAnswers = "../answers.json";

  // Contents JSON
  std::vector<std::string> mTextDocuments;
  size_t mResponseLimit = 0;
  std::vector<std::string> mRequests;

  // Increments the string with the id of the file
  static void incrementId(std::string& idFile);
  // Generic method for extracting desired array from file
  static std::vector<std::string> extractingArrayFromFile(const std::string& path, const std::string& find);
  // Sets response limit from config file
  void setResponseLimit();
};

#endif //SEARCH_ENGINE_INCLUDE_CONVERTERJSON_H_