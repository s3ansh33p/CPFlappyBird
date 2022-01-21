/**
 * @file player.hpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Player class for the engine
 * @version 1.0
 * @date 2022-01-14
 */

#pragma once

#include "../../draw_functions.hpp"

class Player {
    public:
        int16_t x = width / 2;
        int16_t y = height / 2;
        int8_t moveSpeed = 4;
        int8_t jumpPower = 10;
        int8_t txWidth = 34;
        int8_t txHeight = 24;
        uint16_t *textures[3];
        int8_t animationFrame = 0;
        void moveJump();
        void removeOldFrame();
        void animate();
        void loadTextures();
};

void Player::moveJump() {

}

void Player::removeOldFrame() {
	for (int x=this->x; x<this->x+this->txWidth; x++){
		for (int y=this->y; y<this->y+this->txHeight; y++){
			setPixel(x,y, color(255,0,0));
		}
    }
}

void Player::animate() {
    this->removeOldFrame();
    this->animationFrame++;
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
}
