/// @file 
/// @brief Main application file

#include "pe.h"
#include <stdio.h>

/// Application name string
#define APP_NAME "section-extractor"

/// @brief Print usage test
/// @param[in] f File to print to (e.g., stdout)
void usage(FILE *f) {
    fprintf(f, "Usage: " APP_NAME " <in_file> <section_name> <out_file>\n");
}

/// @brief Application entry point
/// @param[in] argc Number of command-line arguments
/// @param[in] argv Command-line arguments
/// @return 0 in case of success, 1 in case of invalid command-line arguments, 2 in case of error

int main(int argc, char **argv) {
    /// @brief Check if number of command-line arguments is correct
    if (argc != 4) {
        usage(stderr);
        return 1;
    }
    const char *in = argv[1];
    const char *out = argv[3];
    char *name = argv[2];

    if (!exec(in, out, name)) {
        fprintf(stderr, "Error: cannot extract section\n");
        return 2;
    }

    return 0;
}
