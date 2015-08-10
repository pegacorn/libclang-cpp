cmake_minimum_required (VERSION 3.1)
project(libclang-cpp CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

set(libclang-cpp_sources
  ${PROJECT_SOURCE_DIR}/src/Index.cpp
  ${PROJECT_SOURCE_DIR}/src/TranslationUnit.cpp
  ${PROJECT_SOURCE_DIR}/src/File.cpp
  ${PROJECT_SOURCE_DIR}/src/Cursor.cpp
  ${PROJECT_SOURCE_DIR}/src/CursorKind.cpp
  )
add_library(clang++			SHARED ${libclang-cpp_sources})
add_library(clang++-static	STATIC ${libclang-cpp_sources})

include_directories("${PROJECT_SOURCE_DIR}/include")

target_compile_definitions(clang++
  PUBLIC	"CLANGXX_DLL"
  )
set_target_properties(clang++
  PROPERTIES 
  DEFINE_SYMBOL	"CLANGXX_EXPORTS"
  )

# http://www.cmake.org/Wiki/CMake_FAQ#How_do_I_make_my_shared_and_static_libraries_have_the_same_root_name.2C_but_different_suffixes.3F
set(static_library_prefix "${CMAKE_STATIC_LIBRARY_PREFIX}")
message("CMAKE_STATIC_LIBRARY_PREFIX = ${static_library_prefix}")
if("${static_library_prefix}" STREQUAL "")
  if(MSVC)
	set(static_library_prefix "lib")
  endif()
endif()
message("static_library_prefix = ${static_library_prefix}")
set_target_properties(clang++-static
  PROPERTIES
  OUTPUT_NAME	"clang++"
  PREFIX		"${static_library_prefix}"
  )

# TODO: プラットフォーム依存したくないので、CMAKE_CXX_FLAGSはCMakeで何とかできないか？
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
elseif(MSVC)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /I<clang-c-include-dir> /L<clang-c-lib-dir>")
endif()

target_link_libraries(clang++ clang)
