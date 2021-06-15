void compile()
{ 
//gSystem->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include");
gSystem->AddIncludePath("-I. -I$ROOTSYS/include");
  gROOT->LoadMacro("SDD.C++");
}
