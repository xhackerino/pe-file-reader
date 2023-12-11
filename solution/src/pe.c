/// @file
/// @brief File with functions for working with PE

#include "pe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// @brief Function to read PE
/// @param[in] in input file
/// @param[in] peFile structure containing PE file data
/// @return true in case of successful reading PE

bool read_PE(FILE *in, struct PE_file *peFile)
{
    /// @brief Check if input file and PE file structure are not NULL
    if (in == NULL || peFile == NULL)
    {
        return false;
    }
    fseek(in, MAIN_OFFSET, SEEK_SET);
    fread(&peFile->header_offset, sizeof(uint32_t), 1, in);
    fseek(in, peFile->header_offset, SEEK_SET);
    fread(&peFile->Magic, sizeof(uint32_t), 1, in);
    fread(&peFile->header, sizeof(struct PE_header), 1, in);
    fseek(in, peFile->header.SizeOfOptionalHeader, SEEK_CUR);

    /// @brief Allocate memory for section headers
    peFile->sectionHeader = malloc(peFile->header.NumberOfSections * sizeof(struct Section_header));
    for (size_t i = 0; i < peFile->header.NumberOfSections; i++)
    {
        fread(&peFile->sectionHeader[i], sizeof(struct Section_header), 1, in);
    }

    return true;
}

/// @brief Function to find section by name
/// @param[in] peFile structure containing PE file data
/// @param[in] name name of desired section
/// @return structure containing section header data

struct Section_header *find_section(struct PE_file *peFile, char *name)
{
    for (size_t i = 0; i < peFile->header.NumberOfSections; i++)
    {
        /// @brief Check if section name is equal to desired name
        if (strcmp(peFile->sectionHeader[i].name, name) == 0)
        {
            return &peFile->sectionHeader[i];
        }
    }

    return NULL;
}

/// @brief Function to write desured section from PE file to another file
/// @param[in] out output file
/// @param[in] sectionHeader structure containing section header data
/// @param[in] in input file
/// @return true in case of successful writing

bool write_PE(FILE *out, struct Section_header *sectionHeader, FILE *in)
{
    /// @brief Check if input file, output file and section header are not NULL
    if (out == NULL || sectionHeader == NULL || in == NULL)
    {
        return false;
    }

    fseek(in, sectionHeader->PointerToRawData, SEEK_SET);
    char *buffer = malloc(sectionHeader->SizeOfRawData * sizeof(char));
    fread(buffer, sizeof(char), sectionHeader->SizeOfRawData, in);
    fwrite(buffer, sizeof(char), sectionHeader->SizeOfRawData, out);
    free(buffer);

    return true;
}

/// @brief Function to execute program
/// @param[in] in input file
/// @param[in] out output file
/// @param[in] name name of desired section
/// @return true in case of successful execution

bool exec(const char * in, const char * out, char* name){
    FILE* in_file = fopen(in, "rb");
    /// @brief Check if input file is not NULL
    if (in_file == NULL)
    {
        return false;
    }

    FILE* out_file = fopen(out, "wb");
    /// @brief Check if output file is not NULL
    if (out_file == NULL)
    {
        fclose(in_file);
        return false;
    }

    struct PE_file peFile;
    /// @brief Check if PE file is read successfully
    if (!read_PE(in_file, &peFile))
    {
        free(peFile.sectionHeader);
        fclose(in_file);
        fclose(out_file);
        return false;
    }

    struct Section_header* sectionHeader = find_section(&peFile, name);
    /// @brief Check if section is found
    if (sectionHeader == NULL)
    {
        free(peFile.sectionHeader);
        fclose(in_file);
        fclose(out_file);
        return false;
    }

    /// @brief Check if section is written successfully
    if (!write_PE(out_file, sectionHeader, in_file))
    {
        free(peFile.sectionHeader);
        fclose(in_file);
        fclose(out_file);
        return false;
    }

    /// @brief Free allocated memory and close files
    free(peFile.sectionHeader);
    fclose(in_file);
    fclose(out_file);

    return true;
}
