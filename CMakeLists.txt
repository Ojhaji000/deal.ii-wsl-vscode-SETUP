cmake_minimum_required (VERSION 3.10) 
find_package(deal.II 9.5.1 REQUIRED)
# or deal.II 9.6.0 REQUIRED
deal_ii_initialize_cached_variables()

project(step1 CXX)

add_executable(step1 step1.cc)
target_compile_features(step1 PRIVATE cxx_std_20)

deal_ii_setup_target(step1)