#include "group.h"
#include <iostream>


Group::Group(std::String namei, int idi):name(namei), id(idi){}

Group::~Group(){}

int Group::get_id(){
  return id;
}

std::String Group::get_name(){
  return name;
}
