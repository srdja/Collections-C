cmake_minimum_required(VERSION 3.5)

project(collectc VERSION 0.0.1)

file(GLOB source_files "*.c")
file(GLOB header_files "include/*.h")

add_library(${PROJECT_NAME} SHARED ${source_files})
add_library(${PROJECT_NAME}_static STATIC ${source_files})
include_directories("./include")

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${header_files}")
set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME})

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
  CACHE INTERNAL "${PROJECT_NAME}: Include directories" FORCE)

include(FindPkgConfig QUIET)
if(PKG_CONFIG_FOUND)
  configure_file("collectionc.pc.in" "collectionc.pc" @ONLY)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/collectionc.pc"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig")
endif()

install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_static
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
  )
