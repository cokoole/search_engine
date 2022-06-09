#include <fstream>
//
#include "gtest/gtest.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include "ConverterJSON.h"
#include "RelativeIndex.h"
#include "nlohmann/json.hpp"

using namespace std;

void TestInvertedIndexFunctionality(
    const vector<string>& path,
    const vector<string>& requests,
    const std::vector<vector<Entry>>& expected
) {
  std::vector<std::vector<Entry>> result;
  InvertedIndex idx;
  idx.UpdateDocumentBase(path);
  for(auto& request : requests) {
    std::vector<Entry> word_count = idx.GetWordCount(request);
    result.push_back(word_count);
  }
  ASSERT_EQ(result, expected);
}

TEST(TestCaseInvertedIndex, TestBasic) {
  const vector<string> path = {
      "../../test/testResources/test001.txt",
      "../../test/testResources/test002.txt"
  };

  const vector<string> requests = {"london", "the"};

  const vector<vector<Entry>> expected = {
      {
          {0, 1}
      }, {
          {0, 1}, {1, 3}
      }};
  TestInvertedIndexFunctionality(path, requests, expected);
}

TEST(TestCaseInvertedIndex, TestBasic2) {
  const vector<string> path = {
      "../../test/testResources/test003.txt",
      "../../test/testResources/test004.txt",
      "../../test/testResources/test005.txt",
      "../../test/testResources/test006.txt"
  };
  const vector<string> requests = {"milk", "water", "cappuccino"};
  const vector<vector<Entry>> expected = {
      {
          {0, 4}, {1, 1}, {2, 5}
      }, {
          {0, 3}, {1, 2}, {2, 5}
      }, {
          {3, 1}
      }
  };
  TestInvertedIndexFunctionality(path, requests, expected);
}

TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
  const vector<string> path = {
      "../../test/testResources/test007.txt",
      "../../test/testResources/test008.txt"
  };
  const vector<string> requests = {"m", "statement"};
  const vector<vector<Entry>> expected = {
      {
      }, {
          {1, 1}
      }
  };
  TestInvertedIndexFunctionality(path, requests, expected);
}

TEST(TestCaseSearchServer, TestSimple) {
  const vector<string> path = {
      "../../test/testResources/test003.txt",
      "../../test/testResources/test004.txt",
      "../../test/testResources/test005.txt",
      "../../test/testResources/test006.txt"
  };
  const vector<string> request = {"milk water", "sugar"};
  const std::vector<vector<RelativeIndex>> expected = {
      {
          {2, 1},
          {0, 0.7},
          {1, 0.3}
      },
      {
      }
  };
  InvertedIndex idx;
  idx.UpdateDocumentBase(path);
  SearchServer srv(idx);
  std::vector<vector<RelativeIndex>> result = srv.search(request);
  ASSERT_EQ(result, expected);
}

TEST(TestCaseSearchServer, TestTop5) {
  const vector<string> docs = {
      "../../test/testResources/test009.txt",
      "../../test/testResources/test010.txt",
      "../../test/testResources/test011.txt",
      "../../test/testResources/test012.txt",
      "../../test/testResources/test013.txt",
      "../../test/testResources/test014.txt",
      "../../test/testResources/test015.txt",
      "../../test/testResources/test016.txt",
      "../../test/testResources/test017.txt",
      "../../test/testResources/test018.txt",
      "../../test/testResources/test019.txt",
      "../../test/testResources/test020.txt",
      "../../test/testResources/test021.txt",
      "../../test/testResources/test022.txt",
      "../../test/testResources/test023.txt",
      "../../test/testResources/test024.txt",
      "../../test/testResources/test025.txt",
      "../../test/testResources/test026.txt",
      "../../test/testResources/test027.txt",
      "../../test/testResources/test028.txt",
      "../../test/testResources/test029.txt",
      "../../test/testResources/test030.txt",
  };

  const vector<string> request = {"moscow is the capital of russia"};
  const std::vector<vector<RelativeIndex>> expected = {
      {
          {7, 1},
          {14, 1},
          {0, 0.667},
          {1, 0.667},
          {2, 0.667},
      }
  };

  InvertedIndex idx;
  idx.UpdateDocumentBase(docs);
  SearchServer srv(idx);
  std::vector<vector<RelativeIndex>> result = srv.search(request, 5);
  ASSERT_EQ(result, expected);
}

TEST(TestCaseConverterJSON, TestConstracParam) {
  size_t expectedResponses = 7;
  std::vector<std::string> expectedDocs = {
      "../resources/file0001.txt",
      "../resources/file0002.txt",
      "../resources/file0003.txt",
      "../resources/file0004.txt",
      "../resources/file0005.txt",
      "../resources/file0006.txt",
      "../resources/file0007.txt",
      "../resources/file0008.txt",
      "../resources/file0009.txt"
  };

  std::vector<std::string> expectedRequests = {
      "a",
      "v",
      "g",
      "q"
  };

  ConverterJSON converterJSON("../../test/testConfig.json", "../../test/testRequests.json", "../../test/testAnswers.json");

  ASSERT_EQ(expectedDocs, converterJSON.GetTextDocuments());
  ASSERT_EQ(expectedRequests, converterJSON.GetRequests());
  ASSERT_EQ(expectedResponses, converterJSON.GetResponseLimit());
}

TEST(TestCaseConverterJSON, TestSetsPath) {
  size_t expectedResponses = 7;
  std::vector<std::string> expectedDocs = {
      "../resources/file0001.txt",
      "../resources/file0002.txt",
      "../resources/file0003.txt",
      "../resources/file0004.txt",
      "../resources/file0005.txt",
      "../resources/file0006.txt",
      "../resources/file0007.txt",
      "../resources/file0008.txt",
      "../resources/file0009.txt"
  };

  std::vector<std::string> expectedRequests = {
      "a",
      "v",
      "g",
      "q"
  };

  ConverterJSON converterJSON;

  converterJSON.setPathConfig("../../test/testConfig.json");
  converterJSON.setPathRequests("../../test/testRequests.json");

  ASSERT_EQ(expectedDocs, converterJSON.GetTextDocuments());
  ASSERT_EQ(expectedRequests, converterJSON.GetRequests());
  ASSERT_EQ(expectedResponses, converterJSON.GetResponseLimit());
}

TEST(TestCaseConverterJSON, TestPutAnswer) {
  ConverterJSON converterJSON;

  const std::vector<vector<RelativeIndex>> answer {
      {
          {2, 1},
          {0, 0.7},
          {1, 0.3}
      },
      {
      }
  };

  nlohmann::json expected = {R"({
    "answers": {
        "request0001": {
            "relevance": [
                {
                    "docid": 2,
                    "rank": 1.0
                },
                {
                    "docid": 0,
                    "rank": 0.7
                },
                {
                    "docid": 1,
                    "rank": 0.3
                }
            ],
            "result": "true"
        },
        "request0002": {
            "result": "false"
        }
      }
    })"_json
  };

  converterJSON.setPathAnswers("../../test/testAnswers.json");
  converterJSON.putAnswers(answer);

  std::ifstream file("../../test/testAnswers.json");

  nlohmann::json result;

  file >> result;

  ASSERT_EQ(expected, result);
}