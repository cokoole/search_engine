#include <filesystem>
#include <fstream>
#include <utility>
#include <vector>
#include <iostream>
//
#include "ConverterJSON.h"

std::vector<std::string> ConverterJSON::GetTextDocuments() {
  return mTextDocuments;
}

size_t ConverterJSON::GetResponseLimit() const {
  if (mResponseLimit == 0) {
    throw std::invalid_argument("response limit missing");
  }
  return mResponseLimit;
}

std::vector<std::string> ConverterJSON::GetRequests() {
  return mRequests;
}

std::vector<std::string> ConverterJSON::extractingArrayFromFile(const std::string& path, const std::string& find) {
  if (!std::filesystem::is_regular_file(path)) {
    throw std::invalid_argument("Wrong path: " + path);
  }

  nlohmann::json readJSON;
  std::ifstream jsonFile(path);

  jsonFile >> readJSON;

  std::vector<std::string> totalFiles = readJSON[find].get<std::vector<std::string>>();

  return totalFiles;
}

void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>> answers) {
  std::ofstream outFile(mPathAnswers);

  nlohmann::json recordAnswers;
  std::string id = "0000";

  for (auto& i:answers) {
    incrementId(id);
    recordAnswers["answers"]["request" + id]["result"] = "false";
    for (int j = 0; j < i.size(); ++j) {
      recordAnswers["answers"]["request" + id]["result"] = "true";
      recordAnswers["answers"]["request" + id]["relevance"][j] = {
           {"docid", i[j].doc_id},
           {"rank", (static_cast<double>(std::round(i[j].rank * 1000))) / 1000}
          };
    }
  }

  outFile << recordAnswers.dump(2) << std::endl;

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

ConverterJSON::ConverterJSON(std::string pathConfig,
                             std::string pathRequests,
                             std::string pathAnswers) :
    mPathConfig(std::move(pathConfig)),
    mPathRequests(std::move(pathRequests)),
    mPathAnswers(std::move(pathAnswers))
                             {
  mTextDocuments = extractingArrayFromFile(mPathConfig, "files");
  mRequests = extractingArrayFromFile(mPathRequests, "requests");
  setResponseLimit();
}

void ConverterJSON::setResponseLimit() {
  if (!std::filesystem::is_regular_file(mPathConfig)) {
    throw std::invalid_argument("missing config file");
  }

  size_t responsesLimit = 5;

  nlohmann::json readJSON;
  std::ifstream jsonFile(mPathConfig);

  jsonFile >> readJSON;

  if (readJSON["config"].find("max_responses") != readJSON["config"].end() && readJSON["config"].find("max_responses").value() > 0)
    responsesLimit = readJSON["config"].find("max_responses").value().get<size_t>();

  mResponseLimit = responsesLimit;
}

void ConverterJSON::setPathConfig(std::string path) {
  mPathConfig = std::move(path);
  mTextDocuments = extractingArrayFromFile(mPathConfig, "files");
  setResponseLimit();
}

void ConverterJSON::setPathRequests(std::string path) {
  mPathRequests = std::move(path);
  mRequests = extractingArrayFromFile(mPathRequests, "requests");
}

void ConverterJSON::setPathAnswers(std::string path) {
  mPathAnswers = std::move(path);
}