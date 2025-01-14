#include "MyGame.h"

void Player::loadTexture(SDL_Renderer* renderer, const char* filePath) {
    SDL_Surface* surface = IMG_Load(filePath);
    if (!surface) {
        SDL_Log("Failed to load player image: %s, %s", filePath, IMG_GetError());
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
    }
}

void Player::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
    else {
        // render a placeholder if no texture is loaded
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

void Player::setPlayerControlled(bool isControlled) {
    isPlayerControlled = isControlled;
}

void Player::setPosition(int x, int y) {
    rect.x = x;
    rect.y = y;
}

void AIPlayer::loadTexture(SDL_Renderer* renderer, const char* filePath) {
    SDL_Surface* surface = IMG_Load(filePath);
    if (!surface) {
        SDL_Log("Failed to load AI Player image: %s, %s", filePath, IMG_GetError());
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
    }
}

void AIPlayer::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
    else {
        // render a placeholder if no texture is loaded
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

void AIPlayer::setPosition(int x, int y) {
    rect.x = x;
    rect.y = y;
}

void Ball::loadTexture(SDL_Renderer* renderer, const char* filePath) {
    SDL_Surface* surface = IMG_Load(filePath);
    if (!surface) {
        SDL_Log("Failed to load Ball image: %s, %s", filePath, IMG_GetError());
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
    }
}

void Ball::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
    else {
        // render a placeholder if no texture is loaded
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

void Ball::setPosition(int x, int y) {
    rect.x = x;
    rect.y = y;
}

void Goal::loadTexture(SDL_Renderer* renderer, const char* filePath) {
    SDL_Surface* surface = IMG_Load(filePath);
    if (!surface) {
        SDL_Log("Failed to load Goal image: %s, %s", filePath, IMG_GetError());
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
    }
}

void Goal::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
    else {
        // Render a placeholder if no texture is loaded
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}


void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {
    if (cmd == "GAME_DATA") {
        if (args.size() == 10) {
            game_data.player1Y = stoi(args.at(0));
            game_data.player1X = stoi(args.at(4));
            game_data.player2Y = stoi(args.at(1));
            game_data.player2X = stoi(args.at(5));
            game_data.ballX = stoi(args.at(2));
            game_data.ballY = stoi(args.at(3));
            game_data.aiTeam1X = stoi(args.at(6));
            game_data.aiTeam1Y = stoi(args.at(7));
            game_data.aiTeam2X = stoi(args.at(8));
            game_data.aiTeam2Y = stoi(args.at(9));
        }
    }
    else if (cmd == "SCORES") {
        if (args.size() == 2) {
            game_data.team1Score = stoi(args.at(0));
            game_data.team2Score = stoi(args.at(1));
        }
    }
    else if (cmd == "PLAYER_JOIN1" || cmd == "PLAYER_JOIN2") {
        if (cmd == "PLAYER_JOIN1") {
            game_data.player = 1;
            player1.setPlayerControlled(true);
        }
        else if (cmd == "PLAYER_JOIN2") {
            game_data.player = 2;
            player2.setPlayerControlled(true);
        }
    }
    else if (cmd == "GAME_ENDED") {
        if (args.size() == 1) {
            if (stoi(args.at(0)) == 1) {
                if (game_data.player == 1) {
                    winner = "You Win!";
                    isWinner = true;
                }
                else if (game_data.player == 2) {
                    winner = "You Lose!";
                    isWinner = false;
                }
            }
            else if (stoi(args.at(0)) == 2) {
                std::cout << "Player 2 wins";
                if (game_data.player == 1) {
                    winner = "You Lose!";
                    isWinner = false;
                }
                else if (game_data.player == 2) {
                    winner = "You Win!";
                    isWinner = true;
                }
            }
        }
        gameOver = true;
    }
    else if (cmd == "CONNECTION_LOST") {
        serverLost = true;
    }
    else if (cmd == "OPPONENT_DISCONNECT") {
        opponentDisconnect = true;
        gameOver = true;
    }
    else if (cmd == "AUDIO") {
        if (args.size() == 1) {
            if (stoi(args.at(0)) == 1) {
                playSoundEffect("kick");
            } else if (stoi(args.at(0)) == 2) {
                playSoundEffect("goal");
            }
        }
    }
    else {
        std::cout << "Received: " << cmd << std::endl;
    }
}

void MyGame::send(std::string message) {
    messages.push_back(message);
}

void MyGame::sendHeartbeat(std::string message) {
    heartbeatMessages.push_back(message);
}

void MyGame::input(SDL_Event& event) {
    switch (event.key.keysym.sym) {
        case SDLK_w:
            if (game_data.player == 1) {
                send(event.type == SDL_KEYDOWN ? "W_DOWN" : "W_UP");
            }
            break;
        case SDLK_s:
            if (game_data.player == 1) {
                send(event.type == SDL_KEYDOWN ? "S_DOWN" : "S_UP");
            }
            break;
        case SDLK_a:
            if (game_data.player == 1) {
                send(event.type == SDL_KEYDOWN ? "A_DOWN" : "A_UP");
            }
            break;
        case SDLK_d:
            if (game_data.player == 1) {
                send(event.type == SDL_KEYDOWN ? "D_DOWN" : "D_UP");
            }
            break;
        case SDLK_i:
            if (game_data.player == 2) {
                send(event.type == SDL_KEYDOWN ? "I_DOWN" : "I_UP");
            }
            break;
        case SDLK_k: 
            if (game_data.player == 2) {
                send(event.type == SDL_KEYDOWN ? "K_DOWN" : "K_UP");
            }
            break;
        case SDLK_j:
            if (game_data.player == 2) {
                send(event.type == SDL_KEYDOWN ? "J_DOWN" : "J_UP");
            }
            break;
        case SDLK_l:
            if (game_data.player == 2) {
                send(event.type == SDL_KEYDOWN ? "L_DOWN" : "L_UP");
            }
            break;
    }
}

void MyGame::update() {
    if (gameOver == false) {
        player1.setPosition(game_data.player1X, game_data.player1Y);

        aiTeam1.setPosition(game_data.aiTeam1X, game_data.aiTeam1Y);

        player2.setPosition(game_data.player2X, game_data.player2Y);

        aiTeam2.setPosition(game_data.aiTeam2X, game_data.aiTeam2Y);

        ball.setPosition(game_data.ballX, game_data.ballY);
        team1Score = game_data.team1Score;
        team2Score = game_data.team2Score;

        prepareHeartbeat();

    }
}

// Text Function
void MyGame::renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text,
    int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Failed to create surface: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect destRect = { x, y, surface->w, surface->h };
    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    SDL_DestroyTexture(texture);
}

void MyGame::render(SDL_Renderer* renderer) {
    // Colours
    SDL_Color navy = { 0, 0, 128, 255 };
    SDL_Color black = { 0, 0, 0, 255 };
    SDL_Color white = { 255, 255, 255, 255 };

    // Render the screen
    SDL_RenderClear(renderer);

    // Define base colors for the "pitch" and lighter/darker shades
    SDL_Color baseGreen = { 34, 139, 34, 255 };
    SDL_Color lighterGreen = { 50, 205, 50, 255 };
    SDL_Color darkerGreen = { 0, 100, 0, 255 };

    for (int y = 0; y < 720; y++) {
        // Calculate the interpolation factor based on the y-coordinate
        float interpolationFactor = (float)(y % 40) / 40.0f; // Change 40 for larger/smaller stripes

        // Alternate between lighter and darker green
        SDL_Color stripeColor;
        if ((y / 40) % 2 == 0) {
            // Lighter green stripe
            stripeColor.r = (Uint8)(baseGreen.r + interpolationFactor * (lighterGreen.r - baseGreen.r));
            stripeColor.g = (Uint8)(baseGreen.g + interpolationFactor * (lighterGreen.g - baseGreen.g));
            stripeColor.b = (Uint8)(baseGreen.b + interpolationFactor * (lighterGreen.b - baseGreen.b));
        }
        else {
            // Darker green stripe
            stripeColor.r = (Uint8)(baseGreen.r + interpolationFactor * (darkerGreen.r - baseGreen.r));
            stripeColor.g = (Uint8)(baseGreen.g + interpolationFactor * (darkerGreen.g - baseGreen.g));
            stripeColor.b = (Uint8)(baseGreen.b + interpolationFactor * (darkerGreen.b - baseGreen.b));
        }

        // Set the color for the current stripe
        SDL_SetRenderDrawColor(renderer, stripeColor.r, stripeColor.g, stripeColor.b, 255);

        // Draw the stripe
        SDL_Rect lineRect = { 0, y, 1280, 1 };
        SDL_RenderFillRect(renderer, &lineRect);
    }

    // Render Images
    renderBackground(renderer);
    renderScoreboard(renderer);
    renderIndicator(renderer);

    // Text Handling
        // Set up font and score text
    TTF_Font* font = TTF_OpenFont("assets/fonts/NeueHaasDisplay.ttf", 22);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }

    if (serverLost == false) {
        if (gameOver == false) {
            // Team 1
            player1.render(renderer);
            aiTeam1.render(renderer);

            // Team 2
            player2.render(renderer);
            aiTeam2.render(renderer);

            // Ball
            ball.render(renderer);

            // Other Objects 
            goalLine1.render(renderer);
            goalLine2.render(renderer);

            std::string scoreText = "Team 1 " + std::to_string(team1Score) + " - " + std::to_string(team2Score) + " Team 2";

            // Render the score & team indicator
            renderText(renderer, font, scoreText, 60, 30, white);
            std::string team = "Controlling: Team " + std::to_string(game_data.player);
            renderText(renderer, font, team, 1000, 30, white);
        }
        else if (gameOver == true && opponentDisconnect == false && isWinner == true) {
            SDL_RenderClear(renderer);
            for (int y = 0; y < 720; y++) {
                float interpolationFactor = (float)y / 720;

                Uint8 r = (Uint8)(128 + interpolationFactor * (255 - 128));
                Uint8 g = (Uint8)(0 + interpolationFactor * (215 - 0));
                Uint8 b = (Uint8)(128 + interpolationFactor * (0 - 128));

                SDL_SetRenderDrawColor(renderer, r, g, b, 255);

                SDL_Rect lineRect = { 0, y, 1280, 1 }; // Full-width rectangle for each line
                SDL_RenderFillRect(renderer, &lineRect);
            }
            renderText(renderer, font, winner, 550, 300, white);
        }
        else if (gameOver == true && opponentDisconnect == false && isWinner == false) {
            SDL_RenderClear(renderer);
            for (int y = 0; y < 720; y++) {
                float interpolationFactor = (float)y / 720;

                Uint8 r = (Uint8)(139 + interpolationFactor * (64 - 139));
                Uint8 g = (Uint8)(0 + interpolationFactor * (64 - 0));
                Uint8 b = (Uint8)(0 + interpolationFactor * (64 - 0));

                SDL_SetRenderDrawColor(renderer, r, g, b, 255);

                SDL_Rect lineRect = { 0, y, 1280, 1 }; // Full-width rectangle for each line
                SDL_RenderFillRect(renderer, &lineRect);
            }
            renderText(renderer, font, winner, 550, 300, white);
        }
        else if (opponentDisconnect = true && gameOver == true) {
            SDL_RenderClear(renderer);
            for (int y = 0; y < 720; y++) {
                float interpolationFactor = (float)y / 720;

                Uint8 r = (Uint8)(255 + interpolationFactor * (25 - 255));
                Uint8 g = (Uint8)(215 + interpolationFactor * (25 - 215));
                Uint8 b = (Uint8)(0 + interpolationFactor * (112 - 0));

                SDL_SetRenderDrawColor(renderer, r, g, b, 255);

                SDL_Rect lineRect = { 0, y, 1280, 1 }; // Full-width rectangle for each line
                SDL_RenderFillRect(renderer, &lineRect);
            }
            renderText(renderer, font, "Opponent Disconnected, you win!", 450, 300, white);
        }
    }
    else if (serverLost == true) {
        SDL_RenderClear(renderer);
        for (int y = 0; y < 720; y++) {
            float interpolationFactor = (float)y / 720;

            Uint8 r = (Uint8)(139 + interpolationFactor * (48 - 139));
            Uint8 g = (Uint8)(0 + interpolationFactor * (0 - 0));
            Uint8 b = (Uint8)(0 + interpolationFactor * (48 - 0));

            SDL_SetRenderDrawColor(renderer, r, g, b, 255);

            SDL_Rect lineRect = { 0, y, 1280, 1 };
            SDL_RenderFillRect(renderer, &lineRect);
        }
        renderText(renderer, font, "ERROR: Connection to server lost!", 450, 300, white);
    }

    TTF_CloseFont(font);
    
}

// Image Handling
// Function to load the background texture
void MyGame::loadBackgroundTexture(SDL_Renderer* renderer, const char* filePath) {
    // Load image as a texture
    backgroundTexture = IMG_LoadTexture(renderer, filePath);
    if (backgroundTexture == nullptr) {
        printf("Failed to load background texture: %s\n", SDL_GetError());
        return;
    }

    // Get the texture width and height to set the destination rect
    int width, height;
    SDL_QueryTexture(backgroundTexture, NULL, NULL, &width, &height);
    backgroundRect = { 50, 80, width, height };  // Set rect to the size of the texture
}

void MyGame::loadPlayerTextures(SDL_Renderer* renderer) {
    // Load textures for each player
    player1.loadTexture(renderer, "assets/textures/Team1Player.png");
    player2.loadTexture(renderer, "assets/textures/Team2Player.png");
    aiTeam1.loadTexture(renderer, "assets/textures/Team1AI.png");
    aiTeam2.loadTexture(renderer, "assets/textures/Team2AI.png");
    ball.loadTexture(renderer, "assets/textures/BallBrown2.png");
    goalLine1.loadTexture(renderer, "assets/textures/GoalLeft.jpg");
    goalLine2.loadTexture(renderer, "assets/textures/GoalRight.jpg");
}

// Function to render the background texture
void MyGame::renderBackground(SDL_Renderer* renderer) {
    if (backgroundTexture != nullptr) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);
    }
}

// Function to cleanup background texture when done
void MyGame::cleanupBackground() {
    if (backgroundTexture != nullptr) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = nullptr;
    }
}

void MyGame::loadScoreboardContainer(SDL_Renderer* renderer, const char* filePath) {
    scoreboardTexture = IMG_LoadTexture(renderer, filePath);
    if (scoreboardTexture == nullptr) {
        printf("Failed to load scoreboard texture: %s\n", SDL_GetError());
        return;
    }

    int width, height;
    SDL_QueryTexture(scoreboardTexture, NULL, NULL, &width, &height);
    scoreboardRect = { 50, 20, width, height};  // Set rect to the size of the texture
}

void MyGame::renderScoreboard(SDL_Renderer* renderer) {
    if (scoreboardTexture != nullptr) {
        SDL_RenderCopy(renderer, scoreboardTexture, NULL, &scoreboardRect);
    }
}

void MyGame::loadIndicatorContainer(SDL_Renderer* renderer, const char* filePath) {
    indicatorTexture = IMG_LoadTexture(renderer, filePath);
    if (indicatorTexture == nullptr) {
        printf("Failed to load scoreboard texture: %s\n", SDL_GetError());
        return;
    }

    int width, height;
    SDL_QueryTexture(indicatorTexture, NULL, NULL, &width, &height);
    indicatorRect = { 985, 20, width, height };  // Set rect to the size of the texture
}

void MyGame::renderIndicator(SDL_Renderer* renderer) {
    if (indicatorTexture != nullptr) {
        SDL_RenderCopy(renderer, indicatorTexture, NULL, &indicatorRect);
    }
}

// Send heartbeat to server
void MyGame::prepareHeartbeat() {
    if (SDL_GetTicks() > nextSendTime) {
        sendHeartbeat("HEARTBEAT, " + std::to_string(game_data.player));
        nextSendTime = SDL_GetTicks() + 1000;
    }
}

// Audio
void MyGame::loadAudio() {
    goalSound = Mix_LoadWAV("assets/audio/GoalSound.wav");
    if (!goalSound) {
        std::cerr << "Failed to load goal sound: " << Mix_GetError() << std::endl;
    }

    kickSound = Mix_LoadWAV("assets/audio/KickSound.mp3");
    if (!kickSound) {
        std::cerr << "Failed to load kick sound: " << Mix_GetError() << std::endl;
    }
}

void MyGame::playSoundEffect(const std::string& soundName) {
    Mix_Chunk* soundToPlay = nullptr;

    if (soundName == "goal") {
        soundToPlay = goalSound;
    }
    else if (soundName == "kick") {
        soundToPlay = kickSound;
    }

    if (soundToPlay) {
        if (Mix_PlayChannel(-1, soundToPlay, 0) == -1) {
            std::cerr << "Failed to play audio: " << Mix_GetError() << std::endl;
        }
    }
}