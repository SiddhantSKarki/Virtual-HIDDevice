cmake_minimum_required(VERSION 4.4)

project(vmouse)


set(CXX_STANDARD 20)

set(CXX_STANDARD_REQUIRED 20)

add_executable(vmouse 
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)

target_include_directories(vmouse PRIVATE 
    ${CMAKE_SOURCE_DIR}/descriptors
)


# target_link_directories() // if not same as 

# target_link_libraries(vmouse PRIVATE
#     "-framework IOKit"
#     "-framework CoreFoundation"    
# )


