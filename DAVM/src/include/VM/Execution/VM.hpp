#pragma once
#include "VM/ByteCode/ByteCode.hpp"
#include "VM/Execution/Process.hpp"
#include "VM/Execution/ErrorLog.hpp"
#include "VM/VMBasicTypes.hpp"

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
  void    ColdBoot();
  int     LoadProcess(const std::string &file_name);

  ByteCode const            &byte_code_;
  std::unique_ptr<ErrorLog> error_log_;
  std::unique_ptr<Internal::Process>  process_;
  bool  ip_modified_;

  ByteCode* ReadByteCode(const std::string &file_name);

  //Functions that handle a set of subtypes of instructions
  bool InstTypeArihmetic( const Reg reg_src1, const Reg reg_src2,
                          const Reg reg_dst,  const SubInst sub_type);
  bool InstTypeComparison(const Reg reg_src1, const Reg reg_src2,
                          const Reg reg_dst,  const SubInst sub_type);
  bool InstTypeLogic     (const Reg reg_src1, const Reg reg_src2,
                          const Reg reg_dst,  const SubInst sub_type);

  Word PopWord();
  void PushWord(const Word word);

  //Class 0
  void Return();
  void Call(const Target target);
  void Jump(const Target target);

  //Class 1
  void Load  (const Reg reg_dst, const Word literal);
  void LoadB (const Reg reg_dst, const Reg reg_base, const Word literal);
  void LoadI (const Reg reg_dst, const Word literal);
  void Store (const Reg reg_src, const Word literal);
  void StoreB(const Reg reg_src, const Reg reg_base, const Word literal);
  void Pop   (const Reg reg_dst);
  void Push  (const Reg reg_src);

  //Class 2
  void ArithI(const Reg reg_dst, const Word literal,  const SubInst op);
  void JumpC (const Reg reg_src, const Target target, const SubInst op);

  //Class 3: Type Ari
  void Add (const Reg reg_src1, const Reg reg_src2, const Reg reg_dst);
  void Sub (const Reg reg_src1, const Reg reg_src2, const Reg reg_dst);
  void Mul (const Reg reg_src1, const Reg reg_src2, const Reg reg_dst);
  void Div (const Reg reg_src1, const Reg reg_src2, const Reg reg_dst);
  void Move(const Reg reg_src1, const Reg reg_src2, const Reg reg_dst);

  //Class 3: Type Cmp
  void Not (const Reg reg_src1, const Reg reg_src2, const Reg reg_dst);
  void Eqt (const Reg reg_src1, const Reg reg_src2, const Reg reg_dst);
  void Lst (const Reg reg_src1, const Reg reg_src2, const Reg reg_dst);
  void Lte (const Reg reg_src1, const Reg reg_src2, const Reg reg_dst);

  //Class 3: Type Logic
  void Or  (const Reg reg_src1, const Reg reg_src2, const Reg reg_dst);
  void And (const Reg reg_src1, const Reg reg_src2, const Reg reg_dst);

};

}//end namespace VM
