#include "max_energy.h"
#include <hook_tools.h>

extern const u32 Func_GetMaxEnergy; //Defined in CUnit.cpp

namespace {

//Inject with jmpPatch()
void __declspec(naked) getUnitMaxEnergyWrapper() {
  static CUnit *unit;
  static u16 maxEnergy;

  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EAX
  }

  maxEnergy = hooks::getUnitMaxEnergyHook(unit);

  __asm {
    POPAD
    MOVZX EAX, maxEnergy
    RETN
  }
}

} //unnamed namespace

namespace hooks {

void injectUnitMaxEnergyHook() {
  jmpPatch(getUnitMaxEnergyWrapper, Func_GetMaxEnergy);
}

} //hooks
