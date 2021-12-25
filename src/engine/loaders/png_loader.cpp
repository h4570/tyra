/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/loaders/png_loader.hpp"

#include "../include/utils/debug.hpp"
#include "../include/utils/string.hpp"
#include <cstdlib>
#include <png.h>

// ----
// Constructors/Destructors
// ----

PngLoader::PngLoader() {}

PngLoader::~PngLoader() {}

// ----
// Methods
// ----

/**
 * @param t_name Without extension. Example "skyfall2"
 * @param t_extension With dot and extension. Example ".png"
 */
void PngLoader::load(Texture &o_texture, const char *t_subfolder, const char *t_name, const char *t_extension)
{
    char *path_part1 = String::createConcatenated(t_subfolder, t_name);
    char *path_part2 = String::createConcatenated("host:", path_part1);
    char *path = String::createConcatenated(path_part2, t_extension);
    delete[] path_part1;
    delete[] path_part2;

    FILE *file = fopen(path, "rb");
    assertMsg(file != NULL, "Failed to open .png file!");

    png_structp png_ptr;
    png_infop info_ptr;
    png_uint_32 width, height;

    u32 sig_read = 0, row = 0, i = 0, j = 0;
    int bit_depth, color_type, interlace_type;

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    assertMsg(png_ptr, "PNG read struct init failed!");
    info_ptr = png_create_info_struct(png_ptr);
    assertMsg(info_ptr, "PNG info struct init failed!");
    assertMsg(!setjmp(png_jmpbuf(png_ptr)), "PNG reader fatal error!");

    png_init_io(png_ptr, file);
    png_set_sig_bytes(png_ptr, sig_read);
    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);
    png_set_strip_16(png_ptr);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_expand(png_ptr);

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand(png_ptr);

    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);

    png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

    png_read_update_info(png_ptr, info_ptr);

    png_byte pngType = png_get_color_type(png_ptr, info_ptr);
    TextureType type;
    switch (pngType)
    {
    case PNG_COLOR_TYPE_RGB_ALPHA:
        type = TEX_TYPE_RGBA;
        break;
    case PNG_COLOR_TYPE_RGB:
        type = TEX_TYPE_RGB;
        break;
    default:
        assertMsg(true == false, "This png format is not supported! RGB/RGBA only.");
    }

    o_texture.setSize(width, height, type);
    printf("PNGLoader - width: %d | height: %d\n", width, height);

    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    png_byte *row_pointers[height];
    for (row = 0; row < height; row++)
        row_pointers[row] = new png_byte[row_bytes];

    png_read_image(png_ptr, row_pointers);

    u32 x = 0;

    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++)
        {
            o_texture.setData(x, row_pointers[i][4 * j]);
            o_texture.setData(x + 1, row_pointers[i][4 * j + 1]);
            o_texture.setData(x + 2, row_pointers[i][4 * j + 2]);
            if (type == TEX_TYPE_RGBA)
            {
                o_texture.setData(x + 3, ((int)row_pointers[i][4 * j + 3] * 128 / 255));
                x += 4;
            }
            else if (type == TEX_TYPE_RGB)
                x += 3;
        }

    for (row = 0; row < height; row++)
        delete[] row_pointers[row];

    png_read_end(png_ptr, NULL);
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

    delete[] path;
    fclose(file);
}
