#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <iostream>

class NewsGroup
{
  public:
    NewsGroup(std::String, int id);
    ~NewsGroup();
    int get_id();
    std::String get_name();
  private:
    int id;
    std::String name;
};


#endif
