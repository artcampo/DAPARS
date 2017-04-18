struct Inst;

struct JumpCond;
struct JumpIncond;
struct LoadI;
struct Load;
struct LoadReg;
struct LoadRegOffs;
struct Store;
struct StoreReg;
struct Arith;
struct Comparison;
struct PtrElem;
struct GetRetVal;
struct SetRetVal;
struct SetPar;
struct Return;
struct ReturnMain;
struct Call;

using PtrInst         = std::unique_ptr<Inst>;
using PtrJumpIncond   = std::unique_ptr<JumpIncond>;
using PtrJumpCond     = std::unique_ptr<JumpCond>;
using PtrLoadI        = std::unique_ptr<LoadI>;
using PtrLoad         = std::unique_ptr<Load>;
using PtrLoadReg      = std::unique_ptr<LoadReg>;
using PtrLoadRegOffs  = std::unique_ptr<LoadRegOffs>;
using PtrStore        = std::unique_ptr<Store>;
using PtrStoreReg     = std::unique_ptr<StoreReg>;
using PtrArith        = std::unique_ptr<Arith>;
using PtrComparison   = std::unique_ptr<Comparison>;
using PtrPtrElem      = std::unique_ptr<PtrElem>;

using PtrGetRetVal    = std::unique_ptr<GetRetVal>;
using PtrSetRetVal    = std::unique_ptr<SetRetVal>;
using PtrSetPar       = std::unique_ptr<SetPar>;
using PtrReturn       = std::unique_ptr<Return>;
using PtrReturnMain   = std::unique_ptr<ReturnMain>;
using PtrCall         = std::unique_ptr<Call>;