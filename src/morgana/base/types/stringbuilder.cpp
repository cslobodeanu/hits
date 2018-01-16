#include "streamingbuffers.h"

using namespace MorganaEngine::Base::Types;

Array<FastString> StringBuilder::allocatedStrings;
Array<int> StringBuilder::dirty;

FastString StringBuilder::initString("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");