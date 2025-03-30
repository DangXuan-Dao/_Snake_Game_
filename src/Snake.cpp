#include "Snake.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "const.h"

Snake::Snake() : snakeHeadTexture(nullptr), snakeBodyTexture(nullptr), snakeTailTexture(nullptr), snakeCurveTexture(nullptr) {        // Tạo rắn hướng đi sang phải
    body.push_back({GRID_WIDTH / 2, GRID_HEIGHT / 2 + 2});
    body.push_back({GRID_WIDTH / 2 - 1, GRID_HEIGHT / 2 + 2});
    body.push_back({GRID_WIDTH / 2 - 2, GRID_HEIGHT / 2 + 2});
    dirX = 1;
    dirY = 0;
}

void Snake::move(bool eat) {                                                                        // Rắn khi di chuyển
    Position newHead = {body[0].x + dirX, body[0].y + dirY};
    if (newHead.x < 0) newHead.x = GRID_WIDTH - 1;
    else if (newHead.x >= GRID_WIDTH) newHead.x = 0;
    if (newHead.y < 2) newHead.y = GRID_HEIGHT + 1;
    else if (newHead.y >= GRID_HEIGHT + 2) newHead.y = 2;
    body.insert(body.begin(), newHead);
    if (!eat) {
        body.pop_back();
    }
}

bool Snake::checkCollision() {                                                                // Kiểm tra va chạm với chính nó
    for (size_t i = 1; i < body.size(); i++) {
        if (body[0].x == body[i].x && body[0].y == body[i].y) {
            return true;
        }
    }
    return false;
}

void Snake::render(SDL_Renderer* renderer, SDL_Texture* snakeHeadTexture, SDL_Texture* snakeBodyTexture, SDL_Texture* snakeTailTexture, SDL_Texture* snakeCurveTexture) {
    if (snakeHeadTexture) {
        SDL_Rect headRect = {body[0].x * TILE_SIZE, body[0].y * TILE_SIZE, TILE_SIZE, TILE_SIZE};                // Vẽ đầu rắn theo hướng đi
        double angle = 0.0;
        if (dirX == 1 && dirY == 0) {
            angle = 0.0;
        } else if (dirX == -1 && dirY == 0) {
            angle = 180.0;
        } else if (dirX == 0 && dirY == 1) {
            angle = 90.0;
        } else if (dirX == 0 && dirY == -1) {
            angle = 270.0;
        }
        SDL_RenderCopyEx(renderer, snakeHeadTexture, nullptr, &headRect, angle, nullptr, SDL_FLIP_NONE);
    }

    if (snakeBodyTexture || snakeCurveTexture) {
        for (size_t i = 1; i < body.size() - 1; i++) {
            SDL_Rect bodyRect = {body[i].x * TILE_SIZE, body[i].y * TILE_SIZE, TILE_SIZE, TILE_SIZE};                        
            double angle = 0.0;
            // Xác định hướng trước và sau của thân rắn
            int dxPrev = body[i].x - body[i - 1].x;                                                
            int dyPrev = body[i].y - body[i - 1].y;
            int dxNext = body[i + 1].x - body[i].x;
            int dyNext = body[i + 1].y - body[i].y;
            // Kiểm tra có phải đoạn thân rắn gấp khúc và qua tường không 
            bool isCurve = (dxPrev != dxNext) || (dyPrev != dyNext);
            bool isPassingWall = false;
            if (abs(dxPrev) > 1 || abs(dxNext) > 1 || abs(dyPrev) > 1 || abs(dyNext) > 1) {
                isPassingWall = true;
            }
            if (!isCurve || isPassingWall) {
                // Vẽ thân thằng
                if (snakeBodyTexture) {
                    if (dxPrev == 1 && dyPrev == 0) {
                        angle = 0.0;
                    } else if (dxPrev == -1 && dyPrev == 0) {
                        angle = 180.0;
                    } else if (dxPrev == 0 && dyPrev == 1) {
                        angle = 90.0;
                    } else if (dxPrev == 0 && dyPrev == -1) {
                        angle = 270.0;
                    }
                    SDL_RenderCopyEx(renderer, snakeBodyTexture, nullptr, &bodyRect, angle, nullptr, SDL_FLIP_NONE);
                }
            } else {
                // Vẽ thân gấp khúc
                if (snakeCurveTexture) {
                    if (dxPrev == 1 && dyPrev == 0) {
                        if (dxNext == 0 && dyNext == 1) {
                            angle = 0.0;
                        } else if (dxNext == 0 && dyNext == -1) {
                            angle = 90.0;
                        }
                    } else if (dxPrev == -1 && dyPrev == 0) {
                        if (dxNext == 0 && dyNext == 1) {
                            angle = 270.0;
                        } else if (dxNext == 0 && dyNext == -1) {
                            angle = 180.0;
                        }
                    } else if (dxPrev == 0 && dyPrev == 1) {
                        if (dxNext == 1 && dyNext == 0) {
                            angle = 180.0;
                        } else if (dxNext == -1 && dyNext == 0) {
                            angle = 90.0;
                        }
                    } else if (dxPrev == 0 && dyPrev == -1) {
                        if (dxNext == 1 && dyNext == 0) {
                            angle = 270.0;
                        } else if (dxNext == -1 && dyNext == 0) {
                            angle = 0.0;
                        }
                    }
                    SDL_RenderCopyEx(renderer, snakeCurveTexture, nullptr, &bodyRect, angle, nullptr, SDL_FLIP_NONE);
                }
            }
        }
    }

    if (snakeTailTexture && body.size() > 1) {                                // Vẽ đuôi rắn
        size_t tailIndex = body.size() - 1;
        SDL_Rect tailRect = {body[tailIndex].x * TILE_SIZE, body[tailIndex].y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        double angle = 0.0;
        int dx = body[tailIndex].x - body[tailIndex - 1].x;
        int dy = body[tailIndex].y - body[tailIndex - 1].y;
        if (dx == 1 && dy == 0) {
            angle = 0.0;
        } else if (dx == -1 && dy == 0) {
            angle = 180.0;
        } else if (dx == 0 && dy == 1) {
            angle = 90.0;
        } else if (dx == 0 && dy == -1) {
            angle = 270.0;
        }
        SDL_RenderCopyEx(renderer, snakeTailTexture, nullptr, &tailRect, angle, nullptr, SDL_FLIP_NONE);
    }
}
