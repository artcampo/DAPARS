#pragma once
#include <memory>
#include <string>
#include <cstddef>

namespace Compiler{
namespace IR{



using LabelId = size_t;
struct Label;
// using PtrLabel = std::unique_ptr<Label>;

struct Label{
  Label():id_(-1),name_(""),is_rt_or_lt_(false){};
  Label& operator= ( const Label &o ) = default;
  Label( const Label &o ) = default;
  ~Label() = default;

  const bool operator<  ( const Label &rhs ) const{ return id_ < rhs.id_; }  
  const bool operator== ( const Label &rhs ) const{ return id_ == rhs.id_; }    

  static Label LabelLT(const LabelId id, std::string name){
    return Label(id, name, false);
  }

  static Label LabelRT(const LabelId id, std::string name){
    return Label(id, name, true);
  }
  
  //Label for an argument that is mapped to a register instead of stack
  static Label LabelArgReg(const LabelId id, std::string name){
  }

  const LabelId Id() const noexcept{ return id_;}
  bool  IsRunTime()  const noexcept{ return is_rt_or_lt_;}
  bool  IsLinkTime() const noexcept{ return not is_rt_or_lt_;}


  std::string str() const noexcept{
    std::string  s;
    if(IsRunTime()) s = "RT "; else s = "LT ";
    s += name_;
    return s;
  };
protected:
  LabelId     id_;
  std::string name_;
  bool        is_rt_or_lt_;

  Label(const LabelId id, const std::string& name, const bool is_rt_or_lt)
  : id_(id), name_(name), is_rt_or_lt_(is_rt_or_lt){};
};




}//end namespace IR
}//end namespace Compiler
