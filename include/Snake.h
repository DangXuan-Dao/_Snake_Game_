#ifndef SNAKE_H
#define SNAKE_H

#include<SDL2/SDL.h>
#include <vector>
#include "Position.h"

class Snake {
public:
    std::vector<Position> body;
    int dirX, dirY;
    SDL_Texture* snakeHeadTexture;
    SDL_Texture* snakeBodyTexture;
    SDL_Texture* snakeTailTexture;
    SDL_Texture* snakeCurveTexture;

    Snake();
    void move(bool eat = false);
    bool checkCollision();
    void render(SDL_Renderer* renderer, SDL_Texture* snakeHeadTexture, SDL_Texture* snakeBodyTexture, SDL_Texture* snakeTailTexture, SDL_Texture* snakeCurveTexture);
};

#endif