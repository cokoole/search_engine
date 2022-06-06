#include <filesystem>
#include <fstream>
#include <vector>
//
#include "ConverterJSON.h"

std::vector<std::string> ConverterJSON::GetTextDocuments() {
  std::vector<std::string> totalFiles = GetVectorString(pathConfig, "files");

  return totalFiles;
}

unsigned int ConverterJSON::GetResponsesLimit() {
  if (!std::filesystem::is_regular_file(pathConfig)) {
    throw "config not";
  }

  unsigned int max_responses = 5;

  nlohmann::json readJSON;
  std::ifstream jsonFile(pathConfig);

  jsonFile >> readJSON;

  if (readJSON["config"].find("max_responses") != readJSON["config"].end())
    max_responses = readJSON["config"].find("max_responses").value().get<unsigned int>();

  return max_responses;
}

std::vector<std::string> ConverterJSON::GetRequests() {
  std::vector<std::string> totalFiles = GetVectorString(pathRequests, "requests");

  return totalFiles;
}

std::vector<std::string> ConverterJSON::GetVectorString(const std::string& path, const std::string& find) {
  if (!std::filesystem::is_regular_file(path)) {
    throw std::invalid_argument("Wrong path: " + path);
  }

  nlohmann::json readJSON;
  std::ifstream jsonFile(path);

  jsonFile >> readJSON;
  readJSON = readJSON[find];

  std::vector<std::string> totalFiles;

  for (auto & it : readJSON) {
    totalFiles.push_back(it);
  }

  return totalFiles;
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
  std::ofstream outFile(pathAnswers);

  if (!outFile.is_open()) {
    throw "file";
  }

  nlohmann::json recordAnswers;
  std::string id = "0000";

  for (auto& i:answers) {
    incrementId(id);
    recordAnswers["answers"]["request" + id]["result"] = "false";
    for (int j = 0; j < i.size(); ++j) {
      recordAnswers["answers"]["request" + id]["result"] = "true";
      recordAnswers["answers"]["request" + id]["relevance"][j] = {
           {"docid", i[j].first},
           {"rank", (static_cast<double>(std::round(i[j].second * 1000))) / 1000}
          };
    }
  }

  outFile << recordAnswers.dump(4) << std::endl;

  outFile.close();
}

void ConverterJSON::incrementId(std::string& idFile) {
  for (unsigned int i = idFile.size() -1; i > 0; --i) {
    if (++idFile[i] <= '9') {
      break;
    }
    idFile[i] = '0';
  }
}
