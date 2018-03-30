#ifndef DATABASE_H
#define DATABASE_H

#include "databaseinterface.h"
#include <string>
#include <vector>
#include <utility>
#include <map>

class Database: public DatabaseInterface {
public:
  Database();
  std::vector<std::pair<int, std::string>> list_news_groups() const override;
  bool create_news_group(const std::string& name) override;
  bool delete_news_group(int grpID) override;
  std::pair<std::vector<std::pair<int, std::string>>, bool> list_articles(int grpID) const override;
  bool create_article(int grpID, const std::string& title, const std::string& author, const std::string& text) override;
  int delete_article(int grpID, int artID) override;
  std::pair<Article, int> get_article(int grpID, int artID) const override;
private:
  std::map<int, NewsGroup> news_groups;
  std::map<int, std::map<int, Article>> articles;
  int ngCount;
  int artCount;
};

#endif
