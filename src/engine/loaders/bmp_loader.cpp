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
void BmpLoader::load(MeshTexture &o_texture, char *t_subfolder, char *t_name, char *t_extension)
{
    char *t_path_part = String::createConcatenated(t_subfolder, t_name);
    char *t_path = String::createConcatenated(t_path_part, t_extension);
    delete[] t_path_part;
    FILE *file = fopen(t_path, "rb");

    if (file == NULL)
    {
        PRINT_ERR("Failed to load .bmp file!");
        return;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, file);

    u32 width = *(u32 *)&header[18];
    u32 height = *(u32 *)&header[22];
    o_texture.setSize(width, height);
    printf("BMPLoader - width: %d | height: %d\n", width, height);
    if (width > 128 || height > 128)
        PRINT_ERR("Given texture is big for PS2. Please strict to 128x128 max");

    u64 rowPadded = (width * 3 + 3) & (~3);

    unsigned char *data = new unsigned char[rowPadded];
    unsigned char tmp;

    u32 x = 0;
    for (u32 i = 0; i < height; i++)
    {
        fread(data, sizeof(unsigned char), rowPadded, file);
        for (u32 j = 0; j < width * 3; j += 3)
        {
            // Convert (B, G, R) to (R, G, B)
            tmp = data[j];
            data[j] = data[j + 2];
            data[j + 2] = tmp;

            o_texture.setData(x, data[j]);
            o_texture.setData(x + 1, data[j + 1]);
            o_texture.setData(x + 2, data[j + 2]);
            x += 3;
        }
    }
    delete[] t_path;
    delete[] data;
    fclose(file);
}
