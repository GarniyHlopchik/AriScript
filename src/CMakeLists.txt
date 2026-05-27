add_library(ari_lib STATIC
    error_handler.cpp
    interpreter.cpp
    tokenizer.cpp
)

target_include_directories(ari_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_executable(ari main.cpp)
target_link_libraries(ari PRIVATE ari_lib)