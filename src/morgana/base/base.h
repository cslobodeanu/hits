#ifndef __MORGANA_BASE_H__
#define __MORGANA_BASE_H__

#include "compatibility/compatibility.h"
#include "reflection/reflection.h"
#include "memory/mememory.h"
#include "types/metypes.h"
#include "io/paths.h"
#include "meparsers.h"
#include "devices/devices.h"
#include "views/meview.h"
#ifdef OS_WINDOWS
#include "views/mevieww32.h"
#endif
#include "clock/meclock.h"
#include "stats/framestats.h"
#include "utils/meutils.h"

using namespace MorganaEngine;
using namespace MorganaEngine::Base::Views;
using namespace MorganaEngine::Base::Math;
using namespace MorganaEngine::Base::Reflection;
using namespace MorganaEngine::Base::Stats;
using namespace MorganaEngine::Base::Utils;
using namespace MorganaEngine::Base::Types;

#endif