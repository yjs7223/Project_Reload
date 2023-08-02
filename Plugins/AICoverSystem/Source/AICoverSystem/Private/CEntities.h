//// Copyright, (c) Sami Kangasmaa 2022

#pragma once

#include "CoreMinimal.h"
#include "handle_map_base.h"

/**
* Wrap handle_map for Unreal usage and define Entity Id data type for cover system
**/

typedef griffin2::Id_T FCEntityId;
#define InvalidCEntityId griffin2::Id_T{}

inline uint32 GetTypeHash(const FCEntityId& EntityId)
{
	return GetTypeHash((uint64)EntityId.value);
}

template<typename TDataType>
class TCEntities : public griffin2::handle_map<TDataType>
{
public:

	explicit TCEntities(int32 ReserveNum)
		: griffin2::handle_map<TDataType>(0, (size_t)ReserveNum) {}

	TCEntities()
		: griffin2::handle_map<TDataType>(0, 0) {}

public: // Unreal friendly functions

	inline FCEntityId Insert(TDataType&& Element) { return insert(Element); }
	inline FCEntityId Insert(const TDataType& Element) { return insert(Element); }
	inline bool Remove(FCEntityId EntityId) { const size_t rNum = erase(EntityId); return rNum > 0; }

	inline void Reset() { reset(); }
	inline bool IsValidId(FCEntityId EntityId) const { return isValid(EntityId); }
	inline int32 Num() const { return (int32)size(); }
};

