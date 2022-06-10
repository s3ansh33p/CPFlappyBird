#include <appdef.hpp>

#include "calc.hpp"
#include "draw_functions.hpp"
#include "lib/core/event_handler.hpp"
#include "lib/core/debug.hpp"
#include "lib/core/player.hpp"
#include "lib/functions/random.hpp"
#include "lib/collision/collisions.hpp"

#ifndef PC
	APP_NAME("Flappy Bird")
	APP_DESCRIPTION("Flappy Bird ported using elements of hhkEngine")
	APP_AUTHOR("s3ansh33p")
	APP_VERSION("1.0.0")
#endif

// Tracks the main game loop
bool game_running = true;

// Determines if the user in the restart screen
bool restart_screen = false;

// RNG pointer
RandomGenerator* rng;

// Player pointer
Player* player_pointer;

const uint16_t pipeHeight = 320;
const uint16_t pipeWidth = 52;

// Pipes
struct Pipe {
	int16_t x;
	int16_t topY;
	int16_t bottomY;
};

// Max of 3 on the screen at once
class Pipes {
	public:
		Pipe pipes[3];
		int8_t pipeCount = 0;
		uint16_t *textures[2];
		void addPipe();
		void removePipe();
		void render();
		void drawPipe(int16_t x, int16_t y, int8_t index);
		void checkCollision(int16_t px, int16_t py, int16_t pw, int16_t ph);
};

void Pipes::checkCollision(int16_t px, int16_t py, int16_t pw, int16_t ph) {
	for (int8_t i = 0; i < this->pipeCount; i++) {
		if (boxBox(px, py, pw, ph, this->pipes[i].x, 0, pipeWidth, this->pipes[i].topY) || boxBox(px, py, pw, ph, this->pipes[i].x, this->pipes[i].bottomY, pipeWidth, height-this->pipes[i].bottomY)) {
			// game over
			game_over = true;
		}
	}
}

void Pipes::addPipe() {
	this->pipes[pipeCount].x = width-1;
	pipes[pipeCount].topY = rng->Generate(100)+80;
	pipes[pipeCount].bottomY = pipes[pipeCount].topY + rng->Generate(200)+100;
	this->pipeCount++;
}

void Pipes::removePipe() {
	// shift pipes down
	for (int i = 1; i < pipeCount; i++) {
		this->pipes[i - 1] = this->pipes[i];
	}
	this->pipeCount--;
}

void Pipes::drawPipe(int16_t x, int16_t y, int8_t index) {
	for (int16_t j = 0; j < pipeHeight; j++) {
		for (int16_t i = 0; i < pipeWidth; i++) {
			if (this->textures[0][2 + j*pipeWidth + i] != TRANSPARENCY_COLOR) {
            	setPixel(x+i, y+j, this->textures[index][2 + j*pipeWidth + i]);
			}
		}
	}
}

void Pipes::render() {
	for (int i = 0; i < this->pipeCount; i++) {
		
		this->drawPipe(this->pipes[i].x, this->pipes[i].topY - pipeHeight, 1);
		//manual draw outset - some bug idk
		line(this->pipes[i].x + 50, this->pipes[i].topY - 24, this->pipes[i].x + 50, this->pipes[i].topY - 1, color(84, 56, 71));
		line(this->pipes[i].x, this->pipes[i].topY - 24, this->pipes[i].x, this->pipes[i].topY - 1, color(84, 56, 71));
		player_pointer->line(this->pipes[i].x + pipeWidth, this->pipes[i].topY - 24, 1, 24); // clear old pipe line outset
		player_pointer->line(this->pipes[i].x + (pipeWidth-2), this->pipes[i].topY - pipeHeight, 1, (pipeHeight-24)); // clear old pipe line inset

		this->drawPipe(this->pipes[i].x, this->pipes[i].bottomY, 0);
		player_pointer->line(this->pipes[i].x + pipeWidth, this->pipes[i].bottomY, 1, 24); // clear old pipe line outset
		player_pointer->line(this->pipes[i].x + (pipeWidth-2), this->pipes[i].bottomY+24, 1, (pipeHeight-24)); // clear old pipe line inset


		// Movement
		this->pipes[i].x -= 1;

		if (this->pipes[i].x < -pipeWidth) {
			this->removePipe();
		}
	}
}

// Ends the game and is called by the event handler
void endGame() {
	restart_screen = false;
	game_running = false;
}

// Restarts the game and is called by the event handler
void restart() {
	restart_screen = false;
	game_over = false;
	player_pointer->init();
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
	pipes.textures[0] = load_texture("pipe0");
	pipes.textures[1] = load_texture("pipe1");

	// load the textures and fonts
	LOAD_FONT_PTR("5x6", f_5x6);
	LOAD_FONT_PTR("7x8", f_7x8);

	// Add event listeners
	addListener(KEY_BACKSPACE, toggleDebug); // toggle debug mode
	addListener(KEY_CLEAR, endGame); // end the game

	addListener2(KEY_UP, jump); // jump
	addListener(KEY_EXE, restart); // restart the game

	Player player;
	player.loadTextures();
	player.init();
	player_pointer = &player;

	// Load game over screen (192x42 px)
	LOAD_TEXTURE_PTR("gameover", gameover);

	// game starting screen
	for (int i = 0; i < 64; i+=5) {
		draw_font_shader(f_5x6, "Flappy Bird", 20, 100, color(252, 160, 72), 0, 0, 4, color(228, 96, 24));
		LCD_Refresh();
	}

	// redraw bg
	player.init();

	uint32_t frame = 0;
	char score[12] = "Score: 0   ";

	while (game_running) {
		frame++;
		checkEvents();

		// game events
		if ((frame+120) % 150 == 0) {
			pipes.addPipe();
		}

		if ((frame-100) % 150 == 0) {
			int16_t scoreInt = frame / 150;
			int8_t xCount = 9;
			// score is the number of pipes that have passed or frames / 150
			if (scoreInt > 999) {
				xCount = 12;
				score[7] = '0' + (scoreInt / 1000);
				score[8] = '0' + (scoreInt / 100) % 10;
				score[9] = '0' + (scoreInt / 10) % 10;
				score[10] = '0' + scoreInt % 10;
			} if (scoreInt > 99) {
				xCount = 11;
				score[7] = '0' + (scoreInt / 100) % 10;
				score[8] = '0' + (scoreInt / 10) % 10;
				score[9] = '0' + scoreInt % 10;
			} if (scoreInt > 9) {
				xCount = 10;
				score[7] = '0' + (scoreInt / 10) % 10;
				score[8] = '0' + scoreInt % 10;
			} else {
				score[7] = '0' + scoreInt % 10;
			}
			// clear old score
		    for (int i = 0; i < 8; i++) {
				for (int j = 49; j < xCount*7; j++) {
					setPixel(12+j, 12+i, color(78, 192, 202));
				}
    		}
		}
		pipes.render();

		player.animate();

		DRAW_FONT(f_7x8, score, 12, 12, color(255, 255, 255), 0);

		pipes.checkCollision(player.x, player.y, player.txWidth, player.txHeight);

		if (game_over) {
			DRAW_TEXTURE(gameover, 64, 192);
			LCD_Refresh();
			// load restart screen
			restart_screen = true;
			while(restart_screen) {
				checkEvents();
			}
			frame = 0;
			for (int i = 0; i < pipes.pipeCount; i++) {
				pipes.removePipe();
			}
			score[7] = '0';
			score[8] = ' ';
			score[9] = ' ';
			score[10] = ' ';
		}
		
		debugger(frame);
		LCD_Refresh();
	}

	// free memory
	free(f_5x6);
	free(f_7x8);
	// free(player_pointer);
	// free(rng);
}
