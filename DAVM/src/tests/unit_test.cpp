#include "VM.hpp"
#include "ByteCode.hpp"
#include "Utils.hpp"
#include "IRCodification.hpp"
#include <iostream>
#include <cassert>

int main(int argc, char **argv){
  
  using namespace VM;
  
  {
  using namespace IRCodification;  
  Word w = 20;
  assert(w == Decode(Code(w)));
  w = -20;
  assert(w == Decode(Code(w)));
  }

  
  return 0;
}
