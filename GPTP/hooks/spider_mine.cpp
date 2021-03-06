#include "spider_mine.h"
#include <SCBW/api.h>
#include <SCBW/enumerations.h>
#include <SCBW/UnitFinder.h>

//V241 for VS2008

//Helper class for findBestSpiderMineTargetHook()
class SpiderMineTargetFinder: public scbw::UnitFinderCallbackMatchInterface {

  const CUnit *spiderMine;

  public:
    //Check if @p unit is a suitable target for the @p spiderMine.
    bool match(const CUnit *target) {

		//Don't attack friendly / allied units
		if (!spiderMine->isTargetEnemy(target))
		  return false;

		//Don't attack invincible units / air units / buildings
		if (target->status & (UnitStatus::Invincible | UnitStatus::InAir | UnitStatus::GroundedBuilding))
		  return false;

		//Don't attack hovering units
		if (units_dat::MovementFlags[target->id] == MovementFlags::HoverUnit)
		  return false;

		return true;

    }

    //Constructor
    SpiderMineTargetFinder(const CUnit *spiderMine) : spiderMine(spiderMine) {}
};

namespace hooks {

//Return the best target for the Spider Mine. If there is no suitable target,
//return NULL instead.
CUnit* findBestSpiderMineTargetHook(const CUnit *spiderMine) {
  //Default StarCraft behavior

  //Don't search for a target if the spider mine is under a Disruption Web
  if (spiderMine->status & UnitStatus::CanNotAttack)
    return NULL;

  s32 range = 32 * spiderMine->getSeekRange();
  
  return scbw::UnitFinder::getNearestTarget(
    spiderMine->getX() - range, spiderMine->getY() - range,
    spiderMine->getX() + range, spiderMine->getY() + range,
    spiderMine, SpiderMineTargetFinder(spiderMine));
}

//Return the initial burrowing delay time (in frames) for the Spider Mine.
u8 getSpiderMineBurrowTimeHook(const CUnit *spiderMine) {
  //Default StarCraft behavior
  return 60;
}

} //hooks
