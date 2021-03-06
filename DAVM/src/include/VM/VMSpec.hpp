#pragma once

namespace VM{
namespace Spec{

//Memory
const static int kPageNumBits         = 12;   //bits that make addresses within page
const static int kWordNumBits         = 2;    //bits that address a unit word
const static int kPageSize            = (1 << kPageNumBits);  //in bytes
const static int kWordSize            = 4;    //in bytes
const static int kWordBits            = kWordSize*8;

const static int kMaxAddress          = (uint64_t(1) << Spec::kWordBits) - 1;

const static int kCodeMemUnit         = 1;  //unit for indexing code memory (one 32 bits word)

//Registers
const static int kNumberRegisters     = 16;
const static int kStackRegister       = 15;

namespace CompilerMemory{
const static int kTestBoolAddress     = kMaxAddress + 1 - kWordSize;
}//end namespace CompilerMemory

}//end namespace Spec
}//end namespace VM


