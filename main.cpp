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

// Pipes
struct Pipe {
	int16_t x;
	int16_t topY;
	int16_t bottomY;
	int16_t color;
};

// Max of 8 on the screen at once
class Pipes {
	public:
		Pipe pipes[8];
		int8_t pipeCount = 0;
		void addPipe();
		void removePipe();
		void render();
};

void Pipes::addPipe() {
	this->pipes[pipeCount].x = width-1;
	pipes[pipeCount].topY = rng->Generate(100)+40;
	pipes[pipeCount].bottomY = pipes[pipeCount].topY + rng->Generate(240)+80;
	pipes[pipeCount].color = color(rng->Generate(255),rng->Generate(255),rng->Generate(255));
	this->pipeCount++;
}

void Pipes::removePipe() {
	// shift pipes down
	for (int i = 1; i < pipeCount; i++) {
		this->pipes[i - 1] = this->pipes[i];
	}
	this->pipeCount--;
}

// void DRAW_PIPE(int16_t x, int16_t y) {
// 	// 320 is height of pipe texture
// 	int16_t yMax = y + 320;
// 	if (yMax > height) {
// 		yMax = height;
// 	}
// 	// 52 is width of pipe texture
// 	int16_t xMax = x + 52;
// 	if (xMax > width) {
// 		xMax = width;
// 	}
// 	for (int16_t j = y; j < yMax; j++) {
// 		for (int16_t i = x; i < xMax; i++) {
//             setPixel(i, j, pipeTx[2 + j*320 + i]);
// 		}
// 	}
// }

void Pipes::render() {
	for (int i = 0; i < this->pipeCount; i++) {
		
		// Top
		int16_t pipeY = this->pipes[i].topY;
		player_pointer->line(this->pipes[i].x, 0, 1, height);
		player_pointer->line(this->pipes[i].x + 52, 0, 1, height);

		player_pointer->line(this->pipes[i].x, 0, this->pipes[i].x + 52, 1);
		player_pointer->line(this->pipes[i].x, pipeY, this->pipes[i].x + 52, 1);

		// Bottom
		int16_t pipeYBottom = height - pipeY;
		player_pointer->line(this->pipes[i].x, 0, this->pipes[i].x + 52, 1);
		player_pointer->line(this->pipes[i].x, pipeYBottom, this->pipes[i].x + 52, 1);
		
		// Top
		pipeY = this->pipes[i].topY;
		int16_t pipeColor = this->pipes[i].color;
		line(this->pipes[i].x, 1, this->pipes[i].x, pipeY, pipeColor);
		line(this->pipes[i].x + 52, 1, this->pipes[i].x + 52, pipeY, pipeColor);
		line(this->pipes[i].x, 1, this->pipes[i].x + 52, 1, pipeColor);
		line(this->pipes[i].x, pipeY, this->pipes[i].x + 52, pipeY, pipeColor);

		// Bottom
		pipeY = this->pipes[i].bottomY;
		pipeYBottom = height;
		line(this->pipes[i].x, pipeY, this->pipes[i].x, pipeYBottom, pipeColor);
		line(this->pipes[i].x + 52, pipeY, this->pipes[i].x + 52, pipeYBottom, pipeColor);
		line(this->pipes[i].x, 1, this->pipes[i].x + 52, 1, pipeColor);
		line(this->pipes[i].x, pipeYBottom, this->pipes[i].x + 52, pipeYBottom, pipeColor);
		

		// Movement
		this->pipes[i].x -= 1;

		// new render


		// 52 is texture width
		if (this->pipes[i].x < -52) {
			this->removePipe();
		}
	}
}

// Ends the game and is called by the event handler
void endGame() {
	game_running = false;
}

// jump
void jump() {
	player_pointer->moveJump();
}

//The acutal main
void main2() {

	RandomGenerator rngp;
	rngp.SetSeed(1337);
	rng = &rngp;

	Pipes pipes;

	// load the textures and fonts
	LOAD_FONT_PTR("fnt\\5x6", f_5x6);

	// Add event listeners
	addListener(KEY_BACKSPACE, toggleDebug); // toggle debug mode
	addListener(KEY_CLEAR, endGame); // end the game

	addListener2(KEY_UP, jump); // jump

	Player player;
	player.loadTextures();
	player.init();
	player_pointer = &player;

	// game starting screen
	for (int i = 0; i < 100; i+=5) {
		draw_font_shader(f_5x6, "Flappy Bird", 12, 128, color(255, 193, 0), 0, 0, 4, color(203, 193, 0));
		LCD_Refresh();
	}

	// redraw bg
	player.init();

	uint32_t frame = 0;

	while (game_running) {
		frame++;
		checkEvents();

		// game events
		if ((frame+120) % 150 == 0) {
			pipes.addPipe();
		}

		pipes.render();

		player.animate();
		
		debugger(frame);
		LCD_Refresh();
	}
	
	// game ending screen
	// for (int i = 0; i < 30; i+=2) {
	// 	fillScreen(color(240, 240, 240));
	// 	draw_font_shader(f_5x6, "Game Over", 1, 248, color(50, 45, 45), 0, 0, 3, i);
	// 	LCD_Refresh();
	// }

	// free memory
	free(f_5x6);
	// free(player_pointer);
	// free(rng);
}
