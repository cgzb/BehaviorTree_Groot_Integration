cmake_minimum_required(VERSION 3.14)
project(BehaviorTree_Groot_Integration)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BTCPP_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BTCPP_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
  behaviortree_cpp_v3
  GIT_REPOSITORY https://github.com/BehaviorTree/BehaviorTree.CPP.git
  GIT_TAG        v3.8
)
FetchContent_MakeAvailable(behaviortree_cpp_v3)

add_executable(bt_groot main.cpp)
# The library name in v3 is usually behavior_tree_cpp or behaviortree_cpp_v3 depending on install
target_link_libraries(bt_groot PRIVATE behaviortree_cpp_v3)
