set(TARGET_NAME modernui)

set(SRC
  ${INCLUDE_DIR}/${TARGET_NAME}/ModernUI.h

  Context.cpp
  Vertex.cpp
  Window.cpp
)

add_library(${TARGET_NAME} STATIC)
target_sources(${TARGET_NAME} PRIVATE ${SRC})
target_include_directories(${TARGET_NAME} PRIVATE ${INCLUDE_DIR}/${TARGET_NAME} INTERFACE ${INCLUDE_DIR})