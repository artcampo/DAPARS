#pragma once

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


}//end namespace Backend
}//end namespace Compiler
