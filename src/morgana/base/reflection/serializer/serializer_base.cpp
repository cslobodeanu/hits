#include "serializer_base.h"
#include "../meobject.h"
#include "meio.h"

using namespace MorganaEngine::Base::Reflection::Serializer;

SerializerBase::SerializerBase()
{

}

SerializerBase::~SerializerBase()
{

}

bool SerializerBase::Serialize(const MEObject* obj, Stream* data_out)
{
	return false;
}

MEObject* SerializerBase::Deserialize(Stream* data_in, MEObject* obj /*= NULL*/)
{
	return NULL;
}
