#pragma once
#include <vector>
#include <string>

namespace VM{


struct ErrorLog{

  ErrorLog() : num_errors_(0){};

  bool HasErrors() const noexcept{ return num_errors_ > 0;}

  void Log(const std::string& m){
    errors_.push_back(m);
    num_errors_++;
  }

  void Dump() const{
    for(auto& it : errors_) std::cout << it << "\n";
  }

private:
  std::vector<std::string> errors_;
  int num_errors_;

};

}//end namespace VM
