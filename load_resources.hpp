#pragma once

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING


#include "PPU466.hpp"

#include <array>

void load_resources(std::array< PPU466::Palette, 8 > &palette_table, std::array< PPU466::Tile, 16 * 16 > &tile_table);