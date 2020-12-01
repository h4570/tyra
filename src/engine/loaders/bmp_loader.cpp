/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/loaders/bmp_loader.hpp"

#include "../include/utils/debug.hpp"
#include "../include/utils/string.hpp"
#include <cstdlib>

// ----
// Constructors/Destructors
// ----

BmpLoader::BmpLoader() {}

BmpLoader::~BmpLoader() {}

// ----
// Methods
// ----

/**
 * @param t_name Without extension. Example "skyfall2"
 * @param t_extension With dot and extension. Example ".BMP"
 */
void BmpLoader::load(Texture &o_texture, char *t_subfolder, char *t_name, char *t_extension)
{
    char *path_part1 = String::createConcatenated(t_subfolder, t_name);
    char *path_part2 = String::createConcatenated("host:", path_part1);
    char *path = String::createConcatenated(path_part2, t_extension);
    delete[] path_part1;
    delete[] path_part2;
    FILE *file = fopen(path, "rb");

    if (file == NULL)
    {
        PRINT_ERR("Failed to load .bmp file!");
        return;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, file);

    u32 width = (u32)header[18];
    u32 height = (u32)header[22];
    o_texture.setSize(width, height, TEX_TYPE_RGB);
    printf("BMPLoader - width: %d | height: %d\n", width, height);

    u64 rowPadded = (width * 3 + 3) & (~3);

    unsigned char row[rowPadded];

    u32 x = 0;
    for (u32 i = 0; i < height; i++)
    {
        fread(&row, sizeof(unsigned char), rowPadded, file);
        for (u32 j = 0; j < width * 3; j += 3)
        {
            // Convert (B, G, R) to (R, G, B)
            o_texture.setData(x, row[j + 2]);
            o_texture.setData(x + 1, row[j + 1]);
            o_texture.setData(x + 2, row[j]);
            x += 3;
        }
    }
    delete[] path;
    fclose(file);
}
