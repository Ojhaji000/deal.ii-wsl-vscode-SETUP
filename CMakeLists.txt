cmake_minimum_required (VERSION 3.10)
find_package(deal.II 9.6.0 REQUIRED)
# or deal.II 9.6.0 REQUIRED
find_package(PkgConfig REQUIRED)
deal_ii_initialize_cached_variables()

project(main CXX)

# Find packages that support pkg-config correctly
pkg_check_modules(CANTERA REQUIRED IMPORTED_TARGET cantera)
pkg_check_modules(YAML_CPP REQUIRED IMPORTED_TARGET yaml-cpp)
pkg_check_modules(FMT REQUIRED IMPORTED_TARGET fmt)

add_executable(main main.cc)
target_compile_features(main PRIVATE cxx_std_20)

# Plain signature linking both discovered targets and raw static sundials files
target_link_libraries(main 
    PkgConfig::CANTERA 
    PkgConfig::YAML_CPP
    PkgConfig::FMT
    sundials_cvodes
    sundials_nvecserial
)

deal_ii_setup_target(main)