file(GLOB_RECURSE sources CONFIGURE_DEPENDS
    src/*.c
    src/*.h
    include/*.h
)

add_executable(section-extractor ${sources} src/pe.c include/pe.h)
target_include_directories(section-extractor PRIVATE src include)
