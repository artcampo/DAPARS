#pragma once
#include <vector>
#include <map>
#include <memory>

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

  void GetRegLoadI(RegMap& mapping){
    GetReg(mapping);
    UsageShared(mapping);
  }
  
  void GetRegArith(RegMap& md, RegMap& ms1, RegMap& ms2){
    GetReg(ms1);
    GetReg(ms2);
    GetReg(md);
    UsageShared(ms1);
    UsageShared(ms2);
    UsageNewValue(md);
  }  
  
  void GetRegStore(RegMap& mapping){
    GetReg(mapping);
    UsageBackToMem(mapping);    
  }
    
  void Reset(){};
private:
  const int   max_machine_reg_;
  int         register_usage_;
  
  std::vector<std::vector<RegSym>>      reg_desc_;
  std::map<RegSym, bool>                is_in_memory_;
  std::map<RegSym, std::vector<MReg>>   addr_desc_;
  
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
  void UsageBackToMem(RegMap& mapping){
    const RegSym& regsymb = mapping.regsymb_;
    const MReg&   mreg    = mapping.mreg_;    
    is_in_memory_[regsymb] = true;
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
