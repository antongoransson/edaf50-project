#include <string>
#include <vector>
#include <utility>
#include <map>
#include "article.h"
#include "newsgroup.h"
#include "databaseinterface.h"

using std::string;
using std::vector;
using std::pair;

class Database : public DatabaseInterface{
public:
  Database();
  vector<pair<int,string>> list_news_groups();
  pair<vector<pair<int,string>, bool>> list_articles(int grpID);
  bool create_news_group(string name);
  bool create_article(int grpID, string name, string author, string text);
  bool delete_news_group(int grpID);
  int delete_article(int grpID, int artID);
  pair<Article,bool> get_article(int grpID, int artID);
private:
  map<int,NewsGroup> news_groups;
  map<int,map<int,Article>> articles;
  int ngCount = 0;
  int artCount = 0;
};
