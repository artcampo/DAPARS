#pragma once
#include "IR/Label.hpp"
#include <map>

namespace Compiler{


class LabelManager{
public:
  LabelManager() : free_id_(0){
//     IR::LabelId id = FreeId();
//     name_of_label_[id] = ;
//     labels_.push_back(IR::Label::LabelLT(id, name_of_label_[0]));
    RegisterLabelLT("MainDS");
    RegisterLabelRT("ArgReg");
  }
  
  const IR::Label GetLabelMainLocals() const noexcept{
    return labels_.at(0);
  }
  
  const IR::Label GetLabelArgumentInReg() const noexcept{
    return labels_.at(1);
  }  

  std::string LabelStr(IR::LabelId id) const{
    return name_of_label_.at(id);
  }

  const IR::Label NewFunctionEntryLabel(std::string function_name) noexcept{
    IR::LabelId id = RegisterLabelLT("entry_" + function_name);
    entry_label_of_name_[function_name] = id;
    return labels_.at(id);
  }

  const IR::Label NewFunctionARLabel(std::string function_name) noexcept{
    IR::LabelId id = RegisterLabelRT("arp_" + function_name);
    ar_label_of_name_[function_name] = id;
    return labels_.at(id);
  }

  const IR::Label NewClassThisLabel(std::string class_name) noexcept{
    IR::LabelId id = RegisterLabelRT("this_" + class_name);
    return labels_.at(id);
  }

  const IR::Label GetFunctionEntryLabel(std::string function_name) const{
    return labels_.at(entry_label_of_name_.at(function_name));
  }

  const IR::Label GetFunctionARLabel(std::string function_name) const{
    return labels_.at(ar_label_of_name_.at(function_name));
  }

private:
  std::map<IR::LabelId, std::string> name_of_label_;
  std::vector<IR::Label> labels_;

  std::map<std::string, IR::LabelId> entry_label_of_name_;
  std::map<std::string, IR::LabelId> ar_label_of_name_;

  IR::LabelId   free_id_;
  const IR::LabelId FreeId() noexcept{ return free_id_ ++;}
  
  IR::LabelId RegisterLabelLT(const std::string& name){
    IR::LabelId id = FreeId();
    name_of_label_[id] = name;
    labels_.push_back(IR::Label::LabelLT(id, name_of_label_[id]));    
    return id;
  }
  
  IR::LabelId RegisterLabelRT(const std::string& name){
    IR::LabelId id = FreeId();
    name_of_label_[id] = name;
    labels_.push_back(IR::Label::LabelRT(id, name_of_label_[id]));    
    return id;
  }    
};


}//end namespace Compiler
