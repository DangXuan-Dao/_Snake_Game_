#include "Game.h"
#include<iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "const.h"

Game::Game() : running(true), score(0), waitingGameOver(false), currentState(GameState::MENU), highestScoreFile("res/text/HighestScore.txt"), font(nullptr), menuBackgroundTexture(nullptr), playButtonTexture(nullptr), exitButtonTexture(nullptr), gameOverbackgroundTexture(nullptr), pauseTexture(nullptr), playAgainButtonTexture(nullptr), exitGameButtonTexture(nullptr), paused(false), menuMusic(nullptr), eatMusic(nullptr), overMusic(nullptr), playingMusic(nullptr), clickMusic(nullptr), offMusic(nullptr), onMusic(nullptr), mute(false) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        running = false;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        running = false;
    }
    if (TTF_Init() == -1) {
        std::cerr << "Không thể khởi tạo SDL_ttf! Lỗi: " << TTF_GetError() << std::endl;
        running = false;
    }
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        running = false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        running = false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Không thể khởi tạo SDL_mixer! Lỗi: " << Mix_GetError() << std::endl;
        running = false;
    }
    srand(time(0));

    snake.snakeHeadTexture = loadTexture("res/image/snake_head.png");
    if (!snake.snakeHeadTexture) {
        running = false;
    }

    snake.snakeBodyTexture = loadTexture("res/image/snake_body.png");
    if (!snake.snakeBodyTexture) {
        running = false;
    }

    snake.snakeTailTexture = loadTexture("res/image/snake_tail.png");
    if (!snake.snakeTailTexture) {
        running = false;
    }

    snake.snakeCurveTexture = loadTexture("res/image/snake_curve.png");
    if (!snake.snakeCurveTexture) {
        running = false;
    }

    food.foodTexture = loadTexture("res/image/fruit.png");
    if (!food.foodTexture) {
        running = false;
    }

    menuBackgroundTexture = loadTexture("res/image/background_menu.png");
    if (!menuBackgroundTexture) {
        running = false;
    }

    pauseTexture = loadTexture("res/image/pause.png");
    if (!pauseTexture) {
        running = false;
    }

    playButtonTexture = loadTexture("res/image/play.png");
    if (!playButtonTexture) {
        running = false;
    }
    playButtonRect = { (SCREEN_WIDTH - 200) / 2, (SCREEN_HEIGHT - 100) / 2, 200, 100 };

    exitButtonTexture = loadTexture("res/image/exit.png");
    if (!exitButtonTexture) {
        running = false;
    }
    exitButtonRect = { 10, 10, 90, 40 };

    gameOverbackgroundTexture = loadTexture("res/image/game_over_background.png");
    if (!gameOverbackgroundTexture) {
        running = false;
    }

    playAgainButtonTexture = loadTexture("res/image/play_again.png");
    if (!playAgainButtonTexture) {
        running = false;
    }
    playagainRect = { (SCREEN_WIDTH - 200) / 2, (SCREEN_HEIGHT - 100) / 2, 200, 100 };

    exitGameButtonTexture = loadTexture("res/image/exit_game.png");
    if (!exitGameButtonTexture) {
        running = false;
    }
    exitGameButtonRect = { (SCREEN_WIDTH - 200) / 2, (SCREEN_HEIGHT - 100) / 2 + 120, 200, 100 };

    font = TTF_OpenFont("res/text/font-times-new-roman.ttf", 36);
    if (!font) {
        running = false;
    }

    menuMusic = Mix_LoadMUS("res/sound/menu_music.mp3");
    if (!menuMusic) {
        running = false;
    }

    eatMusic = Mix_LoadWAV("res/sound/eating.wav");
    if (!eatMusic) {
        running = false;
    }

    overMusic = Mix_LoadWAV("res/sound/gameover.wav");
    if (!overMusic) {
        running = false;
    }

    playingMusic = Mix_LoadMUS("res/sound/playing_music.mp3");
    if (!playingMusic) {
        running = false;
    }

    offMusic = loadTexture("res/image/off_music.png");
    if (!offMusic) {
        running = false;
    }

    onMusic = loadTexture("res/image/on_music.png");
    if (!onMusic) {
        running = false;
    }

    muteButtonRect = {10, 10, 40, 40};

    clickMusic = Mix_LoadWAV("res/sound/clickmouse.wav");
    if (!clickMusic) {
        running = false;
    }

    loadHighestScore();
}

Game::~Game() {
    if (food.foodTexture) {
        SDL_DestroyTexture(food.foodTexture);
    }
    if (snake.snakeHeadTexture) {
        SDL_DestroyTexture(snake.snakeHeadTexture);
    }
    if (snake.snakeBodyTexture) {
        SDL_DestroyTexture(snake.snakeBodyTexture);
    }
    if (snake.snakeTailTexture) {
        SDL_DestroyTexture(snake.snakeTailTexture);
    }
    if (snake.snakeCurveTexture) {
        SDL_DestroyTexture(snake.snakeCurveTexture);
    }
    if (menuBackgroundTexture) {
        SDL_DestroyTexture(menuBackgroundTexture);
    }
    if (playButtonTexture) {
        SDL_DestroyTexture(playButtonTexture);
    }
    if (exitButtonTexture) {
        SDL_DestroyTexture(exitButtonTexture);
    }
    if (gameOverbackgroundTexture) {
        SDL_DestroyTexture(gameOverbackgroundTexture);
    }
    if (pauseTexture) {
        SDL_DestroyTexture(pauseTexture);
    }
    if (playAgainButtonTexture) {
        SDL_DestroyTexture(playAgainButtonTexture);
    }
    if (exitGameButtonTexture) {
        SDL_DestroyTexture(exitGameButtonTexture);
    }
    if (font) {
        TTF_CloseFont(font);
    }
    if (menuMusic) {
        Mix_FreeMusic(menuMusic);
    }
    if (eatMusic) {
        Mix_FreeChunk(eatMusic);
    }
    if (overMusic) {
        Mix_FreeChunk(overMusic);
    }
    if (offMusic) {
        SDL_DestroyTexture(offMusic);
    }
    if (onMusic) {
        SDL_DestroyTexture(onMusic);
    }
    if (clickMusic) {
        Mix_FreeChunk(clickMusic);
    }
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::run() {
    Uint32 lastUpdate = SDL_GetTicks();
    while (running) {
        handleEvents();

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastUpdate >= 100) {
            if (currentState == GameState::PLAYING && !paused && !waitingGameOver) {
                update();
            }
            lastUpdate = currentTime;
        }

        render();

        if (currentState == GameState::MENU && Mix_PlayingMusic() == 0 && !mute) {
            Mix_PlayMusic(menuMusic, -1);
        }
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN && currentState == GameState::PLAYING && !waitingGameOver) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    if (snake.dirY == 0) {
                        snake.dirX = 0;
                        snake.dirY = -1;
                    }
                    break;
                case SDLK_DOWN:
                    if (snake.dirY == 0) {
                        snake.dirX = 0;
                        snake.dirY = 1;
                    }
                    break;
                case SDLK_LEFT:
                    if (snake.dirX == 0) {
                        snake.dirX = -1;
                        snake.dirY = 0;
                    }
                    break;
                case SDLK_RIGHT:
                    if (snake.dirX == 0) {
                        snake.dirX = 1;
                        snake.dirY = 0;
                    }
                    break;
                case SDLK_ESCAPE:
                    paused = !paused;
                    break;
                case SDLK_s:
                    highestScore = 0;
                    saveHighestScore();
                    break;
            }
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (currentState == GameState::MENU) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (mouseX >= playButtonRect.x && mouseX <= playButtonRect.x + playButtonRect.w &&
                    mouseY >= playButtonRect.y && mouseY <= playButtonRect.y + playButtonRect.h) {
                    currentState = GameState::PLAYING;

                    if (clickMusic && !mute) {
                        Mix_PlayChannel(-1, clickMusic, 0);
                    }

                    if (Mix_PlayingMusic()) {
                        Mix_HaltMusic();
                    }

                    if (playingMusic && !mute) {
                        Mix_PlayMusic(playingMusic, -1);
                    }
                }

                if (mouseX >= muteButtonRect.x && mouseX <= muteButtonRect.x + muteButtonRect.w &&
                    mouseY >= muteButtonRect.y && mouseY <= muteButtonRect.y + muteButtonRect.h) {
                    mute = !mute;

                    if (mute) {
                        Mix_PauseMusic();
                        Mix_Volume(-1, 0);
                    } else {
                        Mix_ResumeMusic();
                        Mix_Volume(-1, MIX_MAX_VOLUME);
                    }
                }
            }
            if (currentState == GameState::PLAYING && !waitingGameOver) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (mouseX >= exitButtonRect.x && mouseX <= exitButtonRect.x + exitButtonRect.w &&
                    mouseY >= exitButtonRect.y && mouseY <= exitButtonRect.y + exitButtonRect.h) {
                    currentState = GameState::MENU;
                    
                    if (clickMusic && !mute) {
                        Mix_PlayChannel(-1, clickMusic, 0);
                    }

                    if (Mix_PlayingMusic()) {
                        Mix_HaltMusic();
                    }

                    if (menuMusic && !mute) {
                        Mix_PlayMusic(menuMusic, -1);
                    }
                }
            }
            if (waitingGameOver) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (mouseX >= playagainRect.x && mouseX <= playagainRect.x + playagainRect.w &&
                    mouseY >= playagainRect.y && mouseY <= playagainRect.y + playagainRect.h) {
                    if (clickMusic && !mute) {
                        Mix_PlayChannel(-1, clickMusic, 0);
                    }
                    resetGame();
                }
                if (mouseX >= exitGameButtonRect.x && mouseX <= exitGameButtonRect.x + exitGameButtonRect.w &&
                    mouseY >= exitGameButtonRect.y && mouseY <= exitGameButtonRect.y + exitGameButtonRect.h) {
                    running = false;
                }
            }
        }
    }
}

void Game::resetGame() {
    snake = Snake();
    snake.snakeHeadTexture = loadTexture("res/image/snake_head.png");
    snake.snakeBodyTexture = loadTexture("res/image/snake_body.png");
    snake.snakeTailTexture = loadTexture("res/image/snake_tail.png");
    snake.snakeCurveTexture = loadTexture("res/image/snake_curve.png");
    food.spawn(snake.body);
    score = 0;
    running = true;
    paused = false;
    waitingGameOver = false;
}

void Game::update() {
    if (paused) {
        return;
    }

    snake.move();

    if (snake.checkCollision()) {
        waitingGameOver = true;
        if (overMusic) {
            Mix_PlayChannel(-1, overMusic, 0);
        }
        return;
    }

    if (snake.body[0].x == food.position.x && snake.body[0].y == food.position.y) {
        snake.move(true);
        food.spawn(snake.body);
        score += 10;

        if (score > highestScore) {
            highestScore = score;
            saveHighestScore();
        }

        if (eatMusic) {
            Mix_PlayChannel(-1, eatMusic, 0);
        }
    }
}

void Game::loadHighestScore() {
    std::ifstream file(highestScoreFile);
    if (file.is_open()) {
        file >> highestScore;
        file.close();
    }
}

void Game::saveHighestScore() {
    std::ofstream file(highestScoreFile);
    if (file.is_open()) {
        file << highestScore;
        file.close();
    }
}

void Game::updateHighestScore() {
    if (score > highestScore) {
        highestScore = score;
        saveHighestScore();
    }
}

void Game::renderText(const std::string& text, int x, int y, SDL_Color color) {
    static std::string lastText;
    static SDL_Texture* textTexture = nullptr;
    static SDL_Color lastColor = {0, 0, 0, 0};

    if (text != lastText || color.r != lastColor.r || color.g != lastColor.g || color.b != lastColor.b) {
        if (textTexture) {
            SDL_DestroyTexture(textTexture);
        }
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
        if (!textSurface) {
            std::cerr << "Không thể tạo surface từ văn bản! Lỗi: " << TTF_GetError() << std::endl;
            return;
        }
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        lastText = text;
        lastColor = color;
    }

    if (textTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);
        SDL_Rect textRect = {x, y, textWidth, textHeight};
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    }
}

void Game::renderBackground() {
    SDL_SetRenderDrawColor(renderer, 173, 216, 200, 255);
    SDL_Rect topRowsRect = {0, 0, SCREEN_WIDTH, 2 * TILE_SIZE};
    SDL_RenderFillRect(renderer, &topRowsRect);

    SDL_SetRenderDrawColor(renderer, 50, 150, 50, 255);
    SDL_Rect backgroundRect = {0, 2 * TILE_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT - 2 * TILE_SIZE};
    SDL_RenderFillRect(renderer, &backgroundRect);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawLine(renderer, 0, 2 * TILE_SIZE, SCREEN_WIDTH, 2 * TILE_SIZE);

    SDL_SetRenderDrawColor(renderer, 180, 255, 180, 255);
    for (int y = 2 * TILE_SIZE; y < SCREEN_HEIGHT; y += TILE_SIZE * 2) {
        for (int x = 0; x < SCREEN_WIDTH; x += TILE_SIZE * 2) {
            SDL_Rect tile = {x, y, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &tile);
        }
    }
    for (int y = 2 * TILE_SIZE + TILE_SIZE; y < SCREEN_HEIGHT; y += TILE_SIZE * 2) {
        for (int x = TILE_SIZE; x < SCREEN_WIDTH; x += TILE_SIZE * 2) {
            SDL_Rect tile = {x, y, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &tile);
        }
    }
}

void Game::renderMenu() {
    if (menuBackgroundTexture) {
        SDL_RenderCopy(renderer, menuBackgroundTexture, nullptr, nullptr);
    }

    if (mute) {
        if (onMusic) {
            SDL_RenderCopy(renderer, offMusic, nullptr, &muteButtonRect);
        }
    } else {
        if (offMusic) {
            SDL_RenderCopy(renderer, onMusic, nullptr, &muteButtonRect);
        }
    }
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    bool isHoveredPlay = (mouseX >= playButtonRect.x && mouseX <= playButtonRect.x + playButtonRect.w &&
                          mouseY >= playButtonRect.y && mouseY <= playButtonRect.y + playButtonRect.h);

    if (playButtonTexture) {
        if (isHoveredPlay) {
            SDL_SetTextureColorMod(playButtonTexture, 200, 200, 200);
        } else {
            SDL_SetTextureColorMod(playButtonTexture, 255, 255, 255);
        }
        SDL_RenderCopy(renderer, playButtonTexture, nullptr, &playButtonRect);
    }

    SDL_RenderPresent(renderer);
}

void Game::renderScore() {
    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Color textColor = {255, 0, 0, 255};
    renderText(scoreText, SCREEN_WIDTH / 2, 10, textColor);

    std::string highestScoreText = "Highest Score: " + std::to_string(highestScore);
    int textWidth, textHeight;
    TTF_SizeText(font, highestScoreText.c_str(), &textWidth, &textHeight);
    renderText(highestScoreText, SCREEN_WIDTH - textWidth - 10, 10, textColor);
}

void Game::renderGameOver() {
    if (gameOverbackgroundTexture) {
        SDL_RenderCopy(renderer, gameOverbackgroundTexture, nullptr, nullptr);
    }

    SDL_Color scoreTextColor = {255, 0, 0, 255};
    SDL_Rect scoreTextRect = {playagainRect.x, playagainRect.y - 90, 200, 30};

    std::string scoreText = "Your score: " + std::to_string(score);
    renderText(scoreText, scoreTextRect.x, scoreTextRect.y, scoreTextColor);

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    bool isHoveredPlayAgain = (mouseX >= playagainRect.x && mouseX <= playagainRect.x + playagainRect.w &&
                               mouseY >= playagainRect.y && mouseY <= playagainRect.y + playagainRect.h);

    if (playAgainButtonTexture) {
        if (isHoveredPlayAgain) {
            SDL_SetTextureColorMod(playAgainButtonTexture, 200, 200, 200);
        } else {
            SDL_SetTextureColorMod(playAgainButtonTexture, 255, 255, 255);
        }
        SDL_RenderCopy(renderer, playAgainButtonTexture, nullptr, &playagainRect);
    }

    bool isHoveredExitGame = (mouseX >= exitGameButtonRect.x && mouseX <= exitGameButtonRect.x + exitGameButtonRect.w &&
                              mouseY >= exitGameButtonRect.y && mouseY <= exitGameButtonRect.y + exitGameButtonRect.h);

    if (exitGameButtonTexture) {
        if (isHoveredExitGame) {
            SDL_SetTextureColorMod(exitGameButtonTexture, 200, 200, 200);
        } else {
            SDL_SetTextureColorMod(exitGameButtonTexture, 255, 255, 255);
        }
        SDL_RenderCopy(renderer, exitGameButtonTexture, nullptr, &exitGameButtonRect);
    }

    SDL_RenderPresent(renderer);
}

void Game::render() {
    switch (currentState) {
        case GameState::MENU:
            renderMenu();
            break;
        case GameState::PLAYING:
            renderBackground();

            snake.render(renderer, snake.snakeHeadTexture, snake.snakeBodyTexture, snake.snakeTailTexture, snake.snakeCurveTexture);
            food.render(renderer);

            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            bool isHoveredExit = (mouseX >= exitButtonRect.x && mouseX <= exitButtonRect.x + exitButtonRect.w &&
                                  mouseY >= exitButtonRect.y && mouseY <= exitButtonRect.y + exitButtonRect.h);

            if (exitButtonTexture) {
                if (isHoveredExit) {
                    SDL_SetTextureColorMod(exitButtonTexture, 200, 200, 200);
                } else {
                    SDL_SetTextureColorMod(exitButtonTexture, 255, 255, 255);
                }
                SDL_RenderCopy(renderer, exitButtonTexture, nullptr, &exitButtonRect);
            }

            renderScore();

            if (waitingGameOver) {
                renderGameOver();
            }

            if (paused && pauseTexture) {
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
                SDL_Rect overlayRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
                SDL_RenderFillRect(renderer, &overlayRect);

                SDL_Rect pauseRect = { (SCREEN_WIDTH - 400) / 2, (SCREEN_HEIGHT - 200) / 2, 400, 200 };
                SDL_RenderCopy(renderer, pauseTexture, nullptr, &pauseRect);
            }
            break;
    }
    SDL_RenderPresent(renderer);
}

SDL_Texture* Game::loadTexture(const std::string& path) {
    SDL_Texture* texture = nullptr;
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Không thể tải hình ảnh " << path << "! Lỗi: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cerr << "Không thể tạo texture từ hình ảnh! Lỗi: " << SDL_GetError() << std::endl;
    }
    return texture;
}
