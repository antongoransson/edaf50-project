#ifndef DATABASE_INTERFACE_H
#define DATABASE_INTERFACE_H
#include <string>
#include <vector>
#include <map>
#include <utility>
#include "article.h"
#include "newsgroup.h"

using std::map;
using std::string;
using std::vector;
using std::pair;

class DatabaseInterface {
public:
  virtual ~DatabaseInterface() = default;
  virtual vector<pair<int,string>> list_news_groups();
  virtual pair<vector<pair<int,string>>,bool> list_articles(int grpID);
  virtual bool create_news_group(string name);
  virtual bool create_article(int grpID, string name, string author, string text);
  virtual bool delete_news_group(int grpID);
  virtual int delete_article(int grpID, int artID);
  virtual pair<Article,bool> get_article(int grpID, int artID);
};
#endif
