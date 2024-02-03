cmake_minimum_required(VERSION 3.15)

project(collectc VERSION 0.0.1)

file(GLOB source_files "*.c" "sized/*.c" "memory/*.c")
file(GLOB header_files "include/*.h" "include/sized/*.h" "include/memory/*.h")

include_directories("./include")


if(SHARED)
    message("Building a shared library.")
    add_library(${PROJECT_NAME} SHARED ${source_files})
    set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${header_files}")
    target_link_libraries(${PROJECT_NAME})
else()
    message("Building a static library.")
    add_library(${PROJECT_NAME} STATIC ${source_files})
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
    target_link_libraries(${PROJECT_NAME})
endif()


set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
  CACHE INTERNAL "${PROJECT_NAME}: Include directories" FORCE)

include(FindPkgConfig QUIET)
if(PKG_CONFIG_FOUND)
  configure_file("collectionc.pc.in" "collectionc.pc" @ONLY)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/collectionc.pc"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig")
endif()

if(shared)
install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
  )
else()
install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
  )
endif()