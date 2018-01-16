#include "base64.h"

using namespace MorganaEngine::Base::Utils;
using namespace MorganaEngine::Base::Types;

String Base64::base64_chars ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
Array<byte> Base64::tempBuffer;