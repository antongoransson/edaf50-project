#include "article.h"
#include "newsgroup.h"
#include "databaseinterface.h"

using std::string;
using std::vector;
using std::pair;

class Database : public DatabaseInterface{
public:
  Database();

private:
  map<int,NewsGroup> news_groups;
  map<int,map<int,Article>> articles;
  int ngCount = 0;
  int artCount = 0;
};
