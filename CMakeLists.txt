cmake_minimum_required(VERSION 2.8.10 FATAL_ERROR)
project(WCTE_skPMT)

if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.4)
    message(STATUS "GCC version >= 4.4 required!")
    return()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_LIBCPP_ENABLE_CXX17_REMOVED_AUTO_PTR -D_LIBCPP_ENABLE_CXX14_REMOVED_FEATURES")

find_package(Geant4 REQUIRED ui_all vis_all)
find_package(ROOT REQUIRED)
find_package(ROOT CONFIG REQUIRED)

if(NOT ROOT_FOUND)
  message(STATUS "ROOT package not found.")
  return()
endif()

include(${ROOT_USE_FILE})
include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIR})

link_directories(${ROOT_LIBRARY_DIRS})

# Manual dictionary generation
set(DICT_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include/WCSimRootOptions.hh)
set(DICT_LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/WCSimRootLinkDef.hh)
set(DICT_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/WCSimRootDict.cxx)
set(DICT_OUTPUT_HEADER ${CMAKE_CURRENT_BINARY_DIR}/WCSimRootDict.h)

add_custom_command(
  OUTPUT ${DICT_OUTPUT} ${DICT_OUTPUT_HEADER}
  COMMAND rootcint -f ${DICT_OUTPUT} -c -p ${DICT_HEADER} ${DICT_LINKDEF}
  DEPENDS ${DICT_HEADER} ${DICT_LINKDEF}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(WCSimRoot SHARED ./src/WCSimEnumerations.cc ./src/WCSimRootOptions.cc ${DICT_OUTPUT})
target_link_libraries(WCSimRoot ${ROOT_LIBRARIES})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

file(GLOB MACRO_FILES
	  "*.mac"
)
file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_executable(skPMT skPMT.cc ${sources} ${headers} ${DICT_OUTPUT})
add_executable(hkPMT hkPMT.cc ${sources} ${headers} ${DICT_OUTPUT})
add_executable(mPMT mPMT.cc ${sources} ${headers} ${DICT_OUTPUT})

target_link_libraries(skPMT ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} Core RIO Hist Tree)
target_link_libraries(mPMT ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} Core RIO Hist Tree)
target_link_libraries(hkPMT ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} Core RIO Hist Tree)



add_custom_target(WCTE_skPMT DEPENDS skPMT)

