cmake_minimum_required(VERSION 3.10)

project(rene 
	VERSION 0.1.0
	LANGUAGES C CXX
    )
 

add_library(rene
    "src/error.cpp"
    "src/result.cpp"
    )

target_include_directories(rene
    PUBLIC
        "include"
        "${CMAKE_CURRENT_BINARY_DIR}"
        "third_party/jasl/jasl.github/include"
    )

target_compile_features(rene
    PUBLIC
        cxx_std_17
        cxx_constexpr
        cxx_noexcept
        cxx_variable_templates
        cxx_variadic_macros
    )

include(GenerateExportHeader)

generate_export_header(rene
    EXPORT_FILE_NAME "rene_lib_export.hpp"
)
    
target_compile_definitions(rene PRIVATE "rene_EXPORTS")


if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    target_compile_options(rene PUBLIC
        #"-DRENE_OPTIONAL(T)=std::experimental::optional<T>"
        #"-DRENE_OPTINAL_INCLUDE=<experimental/optional>"
        #"-DRENE_OPTIONAL_IN_PLACE_T=std::experimental::in_place_t"
        
        )
endif()


add_executable(rene.test
    "test/rene.test.main.cpp"
    "test/rene.test.error.cpp"
    "test/rene.test.result.cpp"
    "test/rene.test.result_macro.cpp"
    )

target_include_directories(rene.test
    PRIVATE
        "third_party/Catch2/Catch2.github/single_include"
    )

target_link_libraries(rene.test PRIVATE rene)