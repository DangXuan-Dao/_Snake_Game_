#ifndef FOOD_H
#define FOOD_H

#include <SDL2/SDL.h>
#include <vector>
#include "Position.h"

class Food {
public:
    Position position;
    SDL_Texture* foodTexture;

    Food();
    void spawn(const std::vector<Position>& snakeBody = {});
    void render(SDL_Renderer* renderer);
};

#endif