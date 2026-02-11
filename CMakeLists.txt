cmake_minimum_required(VERSION 3.14)
project(BehaviorTree_Groot_Integration)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Svg)

# Disable tests and examples for dependencies
set(BTCPP_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BTCPP_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

include(FetchContent)

# Fetch BehaviorTree.CPP v3
FetchContent_Declare(
  behaviortree_cpp_v3
  GIT_REPOSITORY https://github.com/BehaviorTree/BehaviorTree.CPP.git
  GIT_TAG        v3.8
)
FetchContent_MakeAvailable(behaviortree_cpp_v3)

# Fetch Groot (v1)
FetchContent_Declare(
  groot
  GIT_REPOSITORY https://github.com/BehaviorTree/Groot.git
  GIT_TAG        master
  PATCH_COMMAND sed -i "s|add_subdirectory( depend/BehaviorTree.CPP )|# add_subdirectory( depend/BehaviorTree.CPP )|g" CMakeLists.txt
)
FetchContent_MakeAvailable(groot)

add_executable(bt_groot main.cpp custom_window.cpp)
target_link_libraries(bt_groot PRIVATE behaviortree_cpp_v3 behavior_tree_editor Qt5::Widgets)

# Include directories for Groot headers
target_include_directories(bt_groot PRIVATE
    ${groot_SOURCE_DIR}
    ${groot_SOURCE_DIR}/include
    ${groot_SOURCE_DIR}/QtNodeEditor/include
)

# Important definitions for Groot/QtNodeEditor
target_compile_definitions(bt_groot PRIVATE NODE_EDITOR_STATIC)
