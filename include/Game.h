#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string>
#include "Snake.h"
#include "Food.h"
#include "Position.h"

enum class GameState {
    MENU,
    PLAYING
};

class Game {
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int moveDelay = 100;
    bool running;
    Snake snake;
    Food food;
    int score;
    int highestScore;
    GameState currentState;
    const std::string highestScoreFile;
    TTF_Font* font;
    SDL_Texture* menuBackgroundTexture;
    SDL_Texture* playButtonTexture;
    SDL_Texture* exitButtonTexture;
    SDL_Texture* gameOverbackgroundTexture;
    SDL_Texture* pauseTexture;
    SDL_Texture* playAgainButtonTexture;
    SDL_Texture* exitGameButtonTexture;
    SDL_Rect exitGameButtonRect;
    SDL_Rect playagainRect;
    SDL_Rect exitButtonRect;
    SDL_Rect playButtonRect;
    bool paused;
    bool waitingGameOver;
    Mix_Music* menuMusic;
    Mix_Chunk* eatMusic;
    Mix_Chunk* overMusic;
    Mix_Music* playingMusic;
    Mix_Chunk* clickMusic;
    SDL_Texture* offMusic;
    SDL_Texture* onMusic;
    SDL_Rect muteButtonRect;
    bool mute;

    Game();
    ~Game();
    void run();
    void handleEvents();
    void resetGame();
    void update();
    void loadHighestScore();
    void saveHighestScore();
    void updateHighestScore();
    void renderText(const std::string& text, int x, int y, SDL_Color color);
    void renderBackground();
    void renderMenu();
    void renderScore();
    void renderGameOver();
    void render();
    SDL_Texture* loadTexture(const std::string& path);
};

#endif
