#pragma once
#include <vector>
#include "VM/VMBasicTypes.hpp"

namespace VM{
namespace Internal{


class TestingPolicy{
public:
  virtual void WritePage(std::vector<Word>& page, const Addr addr) = 0;
};

class NoTestingPolicy : public TestingPolicy{
public:
  virtual void WritePage(std::vector<Word>& page, const Addr addr){}
};

class BasicPatternTestingPolicy : public TestingPolicy{
public:
  virtual void WritePage(std::vector<Word>& page, const Addr addr){
    for(int i = 0; i < (Spec::kPageSize / sizeof(Word)); ++i)
      page[i] = i + addr;
  }
};


}//end namespace Internal
}//end namespace VM
