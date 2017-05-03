#pragma once

namespace VM{
namespace Spec{

const static int kNumberRegisters     = 16;
const static int kPageNumBits         = 12;   //bits that make addresses within page
const static int kWordNumBits         = 2;    //bits that address a unit word
const static int kPageSize            = (1 << kPageNumBits);  //in bytes
const static int kWordSize            = 4;    //in bytes

}//end namespace VM
} //end namespace VM


