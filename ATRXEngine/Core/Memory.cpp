#include "ATRXEngine/Core/Memory.h"
#include "ATRXEngine/Core/Logger.h"

#include <sstream>

namespace ATRX
{
	AllocationStats MemoryAllocator::s_Stats;

	bool MemoryAllocator::OnInit()
	{
		ATRX_LOG_INFO("ATRXMemoryAllocator->Initializing...");
		std::memset(&s_Stats, 0, sizeof(AllocationStats));
		ATRX_LOG_INFO("ATRXMemoryAllocator->Initialized!");
		return true;
	}
	
	void MemoryAllocator::OnDestroy()
	{
		ATRX_LOG_INFO("ATRXMemoryAllocator->Destroying...");
		ATRX_LOG_INFO("ATRXMemoryAllocator->Destroyed!");
	}

	const AllocationStats& MemoryAllocator::GetStatistics()
	{
		return s_Stats;
	}

	void MemoryAllocator::DebugPrintStatistics()
	{
#ifdef ATRX_ENGINE_DEBUG
		std::ostringstream oss;
		oss << "ATRXMemoryAllocator->Statistics Start:\n";
		oss << "Total Allocated: " << s_Stats.TotalAllocated << "\n";
		oss << "Total Freed: " << s_Stats.TotalAllocated << "\n";
		for (size_t i = 0; i < (size_t)AllocateType::AllocateTypeSize; i++)
			oss << "(" << AllocateTypeStr[i] << ")->Allocated: " << s_Stats.Allocations[i] << " Bytes\n";
		oss << "ATRXMemoryAllocator->Statistics End:\n";
		ATRX_LOG_INFO(oss.str());
#endif // ATRX_ENGINE_DEBUG
	}

	void* MemoryAllocator::Allocate(size_t size, AllocateType type)
	{
		if (type == AllocateType::Unknown)
			ATRX_LOG_WARN("ATRXMemoryAllocator->Allocating {} bytes Type Unknown!", size);

		s_Stats.TotalAllocated += size;
		s_Stats.Allocations[(size_t)type] += size;

		void* block = new uint8_t[size];
		Zero(block, size);
		return block;
	}

	void MemoryAllocator::Free(void* block, size_t size, AllocateType type)
	{
		if (type == AllocateType::Unknown)
			ATRX_LOG_WARN("ATRXMemoryAllocator->Freeing {} bytes Type Unknown!", size);

		s_Stats.TotalAllocated -= size;
		s_Stats.Allocations[(size_t)type] -= size;

		delete[] block;
	}

	void MemoryAllocator::Zero(void* block, size_t size)
	{
		std::memset(block, 0, size);
	}

	void MemoryAllocator::Copy(void* dest, const void* src, size_t size)
	{
		std::memcpy(dest, src, size);
	}

	void MemoryAllocator::Set(void* dest, int32_t val, size_t size)
	{
		std::memset(dest, val, size);
	}
}
