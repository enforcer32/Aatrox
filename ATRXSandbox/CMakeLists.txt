file(GLOB_RECURSE ATRXSandboxSources
	"${CMAKE_CURRENT_SOURCE_DIR}/*.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cxx"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
	"${CMAKE_CURRENT_SOURCE_DIR}/**/*.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/**/*.cxx"
	"${CMAKE_CURRENT_SOURCE_DIR}/**/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/**/*.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/**/*.inl"
)

add_definitions(-DATRX_DEBUG)

add_executable(${AATROX_SANDBOX_TARGET_NAME} ${ATRXSandboxSources})

target_include_directories(${AATROX_SANDBOX_TARGET_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../")

target_link_libraries(${AATROX_SANDBOX_TARGET_NAME} PRIVATE ${AATROX_ENGINE_TARGET_NAME})

function(CopyLibToTarget LIB_NAME TARGET_NAME)
        ADD_CUSTOM_COMMAND(TARGET ${TARGET_NAME}
                POST_BUILD
                COMMAND
                ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:${LIB_NAME}>
                $<TARGET_FILE_DIR:${TARGET_NAME}>
        )
endfunction()

CopyLibToTarget(${AATROX_ENGINE_TARGET_NAME} ${AATROX_SANDBOX_TARGET_NAME})
