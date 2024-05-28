file(GLOB_RECURSE ATRXEngineSources
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Platform.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Typedef.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Assertion.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Logger.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Logger.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Window.h"
)

if(WIN32)
	file(GLOB_RECURSE ATRXEnginePlatformSources
		"${CMAKE_CURRENT_SOURCE_DIR}/Platform/Win32/WindowImplWin32.h"
		"${CMAKE_CURRENT_SOURCE_DIR}/Platform/Win32/WindowImplWin32.cpp"
	)
endif(WIN32)

add_definitions(-DATRX_ENGINE_BUILD -DATRX_ENGINE_DEBUG)

set(ATRXEngineSources ${ATRXEngineSources} ${ATRXEnginePlatformSources})
add_library(${AATROX_ENGINE_TARGET_NAME} SHARED ${ATRXEngineSources})

target_include_directories(${AATROX_ENGINE_TARGET_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../")

TARGET_LINK_LIBRARIES(${AATROX_ENGINE_TARGET_NAME} PUBLIC spdlog)
