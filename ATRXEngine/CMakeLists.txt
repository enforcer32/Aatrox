file(GLOB_RECURSE ATRXEngineSources
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Platform.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Typedef.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Assertion.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Logger.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Logger.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Window.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Timer.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/DeltaTime.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Engine.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Engine.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Memory.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Memory.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Event/EventBus.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Event/EventBus.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Event/EventCallback.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Events/Event.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Events/WindowEvents.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Events/KeyEvents.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Events/MouseEvents.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Input/InputCodes.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Input/Input.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Input/Input.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Input/Keyboard.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Input/Keyboard.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Input/Mouse.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Input/Mouse.cpp"
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
