#include "VM/Execution/VM.hpp"
#include "VM/ByteCode/ByteCode.hpp"
#include "VM/ByteCode/IRCodification.hpp"
#include "VM/ByteCode/IRDefinition.hpp"
#include "VM/ByteCode/IRBuilder.hpp"
#include <iostream>
#include <cassert>

int main(int argc, char **argv){

  using namespace VM;
  using namespace IRCodification;
  using namespace IRDefinition;
  using namespace IRBuilder;

  //sign codification
  {
  Word w = 20;                            assert(w == Decode(Code(w)));
  w = -20;                                assert(w == Decode(Code(w)));
  w = (1 << kLiteralSignPosition) - 1;    assert(w == Decode(Code(w)));
  w = -((1 << kLiteralSignPosition) - 1); assert(w == Decode(Code(w)));
  //Inputs out of range, thus can't be properly decoded
  w = (1 << (kLiteralSignPosition + 1)) - 1;    assert(w != Decode(Code(w)));
  w = -((1 << (kLiteralSignPosition + 1)) - 1); assert(w != Decode(Code(w)));
  }

  //coding backpatching
  {
  const Reg reg_src = 4;
  const Target target = 1000;
  const Inst i = JumpIfTrue(reg_src, target);
    { //before backpatching
      Reg r;
      Word l;
      SubInst s;
      DecodeClass2(i, r, l, s);
      assert(r == reg_src);
      assert(Decode(l) == target);
    }
  const Target target2 = 3000;
  Inst i2 = i;
  PatchJump(i2, target2);
    { //after backpatching
      Reg r;
      Word l;
      SubInst s;
      DecodeClass2(i2, r, l, s);
      assert(r == reg_src);
      assert(Decode(l) == target2);
    }
  }


  return 0;
}
