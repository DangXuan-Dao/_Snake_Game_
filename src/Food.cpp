#include "Food.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include "const.h"

Food::Food() : foodTexture(nullptr) {
    spawn();
}

void Food::spawn(const std::vector<Position>& snakeBody) {                                // Tạo thức ăn ở vị trí ngẫu nhiên trong vùng chơi
    bool validPosition = false;
    while (!validPosition) {
        position.x = rand() % GRID_WIDTH;
        position.y = rand() % GRID_HEIGHT + 2;
        validPosition = true;
        for (const auto& segment : snakeBody) {
            if (position.x == segment.x && position.y == segment.y) {
                validPosition = false;
                break;
            }
        }
    }
}

void Food::render(SDL_Renderer* renderer) {                                                // Vẽ ảnh thức ăn
    if (foodTexture) {
        SDL_Rect rect = {position.x * TILE_SIZE, position.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopy(renderer, foodTexture, nullptr, &rect);
    }
}
