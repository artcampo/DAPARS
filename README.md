# DAPARS
This is the main repository for a basic 3 phases compiler. So far the
backend targets a VM but it will be expanded to produce x86 as well.

The project is composed of:
-DAPARS, suite of different parsers for DACOMPILER
-DACOMPILER, the compiler itself minus parser
-DAVM, virtual machine, 3-address instructions (similar to low level ISA)

Parsers are on different stages of evolution:
-RecDesc, this is the more active and up to date
-LLR, on an early stage but stable
-LALR, temporaly on hold (looks like will soon be dead)

*At some point I was using git submodules but it turned out to be only cute,
not effective.

------------------------------------------------------------------------------
CLONING
After cloning it, init the submodules with:
/DAPARS$ git submodule init
/DAPARS$ git submodule update
Or pass "--recursive" to the clone command
Then set a branch in each submodule
/DAPARS/DACOMPILER$ git checkout master
/DAPARS/DAVM$ git checkout master

Other dependencies in linux are resolved via:
sudo apt-get install cmake
sudo apt-get install bison
sudo apt-get install flex

------------------------------------------------------------------------------
INSTALLATION
After executing "./clean_install.sh" you will find the main tests in the folder
"DAPARS/install/tests/ll1"

After that build is done in "/build" with "make install" (better yet followed
by number of processors, ex: "-j8").

------------------------------------------------------------------------------
WHERE TO START
The grammar is defined at "DACOMPILER/doc/grammar.txt" as well as in the file
"Common/src/lib/grammars/grammar_dalang.cpp" which is used to check the ll1
validity.

The nodes that form the AST are defined in:
"DACOMPILER/src/include/AST/Node.hpp"

A good overview of all the passes being used:
"DACOMPILER/src/include/Passes/PassManager.hpp"

The main file for AST creation and semantic analysis is:
"DACOMPILER/src/include/CompilationUnit.hpp"

The next step is IR generation which is entirely within:
"DACOMPILER/src/include/IR"

The backend for the VM is in:
"DACOMPILER/src/include/Backend/BackendDAVM/BackendDAVM.hpp"

The documentation for all the testing infrastructure is in:
"RecDescent/doc/testing_infrastructure.txt"

------------------------------------------------------------------------------
CURRENT STATE
For instance, this is the output of one subtest of code generation:

Parsing: "int f(int p1){int a; a=p1; return a;} void main(){int a; a=f(2);}"

Scopes of module:
Scope 0: {(3:f) (5:main) }
Scope 1: {(1:p1) (2:a) }
Scope 2: {(4:a) }


AST fully decorated dump:
[FDef] FuncDef: f
-[VDec] int a
-[Asgst]
--[Var] a [int Lvalue Write Value Nmemb]
--[Var] p1 [int Lvalue Read Value Nmemb]
-[Retst] Return (a)
--[Var] a [int Lvalue Read Value Nmemb]
[FDef] FuncDef: main
-[VDec] int a
-[Asgst]
--[Var] a [int Lvalue Write Value Nmemb]
--[Fret] FuncRet: int [int Rvalue]
---[Fcal] FuncCall: f [function: (int)->int]
----[Var] f [function: (int)->int Read Value Nmemb]
----[Lit] 2 [int]


Declaration Table:
1: Symbol: [ in scope 1 p1 of type int]
2: Symbol: [ in scope 1 a of type int]
3: Symbol: [ in scope 0 f of type function: (int)->int]
4: Symbol: [ in scope 2 a of type int]
5: Symbol: [ in scope 0 main of type function: ()->void]

IR dump:
LT entry_main
0: %0 = LoadI(2)
1: SetPar( %0)
2: call [LT entry_f: 0]
3: %1 = GetRetVal()
4: store %1 to [LT MainDS:a 0]
5: ReturnMain
LT entry_f
0: %0 = Load [RT arp_f:p1 -2]
1: store %0 to [RT arp_f:a 0]
2: %2 = Load [RT arp_f:a 0]
3: Return
