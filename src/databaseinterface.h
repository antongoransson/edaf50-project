#ifndef DATABASE_INTERFACE_H
#define DATABASE_INTERFACE_H
#include <string>
#include <vector>
#include <utility>
#include "article.h"
#include "newsgroup.h"

class DatabaseInterface {
public:
  virtual ~DatabaseInterface() = default;
  virtual std::vector<std::pair<int, std::string>> list_news_groups() const = 0;
  virtual bool create_news_group(const std::string& name) = 0;
  virtual bool delete_news_group(int grpID) = 0;
  virtual std::pair<std::vector<std::pair<int,std::string>>,bool> list_articles(int grpID) const = 0;
  virtual bool create_article(int grpID, const std::string& name, const std::string& author, const std::string& text) = 0;
  virtual int delete_article(int grpID, int artID) = 0;
  virtual std::pair<Article, int> get_article(int grpID, int artID) const = 0;
};
#endif
