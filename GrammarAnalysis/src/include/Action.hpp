#pragma once
#include <string>
#include "Identifiers.hpp"

namespace GrammarAnalyzer{
  

  class Action{
  public:    
    enum class kAction{ none, shift, reduce, accept };
    
    kAction action_;
    SetId   next_state_;    
    
    Action(): action_(kAction::none){};
    
    Action( const kAction& action, const SetId& next_state)
      : action_(action), next_state_(next_state){};

    std::string str() const noexcept{
      if(action_ == kAction::shift)  return std::string("shift ") + std::to_string(next_state_);
      if(action_ == kAction::reduce) return std::string("reduce ") + std::to_string(next_state_);
      if(action_ == kAction::accept) return std::string("accept");
      return std::string("");
    }
  };
  

} //end namespace GrammarAnalyzer
