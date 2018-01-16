#include "mestringhash.h"

using namespace MorganaEngine::Base::Types;

StringHash::StringHash()
{
	mHashValue = 0;
}

StringHash::StringHash(const char* str)
{
	copy(str);
}

void StringHash::OnStringChanged()
{
	mHashValue = 0;
	if (c_str() != NULL)
		for (char* ptr = (char*)c_str(); *ptr; mHashValue += *ptr, ptr++);
}

const int StringHash::GetHashValue() const
{
	return mHashValue;
}

bool StringHash::Equals(const StringHash& s) const
{
	return mHashValue == s.mHashValue && ((String*)this)->Equals((const char*)s.c_str());
}

bool StringHash::Equals(const char* s, const int hash /*= -1*/) const
{
	if (mHashValue == 0 && hash == 0) return true;

	if (hash >= 0 && mHashValue != hash) return false;

	return ((String*)this)->Equals(s);
}

const StringHash& StringHash::operator = (const char* str)
{
	copy(str);
	return *this;
}

StringHash::operator const char* () const
{
	return *((String*)this);
}
