#include "IR/IRSubtypes.hpp"
#include <string>

namespace Compiler{
namespace IR{

std::string str(const ArithType op) noexcept{
  if(op == ArithType::kAdd) return std::string("+");
  /*
  if(op == 1) return std::string("-");
  if(op == 2) return std::string("*");
  if(op == 3) return std::string("/");
  */
  return std::string("not implemented, code 42");
}

std::string str(const LogicType op) noexcept{
  if(op == LogicType::kOr)  return std::string("or");
  if(op == LogicType::kAnd) return std::string("and");
  return std::string("not implemented, code 42");
}

std::string str(const CompType op) noexcept{
  if(op == CompType::kLessThan) return std::string("<");
  if(op == CompType::kEqualTo)  return std::string("==");
  return std::string("not implemented, code 42");
}

}//end namespace IR
}//end namespace Compiler
