#pragma once
#include <vector>
#include <string>

namespace VM{


struct ErrorLog{

  static ErrorLog& GetInstance(){ static ErrorLog instance; return instance;}
  ErrorLog(ErrorLog const&)       = delete;
  void operator=(ErrorLog const&) = delete;

  bool HasErrors() const noexcept{ return num_errors_ > 0;}

  void Log(const std::string& m){
    std::cout << m << "\n";
    errors_.push_back(m);
    num_errors_++;
  }

  void Dump() const{
    for(auto& it : errors_) std::cout << it << "\n";
  }

private:
  std::vector<std::string> errors_;
  int num_errors_;

  ErrorLog() : num_errors_(0){};

};

}//end namespace VM
