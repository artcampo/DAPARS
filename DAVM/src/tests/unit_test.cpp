#include "VM/Execution/VM.hpp"
#include "VM/ByteCode/ByteCode.hpp"
#include "VM/ByteCode/IRCodification.hpp"
#include "VM/ByteCode/IRDefinition.hpp"
#include <iostream>
#include <cassert>

int main(int argc, char **argv){

  using namespace VM;

  {
  using namespace IRCodification;
  using namespace IRDefinition;
  Word w = 20;                            assert(w == Decode(Code(w)));
  w = -20;                                assert(w == Decode(Code(w)));
  w = (1 << kLiteralSignPosition) - 1;    assert(w == Decode(Code(w)));
  w = -((1 << kLiteralSignPosition) - 1); assert(w == Decode(Code(w)));
  //Inputs out of range, thus can't be properly decoded
  w = (1 << (kLiteralSignPosition + 1)) - 1;    assert(w != Decode(Code(w)));
  w = -((1 << (kLiteralSignPosition + 1)) - 1); assert(w != Decode(Code(w)));
  }


  return 0;
}
