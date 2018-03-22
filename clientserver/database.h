#include "databaseinterface.h"
#include "article.h"
#include "newsgroup.h"
#include <string>
#include <vector>
#include <utility>
#include <map>

using std::string;
using std::vector;
using std::pair;

class Database : public DatabaseInterface{
public:
  Database();
  vector<pair<int,string>> list_news_groups() override;
  pair<vector<pair<int,string>>, bool> list_articles(int grpID) override;
  bool create_news_group(string name) override;
  bool create_article(int grpID, string name, string author, string text) override;
  bool delete_news_group(int grpID) override;
  int delete_article(int grpID, int artID) override;
  pair<Article,bool> get_article(int grpID, int artID) override;
private:
  map<int,NewsGroup> news_groups;
  map<int,map<int,Article>> articles;
  int ngCount = 0;
  int artCount = 0;
};
