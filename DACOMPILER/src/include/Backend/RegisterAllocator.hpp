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
  , IssueStore callback_store, Backend* backend)
    : max_machine_reg_(max_machine_reg)
    , register_usage_(FirstMachRegFree())
    , callback_load_(callback_load)
    , callback_store_(callback_store)
    , backend_(backend){
    reg_desc_.resize(max_machine_reg_);
    mreg_can_be_flushed_.resize(max_machine_reg_);
  }

  MReg MRegRetValue() const noexcept{ return 0; }
  MReg MRegStackPtr() const noexcept{ return max_machine_reg_ - 1; }
  MReg MRegArpPtr()   const noexcept{ return max_machine_reg_ - 2; }
  MReg MRegThisPtr()  const noexcept{ return max_machine_reg_ - 3; }

  static int FirstMachRegFree() noexcept { return 1; }  //forbid use of mach_r0

  //RegSym for N virtual registers and M variables are mapped:
  // { virt_reg0, virt_reg1, ... virt_regN - 1, var0, .., varM - 1
  bool  RegSymIsVirtualRegister(const RegSym reg_sym) const noexcept{
    return reg_sym < max_ir_registers_;
  }

  //Initialize regAllocator for a function given its max_ir_registers
  //and the max of machine reg
  void Reset(const IR::Reg max_ir_registers, const int function_fixed_usage_machine_reg){
    for(int i = 0; i < max_machine_reg_; ++i){
      reg_desc_[i].clear();
      mreg_can_be_flushed_[i] = true;
    }
    is_in_memory_.clear();
    addr_desc_.clear();
    reg_sym_id_of_mem_addr_.clear();
    mem_addr_of_reg_sym_id_.clear();
    id_free_for_mem_addr_     = max_ir_registers;
    max_ir_registers_         = max_ir_registers;
    function_max_machine_reg_ = max_machine_reg_ - function_fixed_usage_machine_reg;
    register_usage_ = FirstMachRegFree();
  }

  void WriteBack(){
    for(auto& it : is_in_memory_) // for each IrSymbol not updated in memory
      if(not it.second){
//         Dump();
        std::cout << std::endl;
        std::cout << "Flush: Store: " << str(it.first) << "\n";
        FlushIRSymbol(it.first);
        it.second = true;
      }
  }

  //If any value's memory address is outdated, update it; then evit all irsymbols
  void Flush(){
    WriteBack();
    Evict();
//     Dump();
  }

  //Flush value of a given register
  void FlushMReg(const MReg r){
    for(auto& it : reg_desc_[r]) FlushIRSymbol(it);
  }


  //Structure for mappjng an IR register
  RegMap IRReg(RegSym in){
    return RegMap(in);
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
//     Dump();
  }

  void GetRegLoadI(RegMap& mapping){
    GetReg(mapping);
    UsageShared(mapping);
//     Dump();
  }

  void GetRegArith(RegMap& md, RegMap& ms1, RegMap& ms2){
    GetReg(ms1);
    GetReg(ms2);
    GetReg(md);
    UsageShared(ms1);
    UsageShared(ms2);
    UsageNewValue(md);
//     Dump();
  }

  void GetRegStore(RegMap& ms, RegMap& md){
    GetReg(ms);
    UsageBackToMem(ms, md);
//     Dump();
  }

  void GetRegStoreThroughPointer(RegMap& ms){
    GetReg(ms);
    UsageShared(ms);
//     Dump();
  }

  //returns true if ms was not already on a register (thus load needs to happen)
  bool GetRegLoad(RegMap& md, RegMap& ms){
    if(HasMregAssigned(ms)){
      //variable to load is already in a register
      GetReg(ms);
      md.mreg_ = ms.mreg_;
      UsageShared(ms);
      UsageShared(md);
//       Dump();
      return false;
    }
    //variable does need load
    GetReg(md);
//     std::cout << "dest: " << md.mreg_<<"\n";
//     std::cout << "rs: " << md.regsymb_<<"\n";
//     std::cout << "@: " << mem_addr_of_reg_sym_id_.at(ms.regsymb_).str()<<"\n";
    UsageNewValue(md);
    ms.mreg_ = md.mreg_;
    UsageShared(ms);
    //std::invoke(backend_, callback_load_, md.mreg_, mem_addr_of_reg_sym_id_.at() );
    (backend_->*callback_load_)(md.mreg_, mem_addr_of_reg_sym_id_.at(ms.regsymb_));
//     Dump();
    return true;
  }

  void SetRegGetArg(RegMap& md){
    UsageShared(md);
    ReserveNoFlush(md.mreg_);
//     Dump();
  }

  void GetRegGetRetVal(RegMap& md){
    GetReg(md);
    UsageShared(md);
//     Dump();
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
  std::vector<std::set<RegSym>>     reg_desc_;
  std::map<RegSym, bool>            is_in_memory_;
  std::map<RegSym, std::set<MReg>>  addr_desc_;
  std::vector<bool>                 mreg_can_be_flushed_;

  //Identifiers for MemAddr
  std::map<IR::MemAddr, RegSym>       reg_sym_id_of_mem_addr_;
  std::map<RegSym, IR::MemAddr>       mem_addr_of_reg_sym_id_;  //only for debug
  RegSym      id_free_for_mem_addr_;
  RegSym      max_ir_registers_;

  //Callbacks to the backed
  IssueStore  callback_store_;
  IssueLoad   callback_load_;
  Backend*    backend_;

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
    else
       return ForceFreeRegister();
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
  //note that this is only done for GetArg ops which are issued
  //at the very beginning of any function
  void ReserveNoFlush(const MReg& mreg){
    if(mreg != 0) register_usage_++;
    //TODO
    mreg_can_be_flushed_[mreg] = false;
  }

  void FlushIRSymbol(const RegSym s){
    if(not RegSymIsVirtualRegister(s)){
      const MReg        rs  = *addr_desc_.at(s).begin();
      const IR::MemAddr ma  = mem_addr_of_reg_sym_id_.at(s);
      (backend_->*callback_store_)(rs, ma);
    }
  }

  void Evict(int mreg_index){
    if(mreg_can_be_flushed_[mreg_index]){
      bool was_used = false;
      for(auto& irsymbol : reg_desc_[mreg_index]){
        addr_desc_.erase(irsymbol);
        is_in_memory_.erase(irsymbol);
        was_used = true;
      }
      reg_desc_[mreg_index].clear();
      if(was_used) --register_usage_;
    }
  }

  //Remove all IrSymbols (but those reserved for args in machine registers)
  void Evict(){
    for(int mreg_index = 0; mreg_index < function_max_machine_reg_; ++mreg_index)
      Evict(mreg_index);
  }

  MReg ForceFreeRegister(){
    MReg free_reg;
    /*
std::vector<std::set<RegSym>>     reg_desc_;
std::map<RegSym, bool>            is_in_memory_;
std::map<RegSym, std::set<MReg>>  addr_desc_;
std::vector<bool>                 mreg_can_be_flushed_;*
      */
    Dump();
    //free a register
    bool  freed = false;
    int   num_symbols = 1;  //start picking those mreg that only contain 1 symbol
    while(not freed){
      for(int i = 0; i < max_machine_reg_ and not freed; ++i)
        if(   mreg_can_be_flushed_[i]
          and reg_desc_[i].size() == num_symbols
          and RegSymIsVirtualRegister(*reg_desc_[i].begin())){
            std::cout << "flushing " << i <<"\n";
            FlushMReg(i);
            Evict(i);
            free_reg = i;
            freed = true;
          }
      ++num_symbols;
    }

    return free_reg;
  }

public:
void Dump(){
//     return;  //uncomment for dump at each change
//     std::cout << "Usage: " <<register_usage_ << "/" << function_max_machine_reg_<<"\n";
    for(int i = 0; i < max_machine_reg_; ++i){
      if(not reg_desc_[i].empty()){
        std::cout << "---REG_ALLOC- ";
        std::cout << "    r" << i << ": ";
        for(auto& it : reg_desc_[i]) std::cout << str(it)  << " ";
        if(not mreg_can_be_flushed_[i]) std::cout << " [dont flush]";
        std::cout << "\n";
      }
    }

    for(const auto& it : addr_desc_){
      if(not it.second.empty()){
        std::cout << "---REG_ALLOC- ";
        if(IsInMemory(it.first)) std::cout <<"[m] "; else std::cout <<"[r] ";
        std::cout << str(it.first);
        std::cout<< ": ";
        for(const auto& it_rs : it.second) std::cout << "r"<< it_rs << " ";
        std::cout << "\n";
      }
    }
//     std::cout << "\n";
  }



};


}//end namespace Backend
}//end namespace Compiler
