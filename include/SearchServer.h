#ifndef SEARCH_ENGINE_INCLUDE_SEARCHSERVER_H_
#define SEARCH_ENGINE_INCLUDE_SEARCHSERVER_H_

#include <vector>
#include <string>
#include "InvertedIndex.h"
#include "RelativeIndex.h"

class SearchServer {
 public:
/**
* @param idx в конструктор класса передаётся ссылка на класс
InvertedIndex,
* чтобы SearchServer мог узнать частоту слов встречаемых в
запросе
*/
  explicit SearchServer(InvertedIndex& idx) : _index(idx){ };
/**
* Метод обработки поисковых запросов
* @param queries_input поисковые запросы взятые из файла
requests.json
* @return возвращает отсортированный список релевантных ответов для
заданных запросов
*/
  std::vector<std::vector<RelativeIndex>> search(const
                                                 std::vector<std::string>& queries_input,
                                                 size_t ResponsesLimit = 5);
 private:
  InvertedIndex _index;
};

#endif //SEARCH_ENGINE_INCLUDE_SEARCHSERVER_H_
