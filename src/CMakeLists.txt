add_subdirectory(bitStream)
add_subdirectory(encoder)

add_executable (compress compress.cpp FileUtils.hpp)
target_link_libraries(compress PRIVATE huffman_encoder)

add_executable (uncompress uncompress.cpp FileUtils.hpp)
target_link_libraries(uncompress PRIVATE huffman_encoder)

add_executable (bitconverter bitconverter.cpp FileUtils.hpp)
target_link_libraries(bitconverter PRIVATE bit_input_stream bit_output_stream)
