cmake_minimum_required (VERSION 3.10) 
find_package(deal.II 9.6.0 REQUIRED)
# or deal.II 9.6.0 REQUIRED
deal_ii_initialize_cached_variables()

project(main CXX)

add_executable(main main.cc)
target_compile_features(main PRIVATE cxx_std_20)

deal_ii_setup_target(main)