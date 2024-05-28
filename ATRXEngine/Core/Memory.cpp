#include "ATRXEngine/Core/Memory.h"
#include "ATRXEngine/Core/Logger.h"

#include <sstream>

namespace ATRX
{
	AllocationStats MemoryManager::s_Stats;

	bool MemoryManager::OnInit()
	{
		std::memset(&s_Stats, 0, sizeof(AllocationStats));
		return true;
	}
	
	void MemoryManager::OnDestroy()
	{
	}

	const AllocationStats& MemoryManager::GetStatistics()
	{
		return s_Stats;
	}

	void MemoryManager::DebugPrintStatistics()
	{
#ifdef ATRX_ENGINE_DEBUG
		std::ostringstream oss;
		oss << "MemoryManager->Statistics Start:\n";
		oss << "Total Allocated: " << s_Stats.TotalAllocated << "\n";
		oss << "Total Freed: " << s_Stats.TotalAllocated << "\n";
		for (size_t i = 0; i < (size_t)AllocateType::AllocateTypeSize; i++)
			oss << "(" << AllocateTypeStr[i] << ")->Allocated: " << s_Stats.Allocations[i] << " Bytes\n";
		oss << "MemoryManager->Statistics End:\n";
		ATRX_LOG_INFO(oss.str());
#endif // ATRX_ENGINE_DEBUG
	}

	void* MemoryManager::Allocate(size_t size, AllocateType type)
	{
		if (type == AllocateType::Unknown)
			ATRX_LOG_WARN("MemoryManager->Allocating {} bytes Type Unknown!", size);

		s_Stats.TotalAllocated += size;
		s_Stats.Allocations[(size_t)type] += size;

		void* block = new uint8_t[size];
		Zero(block, size);
		return block;
	}

	void MemoryManager::Free(void* block, size_t size, AllocateType type)
	{
		if (type == AllocateType::Unknown)
			ATRX_LOG_WARN("MemoryManager->Freeing {} bytes Type Unknown!", size);

		s_Stats.TotalAllocated -= size;
		s_Stats.Allocations[(size_t)type] -= size;

		delete[] block;
	}

	void MemoryManager::Zero(void* block, size_t size)
	{
		std::memset(block, 0, size);
	}

	void MemoryManager::Copy(void* dest, const void* src, size_t size)
	{
		std::memcpy(dest, src, size);
	}

	void MemoryManager::Set(void* dest, int32_t val, size_t size)
	{
		std::memset(dest, val, size);
	}
}
