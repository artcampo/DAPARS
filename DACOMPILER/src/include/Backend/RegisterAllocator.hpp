#pragma once
#include <vector>
#include <set>
#include <map>
#include <memory>
#include <functional>
#include "IR/MemAddr.hpp"
#include "IR/IRSubtypes.hpp"
#include "Backend/RegMap.hpp"
#include "Backend/BackendCallbacks.hpp"


namespace Compiler{
namespace Backend{

class Backend;

//This register allocator is a simplification of the one provided in the
//dragon book, page 544. It does not take liveness into account.
class RegisterAllocator{
public:

  RegisterAllocator(const int max_machine_reg, IssueLoad callback_load
  , Backend* backend)
    : max_machine_reg_(max_machine_reg)
    , register_usage_(FirstMachRegFree())
    , callback_load_(callback_load)
    , backend_(backend){
    reg_desc_.resize(max_machine_reg_);
  }
  
  MReg MRegRetValue() const noexcept{ return 0; }
  MReg MRegStackPtr() const noexcept{ return max_machine_reg_ - 1; }
  MReg MRegArpPtr()   const noexcept{ return max_machine_reg_ - 2; }
  MReg MRegThisPtr()  const noexcept{ return max_machine_reg_ - 3; }

  static int FirstMachRegFree() noexcept { return 1; }  //forbid use of mach_r0
  
  //Initialize regAllocator for a function given its max_ir_registers
  //and the max of machine reg
  void Reset(const IR::Reg max_ir_registers, const int function_fixed_usage_machine_reg){
    for(int i = 0; i < max_machine_reg_; ++i) reg_desc_[i].clear();
    is_in_memory_.clear();
    addr_desc_.clear();
    reg_sym_id_of_mem_addr_.clear();
    mem_addr_of_reg_sym_id_.clear();
    id_free_for_mem_addr_     = max_ir_registers;
    max_ir_registers_         = max_ir_registers;
    function_max_machine_reg_ = max_machine_reg_ - function_fixed_usage_machine_reg;
    register_usage_ = FirstMachRegFree();
  }  
  
  //Structure for mappjng an IR register
  RegMap IRReg(RegSym in){
    return RegMap(in);
  }  
  
  void Flush(const MReg r){
    
  }
  
  //Actually, only the mreg=0 will be forced
  RegMap ForceReg(const MReg r){
    if(r == 0 and register_usage_ == 0) register_usage_ = 1;
    return RegMap(-1, r);
  }
  
  //Structure for mappjng an IR MemAddr
  RegMap IRMemAddr(const IR::MemAddr addr){
    return RegMap(RegSymId(addr));
  }    
  
  
  void GetRegRead(RegMap& mapping){
    GetReg(mapping);
    UsageShared(mapping);
    Dump();
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
  
  //returns true if ms was not already on a register (thus load needs to happen)
  bool GetRegLoad(RegMap& md, RegMap& ms){
    if(HasMregAssigned(ms)){
      GetReg(ms);
      md.mreg_ = ms.mreg_;
      UsageShared(ms);
      UsageCopy(md);
      Dump();
      return false;
    }
//     
//     
//     
    GetReg(md);
    std::cout << "dest: " << md.mreg_<<"\n";
    std::cout << "rs: " << md.regsymb_<<"\n";
    std::cout << "@: " << mem_addr_of_reg_sym_id_.at(ms.regsymb_).str()<<"\n";
    UsageNewValue(md);
    ms.mreg_ = md.mreg_;
    UsageShared(ms);
    //std::invoke(backend_, callback_load_, md.mreg_, mem_addr_of_reg_sym_id_.at() );
    (backend_->*callback_load_)(md.mreg_, mem_addr_of_reg_sym_id_.at(ms.regsymb_));
    Dump();
    return true;
  }  
  
  void SetRegGetArg(RegMap& md){
    UsageShared(md);
    ReserveNoFlush(md.mreg_);
    Dump();
  }
  
  void GetRegGetRetVal(RegMap& md){
    GetReg(md);
    UsageShared(md);
    Dump();
  }  
  
  void Dump(){
    return;
    for(int i = 0; i < max_machine_reg_; ++i){
      if(not reg_desc_[i].empty()){
        std::cout << "---- ";
        std::cout << "r" << i << ": ";
        for(auto& it : reg_desc_[i]) std::cout << str(it)  << " ";
          
        std::cout << "\n";
      }
    }
    
    for(const auto& it : addr_desc_){
      if(not it.second.empty()){
        std::cout << "----";
        std::cout << str(it.first);
        if(IsInMemory(it.first)) std::cout <<"[m]"; else std::cout <<"[r]";
        std::cout<< ": ";
        for(const auto& it_rs : it.second) std::cout << "r"<< it_rs << " ";
        std::cout << "\n";        
      }
    }
    std::cout << "\n";   
  }
  
  std::string str(const RegSym rs){
    if(rs < max_ir_registers_)  
      return std::string("%") + std::to_string(rs) + " ";
    else              
      return mem_addr_of_reg_sym_id_[rs].GetOffset().Name() + " ";            
  }

private:
  const int   max_machine_reg_;
  int         register_usage_;
  int         function_max_machine_reg_;  //registers reserved for: stack, arp and/or this
  
  //what has been allocated and where
  std::vector<std::set<RegSym>>    reg_desc_;
  std::map<RegSym, bool>           is_in_memory_;
  std::map<RegSym, std::set<MReg>> addr_desc_;
  
  //Identifiers for MemAddr
  std::map<IR::MemAddr, RegSym>       reg_sym_id_of_mem_addr_;
  std::map<RegSym, IR::MemAddr>       mem_addr_of_reg_sym_id_;  //only for debug
  RegSym      id_free_for_mem_addr_;
  RegSym      max_ir_registers_;
  
  //Callbacks to the backed
  IssueLoad callback_load_;
  Backend*  backend_;
  
  RegSym RegSymId(const IR::MemAddr addr){
    auto it = reg_sym_id_of_mem_addr_.find(addr);
    if(it == reg_sym_id_of_mem_addr_.end()){
      reg_sym_id_of_mem_addr_[addr] = id_free_for_mem_addr_;
      mem_addr_of_reg_sym_id_[id_free_for_mem_addr_] = addr;
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
  
  bool  IsInMemory(const RegSym rs) const {
    return is_in_memory_.at(rs);
  }
  
  bool  HasMregAssigned(RegMap& mapping) const {
    return HasMregAssigned(mapping.regsymb_);
  }
  
  bool  HasMregAssigned(RegSym regsymb) const {
    auto it = addr_desc_.find(regsymb);
    if(it == addr_desc_.end()) return false;
    return it->second.size() > 0;
  }
  
  MReg  MregAssigned(RegSym regsymb) const {
    auto it       = addr_desc_.find(regsymb);
    std::set<MReg>::iterator mreg_it  = it->second.begin();
    MReg m        = *mreg_it;
    return m;
  }  
  
  MReg GetFreeReg(){
    if(register_usage_ < function_max_machine_reg_)
      return register_usage_++;
    else{
      //free a register
      std::cout << "Registers full. Shame on you. (function max:"<< function_max_machine_reg_<< ")\n";
      exit(1);
    }
  }

  //(regsymb_, mreg_) mreg also contains regsymb, both hold it
  void UsageShared(RegMap& mapping){
    const RegSym& regsymb = mapping.regsymb_;
    const MReg&   mreg    = mapping.mreg_;
    addr_desc_[regsymb].emplace(mreg);
    reg_desc_[mreg].clear();
    reg_desc_[mreg].emplace(regsymb);    
    is_in_memory_[regsymb] = true;
  }
  
  //(regsymb_, mreg_) mreg regsymb have same variable, regsymb
  //has now its copy in memory
  void UsageBackToMem(RegMap& ms, RegMap& md){
    const RegSym& regsymb = md.regsymb_;
    const MReg&   mreg    = ms.mreg_;
    reg_desc_[mreg].emplace(regsymb);
    addr_desc_[regsymb].clear();
    addr_desc_[regsymb].emplace(mreg);   
    is_in_memory_[regsymb] = true;
  }  
  
  //(regsymb_, mreg_) mreg regsymb have same variable, however
  //only mreg has its updated value
  void UsageNewValue(RegMap& mapping){
    const RegSym& regsymb = mapping.regsymb_;
    const MReg&   mreg    = mapping.mreg_;   
    RemoveMregFromOtherAddrDescriptors(mreg);
    reg_desc_[mreg].clear();
    reg_desc_[mreg].emplace(regsymb);   
    addr_desc_[regsymb].clear();
    addr_desc_[regsymb].emplace(mreg);
    is_in_memory_[regsymb] = true;    
  }
  
  //(regsymb_, mreg_) mreg holds a copy of another mreg
  void UsageCopy(RegMap& mapping){
    const RegSym& regsymb = mapping.regsymb_;
    const MReg&   mreg    = mapping.mreg_;   
    reg_desc_[mreg].emplace(regsymb);   
    addr_desc_[regsymb].clear();
    addr_desc_[regsymb].emplace(mreg);
    is_in_memory_[regsymb] = false;
  }  
  
  
  void RemoveMregFromOtherAddrDescriptors(const MReg& mreg){
    for(const auto& regsymb : reg_desc_[mreg])
      addr_desc_[regsymb].erase(mreg);
  }
  
  //do not allow mreg to be flushed (used for registers that are
  //not available in memory (args in regs)
  void ReserveNoFlush(const MReg& mreg){
    if(mreg != 0) register_usage_++;
    //TODO
  }
  
  
};


}//end namespace Backend
}//end namespace Compiler
