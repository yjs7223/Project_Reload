//// Copyright, (c) Sami Kangasmaa 2022


#include "CoverSystemPublicData.h"
#include "CEntities.h"

FCoverHandle::FCoverHandle()
{
	FCEntityId Id = InvalidCEntityId;
	handle = Id.value;
}

FCoverHandle::FCoverHandle(uint64_t InValue)
{
	handle = InValue;
}

bool FCoverHandle::IsValid() const
{
	FCEntityId Id = InvalidCEntityId;
	return Id.value != handle;
}