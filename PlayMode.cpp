#include "PlayMode.hpp"

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>

#include <random>
#include <stdlib.h>

PlayMode::PlayMode() {
	//get sprites from the asset pipeline
	load_resources(ppu.palette_table, ppu.tile_table);

	//set starting position for player
	player_at.x = 64.0f;
	player_at_in_level.x = 64.0f;

	//generate random platform positions
	//for reminders on how to generate random numbers, I referenced http://www.cplusplus.com/reference/cstdlib/rand/
	for (int i = 0; i < 62; i++)
		platform_starts.push_back(glm::vec2(std::rand() % 20 + 1, std::rand() % 30 + 1));
}

PlayMode::~PlayMode() {}

//helps with the platforming by checking for collisions with the platforms
bool PlayMode::check_collisions() {
	for (int i = 0; i < platform_positions.size(); i++) {
		if (player_at.x >= platform_positions[i].x && player_at.x <= platform_positions[i].x + 7) {
			if (player_at.y >= platform_positions[i].y && player_at.y <= platform_positions[i].y + 7) {
				if (i < 55) {
					player_at.x = 64.0f;
					player_at.y = 0.0f;
					player_at_in_level.x = 64.0f;
					gravity = 0;
					air_time_passed = 1.0f;
					return true;
				}
				else {
					ppu.sprites[i + 1].y = int32_t(241.0f);
					platform_positions[i].y = 241.0f;
					score += 1;
				}
			}
			if (player_at.y + 7 > platform_positions[i].y && player_at.y + 7 < platform_positions[i].y + 7) {
				if (i < 55) {
					player_at.x = 64.0f;
					player_at.y = 0.0f;
					player_at_in_level.x = 64.0f;
					gravity = 0;
					air_time_passed = 1.0f;
					return true;
				}
				else {
					ppu.sprites[i + 1].y = int32_t(241.0f);
					platform_positions[i].y = 241.0f;
					score += 1;
				}
			}
		}
		if (player_at.x + 7 >= platform_positions[i].x && player_at.x + 7 <= platform_positions[i].x + 7) {
			if (player_at.y >= platform_positions[i].y && player_at.y <= platform_positions[i].y + 7) {
				if (i < 55) {
					player_at.x = 64.0f;
					player_at.y = 0.0f;
					player_at_in_level.x = 64.0f;
					gravity = 0;
					air_time_passed = 1.0f;
					return true;
				}
				else {
					ppu.sprites[i + 1].y = int32_t(241.0f);
					platform_positions[i].y = 241.0f;
					score += 1;
				}
			}
			if (player_at.y + 7 > platform_positions[i].y && player_at.y + 7 < platform_positions[i].y + 7) {
				if (i < 55) {
					player_at.x = 64.0f;
					player_at.y = 0.0f;
					player_at_in_level.x = 64.0f;
					gravity = 0;
					air_time_passed = 1.0f;
					return true;
				}
				else {
					ppu.sprites[i + 1].y = int32_t(241.0f);
					platform_positions[i].y = 241.0f;
					score += 1;
				}
			}
		}
	}
	return false;
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			if (!left.pressed) walk_time_passed = 0.25f;
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			if (!right.pressed) walk_time_passed = 0.25f;
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.downs += 1;
			down.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_SPACE) {
			if (!space.pressed) {
				if (gravity == 0) gravity = 1;
				else gravity = 0;
				air_time_passed = 1.0f;
			}
			space.downs += 1;
			space.pressed = true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_SPACE) {
			space.pressed = false;
			return true;
		}
	}
	return false;
}

void PlayMode::update(float elapsed) {
	//reset the game once all the stars are collected
	if (score >= 6) {
		score = 0;
		platform_starts.clear();
		for (int i = 0; i < 62; i++) {
			ppu.sprites[i].y = int32_t(240.0f);
			platform_starts.push_back(glm::vec2(std::rand() % 20 + 1, std::rand() % 20 + 1));
		}
		player_at.x = 64.0f;
		player_at.y = 0.0f;
		player_at_in_level.x = 64.0f;
		gravity = 0;
		air_time_passed = 1.0f;
	}

	float vertical_multiplier = 1.5f;
	if (gravity == 1) vertical_multiplier *= -1.0f;
	constexpr float PlayerSpeed = 30.0f;
	if (left.pressed) {
		walk_time_passed += elapsed;
		if (!check_collisions()) {
			player_at.x = std::max(player_at.x - PlayerSpeed * elapsed, 64.0f);
			player_at_in_level -= PlayerSpeed * elapsed;
		}
	}
	if (right.pressed) {
		walk_time_passed += elapsed;
		if (!check_collisions()) {
			player_at.x = std::min(player_at.x + PlayerSpeed * elapsed, 128.0f);
			player_at_in_level += PlayerSpeed * elapsed;
		}
	}
	if ((gravity == 0 && player_at.y > 0) || (gravity == 1 && player_at.y < 232)) {
		if (!check_collisions()) {
			if (air_time_passed <= 0.75f) {
				player_at.y += PlayerSpeed * elapsed * vertical_multiplier;
				air_time_passed += elapsed;
			}
			else if (air_time_passed <= 0.9f) air_time_passed += elapsed;
			else player_at.y -= PlayerSpeed * elapsed * vertical_multiplier;
		}
	}
	if (up.pressed && ((gravity == 0 && player_at.y <= 0) || (gravity == 1 && player_at.y >= 232))) {
		if (!check_collisions()) {
			player_at.y += PlayerSpeed * elapsed * vertical_multiplier;
			air_time_passed = 0.0f;
		}
	}

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//--- set ppu state based on game state ---

	//tilemap gets recomputed every frame to look like some sort of cave (honestly it's still just the plasma thing from the base code lol)
	for (uint32_t y = 0; y < PPU466::BackgroundHeight; ++y) {
		for (uint32_t x = 0; x < PPU466::BackgroundWidth; ++x) {
			ppu.background[x + PPU466::BackgroundWidth*y] = ((x + y) % 16);
		}
	}

	//background scroll:
	ppu.background_position.x = int32_t(-1.0f * player_at_in_level.x);
	//ppu.background_position.y = int32_t(-0.5f * player_at.y);

	//player sprite:
	ppu.sprites[0].x = int32_t(player_at.x);
	ppu.sprites[0].y = int32_t(player_at.y);
	int gravity_sprite = 0;
	if (gravity == 1)
		gravity_sprite = 4;
	if (!(left.pressed || right.pressed)) {
		walk_time_passed = 0.0f;
		ppu.sprites[0].index = 19 + gravity_sprite;
	}
	if (left.pressed) {
		if (walk_time_passed >= 0.25f) {
			if (ppu.sprites[0].index == (17 + gravity_sprite)) ppu.sprites[0].index = 19 + gravity_sprite;
			else ppu.sprites[0].index = 17 + gravity_sprite;
			walk_time_passed = 0.0f;
		}
	}
	if (right.pressed) {
		if (walk_time_passed >= 0.25f) {
			if (ppu.sprites[0].index == (18 + gravity_sprite)) ppu.sprites[0].index = 19 + gravity_sprite;
			else ppu.sprites[0].index = 18 + gravity_sprite;
			walk_time_passed = 0.0f;
		}
	}
	if ((gravity == 0 && player_at.y > 0) || (gravity == 1 && player_at.y < 232))
		ppu.sprites[0].index = 16 + gravity_sprite;
	ppu.sprites[0].attributes = 0b00000111;

	//platorm sprites
	platform_positions.clear();
	for (uint32_t i = 1; i < 63; ++i) {
		if (ppu.sprites[i].y != 241) {
			ppu.sprites[i].x = int32_t(-1.0f * player_at_in_level.x + i * (int)platform_starts[i - 1].x);
			ppu.sprites[i].y = int32_t(i / 7 * (int)platform_starts[i - 1].y + 20);
		}
		glm::vec2 new_platform_entry = glm::vec2(ppu.sprites[i].x, ppu.sprites[i].y);
		platform_positions.push_back(new_platform_entry);
		if (i < 56) {
			ppu.sprites[i].index = 24;
			ppu.sprites[i].attributes = 0b00000110;
		}
		else {
			ppu.sprites[i].index = 25;
			ppu.sprites[i].attributes = 0b00000010;
		}
	}

	//--- actually draw ---
	ppu.draw(drawable_size);
}
