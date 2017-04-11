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

class RegisterAllocator{
public:

  RegisterAllocator(const int max_machine_reg)
    : max_machine_reg_(max_machine_reg)
    , register_usage_(0){
    reg_desc_.resize(max_machine_reg_);
  }

  //All the GetReg have the RegSym as input and the MReg as output
  MReg GetReg(RegMap& mapping){
    const RegSym& regsymb = mapping.regsymb_;
    if(HasMregAssigned(regsymb)){
      return MregAssigned(regsymb);
    }
    return GetFreeReg();
  }  
  /*
  void GetRegIn(RegMap& mapping){
    const RegSym& regsymb = mapping.regsymb_;
    MReg free_reg = GetReg(mapping);
    mapping.mreg_ = free_reg;
    
    addr_desc_[regsymb].push_back(free_reg);
    is_in_memory_[regsymb] = true;
  }
  */
  void GetRegOut(RegMap& mapping){
    const RegSym& regsymb = mapping.regsymb_;
    MReg free_reg = GetReg(mapping);
    mapping.mreg_ = free_reg;
    
    addr_desc_[regsymb].push_back(free_reg);
    is_in_memory_[regsymb] = true;
    reg_desc_[free_reg].clear();
    reg_desc_[free_reg].push_back(regsymb);
  }  
    
  void Reset(){};
private:
  const int   max_machine_reg_;
  int         register_usage_;
  
  std::vector<std::vector<RegSym>>      reg_desc_;
  std::map<RegSym, bool>                is_in_memory_;
  std::map<RegSym, std::vector<MReg>>   addr_desc_;
  
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

};


}//end namespace Backend
}//end namespace Compiler
