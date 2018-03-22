#include <string>
#include <vector>
#include <utility>
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
  Map<int,NewsGroup> news_groups;
  Map<int,Map<int,Article>> articles;
  int ngCount = 0;
  int artCount = 0;
}
