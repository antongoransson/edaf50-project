#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <iostream>
using std::string;

class NewsGroup
{
  public:
    NewsGroup(string, int id);
    ~NewsGroup();
    int get_id();
    string get_name();
  private:
    int id;
    string name;
};


#endif
