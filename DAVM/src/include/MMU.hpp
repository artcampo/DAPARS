#pragma once
#include <map>
#include <stdint.h>
#include "VMBasicTypes.hpp"

namespace VM{
namespace Internal{


class MMU{

protected:
  MMU() :  page_size_in_words_(Spec::kPageSize/Spec::kWordSize){}

  Word *const LogicalToPhysical(const Addr addr){

  }

private:
  struct PageDesc{
    Word* address_;
  };

  const size_t  page_size_in_words_;
  std::map<Addr, PageDesc>  page_mapping_;



};

}//end namespace Internal
}//end namespace VM
