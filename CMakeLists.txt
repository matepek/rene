cmake_minimum_required(VERSION 3.10)

project(rene 
	VERSION 0.1.0
	LANGUAGES C CXX
    )
 
add_library(rene INTERFACE)

target_include_directories(rene
    INTERFACE
        "include"
        "third_party/jasl/jasl.github/include"
    )

target_compile_features(rene
    INTERFACE
        cxx_std_17
        cxx_constexpr
        cxx_noexcept
        cxx_variable_templates
        cxx_variadic_macros
    )

add_executable(rene.test
    "test/rene.test.main.cpp"
    "test/rene.test.error.cpp"
    "test/rene.test.result_with_optional.cpp"
    "test/rene.test.result_with_variant.cpp"
    )

target_include_directories(rene.test
    PRIVATE
        "third_party/Catch2/Catch2.github/single_include"
    )

target_link_libraries(rene.test PRIVATE rene)