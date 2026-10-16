#include <TSystem.h>

void fusion_roots() {
    gSystem->Exec("hadd -f total.root *_[tT]*.root");
}