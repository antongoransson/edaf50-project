#include "databaseinterface.h"
#include "article.h"
#include "newsgroup.h"
#include <string>
#include <vector>
#include <utility>
#include <map>

class Database: public DatabaseInterface {
public:
  Database();
  std::vector<std::pair<int, std::string>> list_news_groups() override;
  std::pair<std::vector<std::pair<int, std::string>>, bool> list_articles(int grpID) override;
  bool create_news_group(std::string name) override;
  bool create_article(int grpID, std::string name, std::string author, std::string text) override;
  bool delete_news_group(int grpID) override;
  int delete_article(int grpID, int artID) override;
  std::pair<Article, int> get_article(int grpID, int artID) override;
private:
  map<int, NewsGroup> news_groups;
  map<int, map<int, Article>> articles;
  int ngCount;
  int artCount;
};
