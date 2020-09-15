#include "PPU466.hpp"
#include "Mode.hpp"
#include "load_resources.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//helps with the platforming by checking for collisions with the platforms
	bool check_collisions();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up, space;

	//some weird background animation:
	float background_fade = 0.0f;

	//player position:
	glm::vec2 player_at = glm::vec2(0.0f);

	//player position in level:
	glm::vec2 player_at_in_level = glm::vec2(0.0f);

	//used for keeping track of platform positions
	std::vector< glm::vec2 > platform_positions;

	//used to randomly decide platform starting positions
	std::vector< glm::vec2 > platform_starts;

	//used to keep track of when to restart the game
	int score = 0;

	//used for player physics and animations:
	float air_time_passed = 0.0f;
	float walk_time_passed = 0.0f;
	char gravity = 0;

	//----- drawing handled by PPU466 -----

	PPU466 ppu;
};
