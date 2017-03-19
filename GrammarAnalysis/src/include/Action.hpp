#pragma once
#include <string>
#include "Identifiers.hpp"
#include <assert.h>

namespace GrammarAnalyzer{


  class Action{
  public:
    enum class kAction{ none, shift, reduce, accept };

    kAction action_;
    StateId next_state_;
    RuleId  rule_id_;

    Action(): action_(kAction::none){};

    Action(const kAction& action, const StateId& next_state, const RuleId& rule_id)
      : action_(action), next_state_(next_state), rule_id_(rule_id)
      {
        assert(action == kAction::reduce);
      };

    Action(const kAction& action, const StateId& next_state)
      : action_(action), next_state_(next_state), rule_id_(0)
      {
        assert(action == kAction::shift or action == kAction::accept);
      };



    std::string str() const noexcept{
      if(action_ == kAction::shift)
        return "s " + std::to_string(next_state_);

      if(action_ == kAction::reduce)
        return  "r " + std::to_string(next_state_);
              //+ std::string(" rule: ") + std::to_string(rule_id_) ;

      if(action_ == kAction::accept) return "acc";
      return " _ ";
    }
  };


} //end namespace GrammarAnalyzer
