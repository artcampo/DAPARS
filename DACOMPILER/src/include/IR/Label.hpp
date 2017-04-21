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
  Label():id_(-1),name_(""),is_rt_or_lt_(false), is_target_(false)
  , is_arp_(false), is_this_ptr_(false){};
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

  const LabelId Id() const noexcept { return id_;}
  bool  IsRunTime()  const noexcept { return is_rt_or_lt_;}
  bool  IsLinkTime() const noexcept { return not is_rt_or_lt_;}
  bool  IsTarget()  const noexcept  { return is_target_;}
  bool  IsArp()     const noexcept  { return is_arp_;}
  bool  IsThisPtr() const noexcept  { return is_this_ptr_;}
  
  std::string str() const noexcept{
    std::string  s;
    if(IsRunTime()) s = "RT "; else s = "LT ";
    s += name_;
    if(IsTarget()) s+= "[isTarget]";
    if(IsArp()) s+= "[isArp]";
    if(IsThisPtr()) s+= "[isThisPtr]";
    return s;
  };
  

  
  void SetIsTarget()  noexcept  { is_target_ = true;}
  void SetIsArp()     noexcept  { is_arp_ = true;}
  void SetIsThisPtr() noexcept  { is_this_ptr_ = true;}
  

  
protected:
  LabelId     id_;
  std::string name_;
  bool        is_rt_or_lt_;
  bool  is_target_;
  bool  is_arp_;
  bool  is_this_ptr_;  

  Label(const LabelId id, const std::string& name, const bool is_rt_or_lt)
  : id_(id), name_(name), is_rt_or_lt_(is_rt_or_lt), is_target_(false)
  , is_arp_(false), is_this_ptr_(false){};
};




}//end namespace IR
}//end namespace Compiler
