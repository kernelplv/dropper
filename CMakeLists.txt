cmake_minimum_required(VERSION 3.15)
project(dropper)

include(GNUInstallDirs)

if(WIN32)
  if(MINGW)
    get_filename_component(mingw64_share ${CMAKE_ROOT} DIRECTORY)
    get_filename_component(mingw64 ${mingw64_share} DIRECTORY)
    set(mingw64_include ${mingw64}/include)
    set(mingw64_lib ${mingw64}/lib)
    set(mingw64_bin ${mingw64}/bin)
    set(CMAKE_INSTALL_PREFIX ${mingw64} CACHE PATH "Cmake prefix" FORCE)
    set(LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
  else()
    message( FATAL_ERROR "Other compilers are not supported yet." )
  endif(MINGW)

elseif(UNIX)
  set(LINKER_FLAGS "")
else()
  message( FATAL_ERROR "Your OS are not supported yet." )  
endif(WIN32)

# common settings
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
set(CMAKE_EXE_LINKER_FLAGS "${LINKER_FLAGS}")

# Create executable
file(GLOB 
          CPP_SRC "src/*.cpp")
          
include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_SOURCE_DIR}/include")

include_directories(${CMAKE_INSTALL_FULL_LIBDIR})
link_directories(${CMAKE_INSTALL_FULL_LIBDIR})

add_executable(${PROJECT_NAME} ${CPP_SRC})

find_package(Poco REQUIRED Foundation Net NetSSL JSON Util Crypto)
if (Poco_FOUND)
  include_directories(${Poco_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} ${Poco_LIBRARIES}) 
endif(Poco_FOUND)

install(TARGETS ${PROJECT_NAME} DESTINATION ${MINGWBINPATH})