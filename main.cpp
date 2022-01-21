#include <appdef.hpp>

#include "calc.hpp"
#include "draw_functions.hpp"
#include "lib/core/event_handler.hpp"
#include "lib/core/debug.hpp"
#include "lib/core/player.hpp"
#include "lib/functions/random.hpp"

#ifndef PC
	APP_NAME("Flappy Bird")
	APP_DESCRIPTION("Flappy Bird ported using elements of hhkEngine")
	APP_AUTHOR("s3ansh33p")
	APP_VERSION("0.0.1")
#endif

// Tracks the main game loop
bool game_running = true;

// RNG pointer
RandomGenerator* rng;

// Player pointer
Player* player_pointer;

// Ends the game and is called by the event handler
void endGame() {
	game_running = false;
}

// jump
void jump() {
}

//The acutal main
void main2() {

	RandomGenerator rngp;
	rngp.SetSeed(1337);
	rng = &rngp;

	// load the textures and fonts
	LOAD_FONT_PTR("fnt\\5x6", f_5x6);

	// game starting screen
	for (int i = 0; i < 100; i+=5) {
		fillScreen(color(22, 22, 22));
		draw_font_shader(f_5x6, "Flappy Bird", 1, 128, color(77, 193, 203), 0, 0, 4, color(155, 219, 214));
		LCD_Refresh();
	}
	
	uint32_t frame = 0;

	// Add event listeners
	addListener(KEY_BACKSPACE, toggleDebug); // toggle debug mode
	addListener(KEY_CLEAR, endGame); // end the game

	addListener2(KEY_UP, jump, true); // jump

	// Load bg
	LOAD_TEXTURE_PTR("background", bg);
	DRAW_TEXTURE(bg, 0, 0);

	Player player;
	player.loadTextures();
	player_pointer = &player;


	while (game_running) {
		frame++;
		checkEvents();

		player.animate();
		
		debugger(frame);
		LCD_Refresh();
	}
	
	// game ending screen
	// for (int i = 0; i < 30; i+=2) {
	// 	fillScreen(color(240, 240, 240));
	// 	draw_font_shader(f_7x8, "HHK Engine", 1, 248, color(50, 45, 45), 0, 0, 3, i);
	// 	LCD_Refresh();
	// }

	// free memory
	free(f_5x6);
}
