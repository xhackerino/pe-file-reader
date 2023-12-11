//
// Created by rakin on 5/27/2023.
//

#ifndef SECTION_EXTRACTOR_PE_H
#define SECTION_EXTRACTOR_PE_H

#endif //SECTION_EXTRACTOR_PE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define MAIN_OFFSET 0x3c

/// @brief Structure containing PE header data
struct __attribute__((packed)) PE_header {
    uint16_t Machine;
    uint16_t NumberOfSections;
    uint32_t TimeDateStamp;
    uint32_t PointerToSymbolTable;
    uint32_t NumberOfSymbols;
    uint16_t SizeOfOptionalHeader;
    uint16_t Characteristics;
};

/// @brief Structure containing section header data

struct __attribute__((packed)) Section_header {
    char name[8];
    uint32_t VirtualSize;
    uint32_t VirtualAddress;
    uint32_t SizeOfRawData;
    uint32_t PointerToRawData;
    uint32_t PointerToRelocations;
    uint32_t PointerToLinenumbers;
    uint16_t NumberOfRelocations;
    uint16_t NumberOfLinenumbers;
    uint32_t Characteristics;
};

/// @brief Structure containing PE file data

struct __attribute__((packed)) PE_file {
    /// @brief Structure containing PE header data
    uint32_t header_offset;
    uint32_t Magic;
    struct PE_header header;
    /// @brief Structure containing section header data
    struct Section_header* sectionHeader;
};
/// @brief Function to read PE
/// @param[in] in input file
/// @param[in] peFile structure containing PE file data
/// @return true in case of successful reading PE
bool read_PE (FILE* in, struct PE_file* peFile);

/// @brief Function to find section by name
/// @param[in] peFile structure containing PE file data
/// @param[in] name name of desired section
/// @return structure containing section header data
struct Section_header* find_section(struct PE_file* peFile, char* name);

/// @brief Function to write desured section from PE file to another file
/// @param[in] out output file
/// @param[in] sectionHeader structure containing section header data
/// @param[in] in input file
/// @return true in case of successful writing
bool write_PE(FILE* out, struct Section_header* sectionHeader, FILE* in);

/// @brief Function to start all action with PE
/// @param[in] in name of income file
/// @param[in] out name of outcome file
/// @param[in] name name of desired section
/// @return true in case of successful executing
bool exec(const char * in, const char * out, char* name);
