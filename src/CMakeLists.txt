
message("configure SRC ${PROJECT_NAME}")

set(SOURCE_FILES main.cpp tcpping.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(
        ${PROJECT_NAME}
        PRIVATE
        ${PROJECT_BINARY_DIR}/generated
)