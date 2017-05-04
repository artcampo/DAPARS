#pragma once
#include <map>
#include <stdint.h>
#include "VMBasicTypes.hpp"
#include "VMSpec.hpp"
//#include "Policies/MemoryTesting.hpp"

namespace VM{
namespace Internal{

/*
  With the current specs, pages are 4k and the machine word is 4 bytes.
  Thus a 32 bit address is splitted in two parts (31 being MSB):
  1) 31-10: page of the address
  2)  9- 0: offset within the page
*/

template <class TestingPolicy>
class MMU{

protected:
  MMU()
  : page_size_in_words_(Spec::kPageSize/Spec::kWordSize)
  , address_mask_((1 << (Spec::kPageNumBits - Spec::kWordNumBits)) - 1){}

  ~MMU(){
    for(auto& it : page_mapping_) delete it.second.vector_;
  }

  Word *const LogicalToPhysical(const Addr addr){
    Addr page = PageOfAddr(addr);
    auto it = page_mapping_.find(page);
    if(it == page_mapping_.end()){
      PageDesc desc;
      desc.vector_  = new std::vector<Word>(page_size_in_words_);
      desc.address_ = &(*desc.vector_)[0];
      page_mapping_[page] = desc;
      testing_.WritePage(*desc.vector_, page);
//       std::cout << "** Mem " << addr<< " page " << page << " offset: " << (addr & address_mask_)
//         << " physical " << PointerOfAddr(addr, desc)  <<"\n";
      return PointerOfAddr(addr, desc);
    }
//     std::cout << "** Mem " << addr<< " page " << page << " offset: " << (addr & address_mask_)
//       << " physical " << PointerOfAddr(addr, it->second) <<"\n";
    return PointerOfAddr(addr, it->second);
  }

private:
  struct PageDesc{
    Word* address_;               //Typed pointer for faster access
    std::vector<Word>*  vector_;  //vector holding page
  };

  const size_t              page_size_in_words_;
  const size_t              address_mask_;
  std::map<Addr, PageDesc>  page_mapping_;
  TestingPolicy             testing_;

  Addr PageOfAddr(const Addr addr){
    return addr >> (Spec::kPageNumBits - Spec::kWordNumBits);
  }

  Word *const PointerOfAddr(const Addr addr, const PageDesc& desc){
    return desc.address_ + (addr & address_mask_);
  }

};

}//end namespace Internal
}//end namespace VM
