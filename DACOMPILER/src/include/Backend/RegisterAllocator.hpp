#pragma once
#include <vector>
#include <map>
#include <memory>
#include "IR/MemAddr.hpp"
#include "IR/IRSubtypes.hpp"

namespace Compiler{
namespace Backend{

using MReg    = int;
using RegSym  = int;

//mapping of an IR symbol (variable or abstract register) to a machine register
struct RegMap{
  RegMap(RegSym in) : regsymb_(in){}
  RegMap(RegSym in, MReg out) : regsymb_(in), mreg_(out){}
  
  RegSym  regsymb_;
  MReg    mreg_;
  
};

//This register allocator is a simplification of the one provided in the
//dragon book, page 544. It does not take liveness into account.
class RegisterAllocator{
public:

  RegisterAllocator(const int max_machine_reg)
    : max_machine_reg_(max_machine_reg)
    , register_usage_(0){
    reg_desc_.resize(max_machine_reg_);
  }

  //Initialize regAllocator for a function given its max_ir_registers
  void Reset(const IR::Reg max_ir_registers){
    for(int i = 0; i < max_machine_reg_; ++i) reg_desc_[i].clear();
    is_in_memory_.clear();
    addr_desc_.clear();
    reg_sym_id_of_mem_addr_.clear();
    id_free_for_mem_addr_ = max_ir_registers;
  }  
  
  //Structure for mappjng an IR register
  RegMap IRReg(RegSym in){
    return RegMap(in);
  }  
  
  //Structure for mappjng an IR MemAddr
  RegMap IRMemAddr(const IR::MemAddr addr){
    return RegMap(RegSymId(addr));
  }    
  
  void GetRegLoadI(RegMap& mapping){
    GetReg(mapping);
    UsageShared(mapping);
    Dump();
  }
  
  void GetRegArith(RegMap& md, RegMap& ms1, RegMap& ms2){
    GetReg(ms1);
    GetReg(ms2);
    GetReg(md);
    UsageShared(ms1);
    UsageShared(ms2);
    UsageNewValue(md);
    Dump();
  }  
  
  void GetRegStore(RegMap& ms, RegMap& md){
    GetReg(ms);
    UsageBackToMem(ms, md);
    Dump();
  }
    
  
  void Dump(){
    for(int i = 0; i < max_machine_reg_; ++i){
      if(not reg_desc_[i].empty()){
        std::cout << "r" << i << ": ";
        for(auto& it : reg_desc_[i]) std::cout << it << " ";
        std::cout << "\n";
      }
    }
    
    for(const auto& it : addr_desc_){
      if(not it.second.empty()){
        std::cout << "regsym" << it.first << ": ";
        for(auto& it : it.second) std::cout << it << " ";
        std::cout << "\n";        
      }
    }
  }
  
  
private:
  const int   max_machine_reg_;
  int         register_usage_;
  
  //what has been allocated and where
  std::vector<std::vector<RegSym>>    reg_desc_;
  std::map<RegSym, bool>              is_in_memory_;
  std::map<RegSym, std::vector<MReg>> addr_desc_;
  
  //Identifiers for MemAddr
  std::map<IR::MemAddr, RegSym>       reg_sym_id_of_mem_addr_;
  RegSym      id_free_for_mem_addr_;
  
  RegSym RegSymId(const IR::MemAddr addr){
    auto it = reg_sym_id_of_mem_addr_.find(addr);
    if(it == reg_sym_id_of_mem_addr_.end()){
      reg_sym_id_of_mem_addr_[addr] = id_free_for_mem_addr_;
      return id_free_for_mem_addr_++;
    }
    return it->second;
  }  
  
  //Assign mreg to regsymb
  void GetReg(RegMap& mapping){
    const RegSym& regsymb = mapping.regsymb_;
    if(HasMregAssigned(regsymb))
      mapping.mreg_ = MregAssigned(regsymb);
    else
      mapping.mreg_ = GetFreeReg();
  }    
  
  bool  HasMregAssigned(RegSym regsymb){
    auto it = addr_desc_.find(regsymb);
    if(it == addr_desc_.end()) return false;
    return it->second.size() > 0;
  }
  
  MReg  MregAssigned(RegSym regsymb){
    auto it = addr_desc_.find(regsymb);
    return it->second.front();
  }  
  
  MReg GetFreeReg(){
    if(register_usage_ < max_machine_reg_)
      return register_usage_++;
    else{
      //free a register
    }
  }

  //(regsymb_, mreg_) mreg also contains regsymb, both hold it
  void UsageShared(RegMap& mapping){
    const RegSym& regsymb = mapping.regsymb_;
    const MReg&   mreg    = mapping.mreg_;
    addr_desc_[regsymb].push_back(mreg);
    is_in_memory_[regsymb] = true;
    reg_desc_[mreg].clear();
    reg_desc_[mreg].push_back(regsymb);    
  }
  
  //(regsymb_, mreg_) mreg regsymb have same variable, regsymb
  //has now its copy in memory
  void UsageBackToMem(RegMap& ms, RegMap& md){
    const RegSym& regsymb = md.regsymb_;
    const MReg&   mreg    = ms.mreg_;
    is_in_memory_[regsymb] = true;
    reg_desc_[mreg].push_back(regsymb);
    addr_desc_[regsymb].clear();
    addr_desc_[regsymb].push_back(mreg);    
  }  
  
  //(regsymb_, mreg_) mreg regsymb have same variable, however
  //only mreg has its updated value
  void UsageNewValue(RegMap& mapping){
    const RegSym& regsymb = mapping.regsymb_;
    const MReg&   mreg    = mapping.mreg_;   
    RemoveMregFromOtherAddrDescriptors(mreg);
    reg_desc_[mreg].clear();
    reg_desc_[mreg].push_back(regsymb);   
    addr_desc_[regsymb].clear();
    addr_desc_[regsymb].push_back(mreg);
    is_in_memory_[regsymb] = true;    
  }
  
  void RemoveMregFromOtherAddrDescriptors(const MReg& mreg){
    for(const auto& regsymb : reg_desc_[mreg])
      addr_desc_[regsymb].erase(
        std::remove(  addr_desc_[regsymb].begin()
                    , addr_desc_[regsymb].end()
                    , mreg)
        , addr_desc_[regsymb].end());
  }

};


}//end namespace Backend
}//end namespace Compiler
