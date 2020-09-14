#include "load_resources.hpp"

#include "data_path.hpp"
#include "load_save_png.hpp"

#include <experimental/filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

namespace fs = std::experimental::filesystem;

void load_resources(std::array< PPU466::Palette, 8 >& palette_table, std::array< PPU466::Tile, 16 * 16 >& tile_table) {
	//initiating the palette_table and the tile_table in the same way as in PPU466.cpp
	for (auto& palette : palette_table) {
		palette[0] = glm::u8vec4(0x00, 0x00, 0x00, 0x00);
		palette[1] = glm::u8vec4(0x00, 0x00, 0x00, 0xff);
		palette[2] = glm::u8vec4(0x00, 0x00, 0x00, 0xff);
		palette[3] = glm::u8vec4(0x00, 0x00, 0x00, 0xff);
	}

	for (auto& tile : tile_table) {
		tile.bit0 = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
		tile.bit1 = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	}

	//find the resources folder, which contains the game's assets
	std::string path = resources_path();
	for (auto& asset_path : fs::directory_iterator(path)) {
		//for help converting the asset directory_entry to a string, I used http://www.cpp.re/forum/beginner/263980/
		std::string asset_string = asset_path.path().string();

		int palette_index = 6;
		//for help with this if statement, I used https://stackoverflow.com/questions/2340281/check-if-a-string-contains-a-string-in-c
		if (asset_string.find("background") != std::string::npos) {
			palette_index = 0;
		}
		else if (asset_string.find("player") != std::string::npos) {
			palette_index = 7;
		}

		//maybe reading in a PNG properly using load_png, I'm honestly not sure
		glm::uvec2 asset_size;
		std::vector< glm::u8vec4 > asset_data;
		OriginLocation *asset_origin = new OriginLocation();
		load_png(asset_string, &asset_size, &asset_data, *asset_origin);

		int num_colors = 0;
		std::vector< int > color_vector;
		for (int i = 0; i < asset_data.size(); i++) {
			//setting the palette table
			if (asset_data[i] == palette_table[palette_index][0]) {
				color_vector.push_back(0);
			}
			else if (asset_data[i] == palette_table[palette_index][1]) {
				color_vector.push_back(1);
			}
			else if (asset_data[i] == palette_table[palette_index][2]) {
				color_vector.push_back(2);
			}
			else if (asset_data[i] == palette_table[palette_index][3]) {
				color_vector.push_back(3);
			}
			else {
				palette_table[palette_index][num_colors] = asset_data[i];
				//palette_table[1][num_colors] = asset_data[i];
				color_vector.push_back(num_colors);
				num_colors += 1;
			}

			if (asset_string.find("background") != std::string::npos) {
				//setting the tile table
				if ((i - 7) % 8 == 0) {
					//getting bit0 and bit1 a row at a time
					int bit0 = 0;
					int bit1 = 0;
					for (int j = i; j > i - 8; j--) {
						if (color_vector[j] == 1) {
							bit0 += (int)pow(2, i - j);
						}
						else if (color_vector[j] == 2) {
							bit1 += (int)pow(2, i - j);
						}
						else if (color_vector[j] == 3) {
							bit0 += (int)pow(2, i - j);
							bit1 += (int)pow(2, i - j);
						}
					}
					//set bit0 and bit1 in the tile table
					int x = i % asset_size[0];
					int y = (i % (asset_size[1] * 8) / 8) % 8;
					int tile_table_index = i / 64;
					tile_table[tile_table_index].bit0[y] = bit0;
					tile_table[tile_table_index].bit1[y] = bit1;
				}
			}

			if (asset_string.find("player") != std::string::npos) {
				//setting the tile table
				if ((i - 7) % 8 == 0) {
					//getting bit0 and bit1 a row at a time
					int bit0 = 0;
					int bit1 = 0;
					for (int j = i; j > i - 8; j--) {
						if (color_vector[j] == 1) {
							bit0 += (int)pow(2, i - j);
						}
						else if (color_vector[j] == 2) {
							bit1 += (int)pow(2, i - j);
						}
						else if (color_vector[j] == 3) {
							bit0 += (int)pow(2, i - j);
							bit1 += (int)pow(2, i - j);
						}
					}
					//set bit0 and bit1 in the tile table
					int x = i % asset_size[0];
					int y = (i % (asset_size[1] * 8) / 8) % 8;
					int tile_table_index = i / 64 + 32;
					tile_table[tile_table_index].bit0[y] = bit0;
					tile_table[tile_table_index].bit1[y] = bit1;
				}
			}
		}
	}
}