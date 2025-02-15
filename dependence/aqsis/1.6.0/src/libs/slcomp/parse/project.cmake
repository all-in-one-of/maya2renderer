# Generate scanner by invoking flex with a custom command
set(_scanner_cpp_name ${parse_BINARY_DIR}/scanner.cpp)
add_custom_command(
	OUTPUT ${_scanner_cpp_name}
	COMMAND ${AQSIS_FLEX_EXECUTABLE}
		-o${_scanner_cpp_name} ${parse_SOURCE_DIR}/scanner.ll
	MAIN_DEPENDENCY ${parse_SOURCE_DIR}/scanner.ll
	DEPENDS ${parse_BINARY_DIR}/parser.hpp
)

# Generate parser by invoking bison with a custom command
set(_parser_cpp_name ${parse_BINARY_DIR}/parser.cpp)
set(_parser_hpp_name ${parse_BINARY_DIR}/parser.hpp)
add_custom_command(
	OUTPUT ${_parser_cpp_name} ${_parser_hpp_name}
	COMMAND ${AQSIS_BISON_EXECUTABLE}
		-d -o ${_parser_cpp_name} ${parse_SOURCE_DIR}/parser.yy 
	MAIN_DEPENDENCY ${parse_SOURCE_DIR}/parser.yy
)

# Create source list variables
set(parse_srcs
	funcdef.cpp
	libslparse.cpp
	optimise.cpp
	parsenode.cpp
	typecheck.cpp
	vardef.cpp
)
set(parse_srcs ${parse_srcs} ${_scanner_cpp_name} ${_parser_cpp_name})
make_absolute(parse_srcs ${parse_SOURCE_DIR})

# Create header list variables
set(parse_hdrs
	funcdef.h
	parsenode.h
	vardef.h
)
set(parse_hdrs ${parse_hdrs} ${_parser_hpp_name})
make_absolute(parse_hdrs ${parse_SOURCE_DIR})

include_directories(${parse_SOURCE_DIR})
include_directories(${parse_BINARY_DIR})
