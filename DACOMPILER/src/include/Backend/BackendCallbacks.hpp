#pragma once
#include "Backend/RegMap.hpp"

namespace Compiler{
namespace Backend{

class Backend;
typedef void (Backend::*IssueLoad)(const MReg r, const IR::MemAddr addr);


}//end namespace Backend
}//end namespace Compiler
