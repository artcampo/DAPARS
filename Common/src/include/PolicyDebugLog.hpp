#pragma once
#include <iostream>
#include <string>

namespace Common{
namespace DebugLog{
  
class DebugLogWriteToCout{
protected:
  void print(const std::string& m) const{
    std::cout << m;
  }
};

class DebugLogNull{
protected:
  void print(const std::string& m) const{}
};


} //end namespace DebugLog
} //end namespace Common

