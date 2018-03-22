#include "newsgroup.h"
#include <iostream>


NewsGroup::NewsGroup(std::String namei, int idi):name(namei), id(idi){}

NewsGroup::~NewsGroup(){}

int NewsGroup::get_id(){
  return id;
}

std::String NewsGroup::get_name(){
  return name;
}
