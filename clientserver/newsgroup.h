#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>

class NewsGroup {
  public:
    NewsGroup(std::string, int id);
    ~NewsGroup();
    int get_id();
    std::string get_name();
  private:
    std::string name;
    int id;
};


#endif
