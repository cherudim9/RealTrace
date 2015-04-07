#ifndef _BASIC_TEXTURE_H
#define _BASIC_TEXTURE_H

#include "tga_input.h"
#include <vector>
#include <iostream>
#include <string>

class BasicTextureVector{
  
 public:
  
  int GetSize()const{ return texture.size(); }

  TGAFILE* GetTexture(int id){
    if (id<0 || id>=GetSize())
      return NULL;
    return texture.data()+id;
  }

  void Clear(){ texture.clear(); }

  bool AddTexture(char *file_name){
    texture.push_back(TGAFILE());
    bool load_result=LoadTGAFile(file_name, &(texture.back()));
    if (!load_result){
      texture.pop_back();
      std::clog<<"Failed to read texture: "<<std::string(file_name)<<std::endl;
    }else
      std::clog<<"Read texture "<<std::string(file_name)<<std::endl;
    return load_result;
  }

 private:
  std::vector<TGAFILE> texture;

};

#endif
