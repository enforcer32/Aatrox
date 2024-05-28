#pragma once

#include "ATRXEngine/Core/Typedef.h"

namespace ATRX
{
	enum class AllocateType
	{
		Unknown,
		Array,
		AllocateTypeSize
	};

	static const char* AllocateTypeStr[(size_t)AllocateType::AllocateTypeSize] = { "Unknown", "Array" };

	struct AllocationStats
	{
		size_t TotalAllocated = 0;
		size_t TotalFreed = 0;
		size_t Allocations[(size_t)AllocateType::AllocateTypeSize];
	};

	class ATRX_API MemoryManager
	{
	public:
		static bool OnInit();
		static void OnDestroy();
		static const AllocationStats& GetStatistics();
		static void DebugPrintStatistics();

		static void* Allocate(size_t size, AllocateType type);
		static void Free(void* block, size_t size, AllocateType type);
		static void Zero(void* block, size_t size);
		static void Copy(void* dest, const void* src, size_t size);
		static void Set(void* dest, int32_t val, size_t size);

	private:
		static AllocationStats s_Stats;
	};
}
