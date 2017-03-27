#pragma once

namespace IRDefinition{


enum InstClasses{
    InstClassNoReg        = 0
  , InstClassRegLit       = 1
  , InstClassRegLitSub    = 2
  , InstClassRegRegRegSub = 3
  , NumBits = 2
};

enum IRClass0{
    IR_CID_NOP = 0
  , IR_CID_STOP = 1
  , IR_CID_NOT_IMPL = 2
  , IR_CID_JMP = 3
  , IR_CID_NEW_VAR = 4
};

enum IRClass1{
    IR_CID_LOAD = 0
  , IR_CID_CALL = 1
};

enum IRClass2{
    IR_CID_JMPC = 0
  , IR_CID_ARII = 1
};

enum IRClass3{
    IR_CID_ARI = 0
  , IR_CID_CMP = 1
};
//enum class

const static int kClassNumBits    = 2;
  const static int kClass0InstTypeNumBits    = 3;
  const static int kClass1InstTypeNumBits    = 2;
  const static int kClass2InstTypeNumBits    = 1;
  const static int kClass3InstTypeNumBits    = 1;
  const static int kClass0OpcodeNumBits = kClassNumBits + kClass0InstTypeNumBits;
  const static int kClass1OpcodeNumBits = kClassNumBits + kClass1InstTypeNumBits;
  const static int kClass2OpcodeNumBits = kClassNumBits + kClass2InstTypeNumBits;
  const static int kClass3OpcodeNumBits = kClassNumBits + kClass3InstTypeNumBits;
const static int kRegisterNumBits = 4;
const static int kLiteralNumBits  = 16;
const static int kSubtypeNumBits  = 2;

const static int kClassBitMask =     (1 << kClassNumBits) - 1;
  const static int kClass0BitMask =  (1 << kClass0InstTypeNumBits) - 1;
  const static int kClass1BitMask =  (1 << kClass1InstTypeNumBits) - 1;
  const static int kClass2BitMask =  (1 << kClass2InstTypeNumBits) - 1;
  const static int kClass3BitMask =  (1 << kClass3InstTypeNumBits) - 1;
  const static int kClass0OpcodeBitMask =  (1 << kClass0OpcodeNumBits) - 1;
  const static int kClass1OpcodeBitMask =  (1 << kClass1OpcodeNumBits) - 1;
  const static int kClass2OpcodeBitMask =  (1 << kClass2OpcodeNumBits) - 1;
  const static int kClass3OpcodeBitMask =  (1 << kClass3OpcodeNumBits) - 1;
const static int kRegistertMask =    (1 << kRegisterNumBits) - 1;
const static int kLiteraltMask  =    (1 << kLiteralNumBits) - 1;
const static int kSubtypeMask   =    (1 << kSubtypeNumBits) - 1;


// namespace OpCodes{
enum IRCodf {
  //Class 0
  IR_NOP      = InstClassNoReg + (IR_CID_NOP      << kClassNumBits),
  IR_STOP     = InstClassNoReg + (IR_CID_STOP     << kClassNumBits),
  IR_NOT_IMPL = InstClassNoReg + (IR_CID_NOT_IMPL << kClassNumBits),
  IR_JMP      = InstClassNoReg + (IR_CID_JMP      << kClassNumBits),
  IR_NEW_VAR  = InstClassNoReg + (IR_CID_NEW_VAR  << kClassNumBits),

  //Class 1

  IR_LOAD  = InstClassRegLit + (IR_CID_LOAD << kClassNumBits),
  IR_CALL  = InstClassRegLit + (IR_CID_CALL << kClassNumBits),

  //Class 2
  IR_JMPC  = InstClassRegLitSub + (IR_CID_JMPC << kClassNumBits),
  IR_ARII  = InstClassRegLitSub + (IR_CID_ARII << kClassNumBits),

  //Class 3
  IR_ARI   = InstClassRegRegRegSub + (IR_CID_ARI << kClassNumBits),
  IR_CMP   = InstClassRegRegRegSub + (IR_CID_CMP << kClassNumBits),
};
// };

namespace SubtypesArithmetic{
enum IRSubtypesArithmetic{
  IR_ADD = 0,
  IR_SUB = 1,
  IR_MUL = 2,
  IR_DIV = 3
};
};

namespace SubtypesComparison{
enum IRSubtypesComparison{
  IR_NOT = 0,   // !
  IR_EQL = 1,   // ==
  IR_LST = 2,   // <
  IR_LTE = 3,   // <=
};
};

namespace SubtypesJMPC{
enum IRSubtypesComparison{
  IR_FALSE = 0,
  IR_TRUE = 1
};
};

enum IRRegisters {
  IR_REG0  = 0,
  IR_REG1  = 1,
  IR_REG2  = 2,
  IR_REG3  = 3,
  IR_REG4  = 4,
  IR_REG5  = 5,
  IR_REG6  = 6,
  IR_REG7  = 7,
  IR_REG8  = 8,
  IR_REG9  = 9,
  IR_REG10 = 10,
  IR_REG11 = 11,
  IR_REG12 = 12,
  IR_REG13 = 13,
  IR_REG14 = 14,
  IR_REG15 = 15
};


}; //namespace IRDefinition

