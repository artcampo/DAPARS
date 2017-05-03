#pragma once
#include "ByteCode.hpp"
#include "Process.hpp"
#include "ErrorLog.hpp"
#include "VMBasicTypes.hpp"

#include <string>
#include <memory>

namespace VM{



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


  void InstLoad(const Reg &reg_dst, uint32_t const &literal);

  //Class 3: Type Ari
  bool InstTypeArihmetic(const Reg &reg_src1, const Reg &reg_src2,
                            const Reg &reg_dst, uint32_t const &sub_type);

  void InstAdd (const Reg &reg_src1, const Reg &reg_src2,
                       const Reg &reg_dst);
  void InstSub (const Reg &reg_src1, const Reg &reg_src2,
                       const Reg &reg_dst);
  void InstMul (const Reg &reg_src1, const Reg &reg_src2,
                       const Reg &reg_dst);
  void InstDiv (const Reg &reg_src1, const Reg &reg_src2,
                       const Reg &reg_dst);

  //Class 3: Type Cmp

  bool InstTypeComparison(const Reg &reg_src1, const Reg &reg_src2,
                            const Reg &reg_dst, uint32_t const &sub_type);

  void InstNot (const Reg &reg_src1, const Reg &reg_src2,
                       const Reg &reg_dst);
  void InstEql (const Reg &reg_src1, const Reg &reg_src2,
                       const Reg &reg_dst);
  void InstLst (const Reg &reg_src1, const Reg &reg_src2,
                       const Reg &reg_dst);
  void InstLte (const Reg &reg_src1, const Reg &reg_src2,
                       const Reg &reg_dst);

  void InstMov (const Reg &reg_src1, const Reg &reg_src2,
                       const Reg &reg_dst);

};

}//end namespace VM
