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
  std::unique_ptr<Internal::Process>  process_;
  std::unique_ptr<ErrorLog> error_log_;

  ByteCode* ReadByteCode(const std::string &file_name);

  //Functions that handle a set of subtypes of instructions
  bool InstTypeArihmetic(const Reg &reg_src1, const Reg &reg_src2,
                            const Reg &reg_dst, uint32_t const &sub_type);
  bool InstTypeComparison(const Reg &reg_src1, const Reg &reg_src2,
                            const Reg &reg_dst, uint32_t const &sub_type);

  //Class 1: Mem
  void LoadI(const Reg &reg_dst, uint32_t const &literal);
  void Load(const Reg &reg_dst, uint32_t const &literal);

  //Class 3: Type Ari
  void Add (const Reg &reg_src1, const Reg &reg_src2, const Reg &reg_dst);
  void Sub (const Reg &reg_src1, const Reg &reg_src2, const Reg &reg_dst);
  void Mul (const Reg &reg_src1, const Reg &reg_src2, const Reg &reg_dst);
  void Div (const Reg &reg_src1, const Reg &reg_src2, const Reg &reg_dst);

  //Class 3: Type Cmp
  void Not (const Reg &reg_src1, const Reg &reg_src2,
                       const Reg &reg_dst);
  void Eqt (const Reg &reg_src1, const Reg &reg_src2,
                       const Reg &reg_dst);
  void Lst (const Reg &reg_src1, const Reg &reg_src2,
                       const Reg &reg_dst);
  void Lte (const Reg &reg_src1, const Reg &reg_src2,
                       const Reg &reg_dst);

  void Move (const Reg &reg_src1, const Reg &reg_src2,
                       const Reg &reg_dst);

};

}//end namespace VM
