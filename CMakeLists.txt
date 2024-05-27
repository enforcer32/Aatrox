cmake_minimum_required(VERSION 3.28)

project(Aatrox
	VERSION 1.0.0
	LANGUAGES CXX
)

# CONFIGS
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(AATROX_ENGINE_TARGET_NAME ATRXEngine)
set(AATROX_SANDBOX_TARGET_NAME ATRXSandbox)

# Extensions
include(FetchContent)

# Libraries
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY "https://github.com/gabime/spdlog"
        GIT_TAG "v1.14.1"
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(spdlog)

add_subdirectory(${AATROX_ENGINE_TARGET_NAME})
add_subdirectory(${AATROX_SANDBOX_TARGET_NAME})
