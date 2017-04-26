#pragma once
#include <cstddef>
#include <string>

namespace Compiler{
namespace IR{

using AddrOffset  = int;
using Addr        = size_t; //for @ to code
using Reg         = size_t;

//0 can only point to start of main, which is not referenced
const static Addr kAddrUninitialized = 0; 

const static int kNumArgsInRegister = 1;

enum class ArithType{
    kAdd = 0
  , kSub = 1
  , kMul = 2
  , kDiv = 3
};

enum class LogicType{
    kOr = 0
};

enum class CompType{
    kNotEqual = 0
  , kEqual = 1
  , kLessThan = 2
  , kLessEqualThan = 3
};

enum class JumpCondType{
    kTrue = 0
  , kFalse = 1
};


std::string str(const ArithType op) noexcept;
std::string str(const LogicType op) noexcept;
std::string str(const CompType  op) noexcept;


}//end namespace IR
}//end namespace Compiler
