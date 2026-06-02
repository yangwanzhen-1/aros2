cmake_minimum_required(VERSION 3.5)
project(pkg01_helloworld_cpp)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)   # 引入外部依赖包

# 映射源文件与可执行文件
add_executable(helloworld src/helloworld.cpp)
add_executable(helloworld_ywz src/helloworld_ywz.cpp)

target_include_directories(helloworld PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

# 设置目标依赖库
ament_target_dependencies(
  helloworld
  "rclcpp"
)
ament_target_dependencies(
  helloworld_ywz
  "rclcpp"
)

# 定义安装规则
install(TARGETS helloworld
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS helloworld_ywz
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
