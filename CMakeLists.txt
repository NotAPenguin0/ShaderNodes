cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(ShaderNodes)
enable_language(CXX)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)

set(SHADER_NODES_COMPILE_FLAGS -Wall -Werror)
set(SHADER_NODES_PREPROCESSOR_DEFINITIONS "")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message(STATUS "ShaderNodes: No build type specified. Using Release by default")
endif(NOT CMAKE_BUILD_TYPE)


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${DEBUG_OUTPUT_DIRECTORY}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${DEBUG_OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${DEBUG_OUTPUT_DIRECTORY}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${RELEASE_OUTPUT_DIRECTORY}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${RELEASE_OUTPUT_DIRECTORY}")
set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${RELEASE_OUTPUT_DIRECTORY}")

if(MSVC)
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W3 /MD /Od /GR /EHsc")
   set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /W3 /GL /Od /Oi /GR /Gy /EHsc")
else()
   if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++ -D_GLIBCXX_DEBUG")
   endif()
endif()

#find_package(OpenGL REQUIRED COMPONENTS OpenGL)

add_subdirectory("src")
add_subdirectory("include")
add_subdirectory("external")

add_executable(ShaderNodes 
    ${SHADER_NODES_SOURCES}
)

target_include_directories(ShaderNodes PUBLIC
    ${SHADER_NODES_INCLUDE_DIRS}
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/external"
)

target_link_libraries(ShaderNodes 
    ${SHADER_NODES_LINK_LIBRARIES}
)

# add compile flags and preprocessor definitions
target_compile_options(ShaderNodes PUBLIC
    ${SHADER_NODES_COMPILE_FLAGS}
)

target_compile_definitions(ShaderNodes PUBLIC
    ${SHADER_NODES_PREPROCESSOR_DEFINITIONS}
)
