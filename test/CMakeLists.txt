set(TARGET_NAME test)

set(SRC
  Main.cpp
)

add_executable(${TARGET_NAME})
target_sources(${TARGET_NAME} PRIVATE ${SRC})
target_link_libraries(${TARGET_NAME} PRIVATE glad glfw glm modernui)

if(WIN32)
  set (DLLS "${CMAKE_SOURCE_DIR}/external/glfw/lib/win/glfw3.dll")
  foreach (DLL ${DLLS})
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLL} "$<TARGET_FILE_DIR:${TARGET_NAME}>")
  endforeach()
endif()