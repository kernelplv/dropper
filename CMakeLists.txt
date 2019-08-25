cmake_minimum_required(VERSION 3.15)
project(dropper)

IF(WIN32)

  IF(MINGW)
      get_filename_component(MINGWBINPATH ${CMAKE_C_COMPILER} PATH)
      set(MINGWLIBPATH ${MINGWBINPATH}/../lib
          CACHE FILEPATH
          DOCSTRING "Path to MinGW import libraries")
      set(MINGWINCLUDEPATH ${MINGWBINPATH}/../include)

    #message(STATUS "!!!${MINGWINCLUDEPATH}")

    set(CMAKE_BUILD_TYPE Debug)
    # default set(CMAKE_BUILD_TYPE Release)

    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

    # Create executable
    file(GLOB 
              CPP_SRC "src/*.cpp")
              
    include_directories("${PROJECT_SOURCE_DIR}/include")
    link_directories("${PROJECT_SOURCE_DIR}/include")

    include_directories("${MINGWINCLUDEPATH}")
    link_directories("${MINGWINCLUDEPATH}")


    add_executable(${PROJECT_NAME} ${CPP_SRC})

    find_library(PocoCrypto PocoCrypto HINTS ${MINGWBINPATH})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${PocoCrypto})
    list( APPEND ExternalSharedLibraries ${PocoCrypto} )
    message(STATUS "${PocoCrypto} found!") 

    find_library(PocoFoundation PocoFoundation HINTS ${MINGWBINPATH})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${PocoFoundation})
    list( APPEND ExternalSharedLibraries ${PocoFoundation} )
    message(STATUS "${PocoFoundation} found!")

    find_library(PocoNet PocoNet HINTS ${MINGWBINPATH})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${PocoNet})
    list( APPEND ExternalSharedLibraries ${PocoNet} )
    message(STATUS "${PocoNet} found!")

    find_library(PocoUtil PocoUtil HINTS ${MINGWBINPATH})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${PocoUtil})
    list( APPEND ExternalSharedLibraries ${PocoUtil} )
    message(STATUS "${PocoUtil} found!")

    find_library(PocoJSON PocoJSON HINTS ${MINGWBINPATH})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${PocoJSON})
    list( APPEND ExternalSharedLibraries ${PocoJSON} )
    message(STATUS "${PocoJSON} found!")

    find_library(PocoNetSSL PocoNetSSL HINTS ${MINGWBINPATH})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${PocoNetSSL})
    list( APPEND ExternalSharedLibraries ${PocoNetSSL} )
    message(STATUS "${PocoNetSSL} found!")

    # add mingw64/bin libs

    find_library(libcrypto-1_1-x64 libcrypto-1_1-x64 HINTS ${MINGWBINPATH})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${libcrypto-1_1-x64})
    list( APPEND ExternalSharedLibraries ${libcrypto-1_1-x64} )
    message(STATUS "${libcrypto-1_1-x64} found!")

    find_library(libexpat-1 libexpat-1 HINTS ${MINGWBINPATH})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${libexpat-1})
    list( APPEND ExternalSharedLibraries ${libexpat-1} )
    message(STATUS "${libexpat-1} found!")

    find_library(libgcc_s_seh-1 libgcc_s_seh-1 HINTS ${MINGWBINPATH})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${libgcc_s_seh-1})
    list( APPEND ExternalSharedLibraries ${libgcc_s_seh-1} )
    message(STATUS "${libgcc_s_seh-1} found!")

    find_library(libpcre-1 libpcre-1 HINTS ${MINGWBINPATH})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${libpcre-1})
    list( APPEND ExternalSharedLibraries ${libpcre-1} )
    message(STATUS "${libpcre-1} found!")

    find_library(libssl-1_1-x64 libssl-1_1-x64 HINTS ${MINGWBINPATH})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${libssl-1_1-x64})
    list( APPEND ExternalSharedLibraries ${libssl-1_1-x64} )
    message(STATUS "${libssl-1_1-x64} found!")

    find_library(libstdc++-6 libstdc++-6 HINTS ${MINGWBINPATH})
    list( APPEND ExternalSharedLibraries ${libstdc++-6} )
    #target_link_libraries(${PROJECT_NAME} PUBLIC ${libstdc++-6})
    message(STATUS "${libstdc++-6} found!")

    find_library(libwinpthread-1 libwinpthread-1 HINTS ${MINGWBINPATH})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${libwinpthread-1})
    list( APPEND ExternalSharedLibraries ${libwinpthread-1} )
    message(STATUS "${libwinpthread-1} found!")

    find_library(zlib1 zlib1 HINTS ${MINGWBINPATH})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${zlib1})
    list( APPEND ExternalSharedLibraries ${zlib1} )
    message(STATUS "${zlib1} found!")

    # copy external libraries to exe (for debug && windows)
    #IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
    message( STATUS "Copying shared libraries..." )
    file( COPY ${ExternalSharedLibraries} DESTINATION "${PROJECT_BINARY_DIR}" NO_SOURCE_PERMISSIONS )
    #ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)

    install(TARGETS ${PROJECT_NAME} DESTINATION ${MINGWBINPATH})


  ELSE()
    message( FATAL_ERROR "Other compilers are not supported yet." )

  ENDIF(MINGW)

ELSEIF(UNIX)
  #message( FATAL_ERROR "Easy man, easy.." ) 
  set(CMAKE_BUILD_TYPE Debug)
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

  file(GLOB 
            CPP_SRC "src/*.cpp")
            
  include_directories("${PROJECT_SOURCE_DIR}/include")
  link_directories("${PROJECT_SOURCE_DIR}/include")

  add_executable(${PROJECT_NAME} ${CPP_SRC})

  find_package(Poco REQUIRED Foundation Net NetSSL JSON Util Crypto)
  target_link_libraries(${PROJECT_NAME} 
    Poco::Foundation
    Poco::Net
    Poco::NetSSL
    Poco::JSON
    Poco::Util
    Poco::Crypto
  )
  install(TARGETS ${PROJECT_NAME} DESTINATION /bin)
ELSE()
  message( FATAL_ERROR "Your OS are not supported yet." )  
ENDIF(WIN32)

