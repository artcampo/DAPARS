#pragma once
#include "ByteCode.hpp"
#include "Process.hpp"
#include "ErrorLog.hpp"

#include <string>
#include <memory>

namespace VirtualMachineSpecs{
const static int kNumberRegisters = 10;  
};

class VirtualMachine{

public:    
//   VirtualMachine(std::string const &file_name);
  VirtualMachine(ByteCode const &byte_code);
  
  bool    ExecProcess();
  void    DumpExecutionContext(int const registers_num = 10) const;

private:  
  int     LoadProcess(const std::string &file_name);
  
  ByteCode const            &byte_code_;
  std::unique_ptr<Process>  process_;
  std::unique_ptr<ErrorLog> error_log_;
  
  ByteCode* ReadByteCode(const std::string &file_name);
  
private:
  void InstLoad(uint32_t const &reg_dst, uint32_t const &literal);
  
  //Class 3: Type Ari
  bool InstTypeArihmetic(uint32_t const &reg_src1, uint32_t const &reg_src2, 
                            uint32_t const &reg_dst, uint32_t const &sub_type);
  
  void InstAdd (uint32_t const &reg_src1, uint32_t const &reg_src2, 
                       uint32_t const &reg_dst);
  void InstSub (uint32_t const &reg_src1, uint32_t const &reg_src2, 
                       uint32_t const &reg_dst);
  void InstMul (uint32_t const &reg_src1, uint32_t const &reg_src2, 
                       uint32_t const &reg_dst);
  void InstDiv (uint32_t const &reg_src1, uint32_t const &reg_src2, 
                       uint32_t const &reg_dst);  

  //Class 3: Type Cmp

  bool InstTypeComparison(uint32_t const &reg_src1, uint32_t const &reg_src2, 
                            uint32_t const &reg_dst, uint32_t const &sub_type);
 
  void InstNot (uint32_t const &reg_src1, uint32_t const &reg_src2, 
                       uint32_t const &reg_dst);
  void InstEql (uint32_t const &reg_src1, uint32_t const &reg_src2, 
                       uint32_t const &reg_dst);
  void InstLst (uint32_t const &reg_src1, uint32_t const &reg_src2, 
                       uint32_t const &reg_dst);
  void InstLte (uint32_t const &reg_src1, uint32_t const &reg_src2, 
                       uint32_t const &reg_dst);  
  
  
  
};