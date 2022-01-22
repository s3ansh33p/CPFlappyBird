/**
 * @file player.hpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Player class for the engine
 * @version 1.0
 * @date 2022-01-14
 */

#pragma once

#include "../../draw_functions.hpp"

// Load bg
uint16_t *bg;

uint32_t frame = 0;

bool game_over = false;

class Player {
    public:
        int16_t x = width / 2 - 17; // - 17 to center the player
        int16_t y = height / 2 - 12; // - 12 to center the player
        int8_t velocity = 1;
        int8_t jumpPower = 6;
        int16_t txWidth = 34;
        int16_t txHeight = 24;
        uint16_t *textures[3];
        uint16_t *bg;
        int8_t animationFrame = 0;
        void init();
        void moveJump();
        void removeOldFrame();
        void animate();
        void loadTextures();
        void line(int16_t x1, int16_t y1, int16_t w, int16_t h);
};

void Player::init() {
	DRAW_TEXTURE(this->bg, 0, 0);
    this->x = width / 2 - 17;
    this->y = height / 2 - 12;
    this->velocity = 1;
    this->animationFrame = 0;
}

void Player::moveJump() {
    if (this->velocity > -this->jumpPower) {
        this->velocity = -this->jumpPower;
    }
}

void Player::line(int16_t x1, int16_t y1, int16_t w, int16_t h) {
    for (int16_t j = y1; j < y1 + h; j++) {
		for (int16_t i = x1; i < x1+w; i++) {
            setPixel(i, j, this->bg[2 + j*320 + i]);
		}
	}
}

void Player::animate() {
    // overwrite buffer for new frame
	for (int16_t j = this->y; j < this->y + this->txHeight; j++) {
		for (int16_t i = this->x; i < this->x+this->txWidth; i++) {
            setPixel(i, j, this->bg[2 + j*320 + i]);
		}
	}

    // calculations
    this->y += this->velocity;
    if (frame % 3 == 0) {
        this->velocity++;
    }

    if (this->y > height) {
        game_over = true;
    }

    this->animationFrame++;
    frame++;
    if (this->animationFrame > 3) {
        this->animationFrame = 0;
    }
    if (this->animationFrame == 1 || this->animationFrame == 3) {
        DRAW_TEXTURE(this->textures[1], this->x, this->y);
    } else {
        DRAW_TEXTURE(this->textures[this->animationFrame], this->x, this->y);
    }
}

void Player::loadTextures() {
    char hexChars[] = {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    };
    char path[32] = "flappy";
    for (int i=0; i<3; i++) {
        path[6] = hexChars[i];
        this->textures[i] = load_texture(path);
    }
    LOAD_TEXTURE_PTR("background", bgTx);
    this->bg = bgTx;
}
