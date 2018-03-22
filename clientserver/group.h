#ifndef GROUP_H
#define GROUP_H

#include <iostream>

class Group
{
  public:
    Group(std::String, int id);
    ~Group();
    int get_id();
    std::String get_name();
  private:
    int id;
    std::String name;
};


#endif
