file(GLOB_RECURSE ATRXEngineSources
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Platform.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Typedef.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Assertion.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Logger.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Logger.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Window.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Timer.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Engine.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Engine.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Memory.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Memory.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Event.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Event.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Events/Event.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Events/WindowEvents.h"

)

if(WIN32)
	file(GLOB_RECURSE ATRXEnginePlatformSources
		"${CMAKE_CURRENT_SOURCE_DIR}/Platform/Win32/WindowImplWin32.h"
		"${CMAKE_CURRENT_SOURCE_DIR}/Platform/Win32/WindowImplWin32.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/Platform/Win32/TimerImplWin32.cpp"
	)
endif(WIN32)

add_definitions(-DATRX_ENGINE_BUILD -DATRX_ENGINE_DEBUG)

set(ATRXEngineSources ${ATRXEngineSources} ${ATRXEnginePlatformSources})
add_library(${AATROX_ENGINE_TARGET_NAME} SHARED ${ATRXEngineSources})

target_include_directories(${AATROX_ENGINE_TARGET_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../")

TARGET_LINK_LIBRARIES(${AATROX_ENGINE_TARGET_NAME} PUBLIC spdlog)
