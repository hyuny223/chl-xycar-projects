cmake_minimum_required(VERSION 3.5)
project(sliding_window)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#   add_compile_options(-Wall -Wextra -Wpedantic)
# endif()

# find dependencies
# find_package(ament_cmake REQUIRED)
# find_package(rclcpp REQUIRED)
# find_package(std_msgs REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

# include_directories(include)
add_subdirectory(module)
add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${OpenCV_INCLUDE_DIR}
        ${Eigen3_INCLUDE_DIR}
        module/include
        )

target_link_libraries(${PROJECT_NAME} PUBLIC
        ${OpenCV_LIBRARIES}
        Eigen3::Eigen
        module
        )


# ament_target_dependencies(${PROJECT_NAME}
# 	"rclcpp"
# 	"std_msgs"
# 	"OpenCV"
#   "Eigen3"
# )

# install(TARGETS ${PROJECT_NAME}
# 		DESTINATION lib/${PROJECT_NAME})

# if(BUILD_TESTING)
#   find_package(ament_lint_auto REQUIRED)
#   # the following line skips the linter which checks for copyrights
#   # uncomment the line when a copyright and license is not present in all source files
#   #set(ament_cmake_copyright_FOUND TRUE)
#   # the following line skips cpplint (only works in a git repo)
#   # uncomment the line when this package is not in a git repo
#   #set(ament_cmake_cpplint_FOUND TRUE)
#   ament_lint_auto_find_test_dependencies()
# endif()

# ament_package()
