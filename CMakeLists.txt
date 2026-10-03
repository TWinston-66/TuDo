cmake_minimum_required(VERSION 4.4.3)

project(TuDo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v7.0.3
  )

FetchContent_MakeAvailable(ftxui)
add_executable(TuDo
    src/main.cpp
)
target_link_libraries(TuDo
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)


target_include_directories(TuDo PRIVATE src)

target_compile_features(TuDo PRIVATE cxx_std_26)

set_target_properties(TuDo PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
