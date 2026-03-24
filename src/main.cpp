#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

#include <iostream>
#include <string>
#include <vector>

TTF_Font* font = nullptr;
SDL_Texture* ghostTexture = nullptr;

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static bool running = true;

enum class GameState {
    MENU,
    STARTED
};

struct MenuState {
    std::vector<std::string> characters = {
        "Ghost",
        "Witch",
        "Vampire",
        "Corpse",
        "Pumpking",
        "Mummy",
        "Skeleton",
        "Drosera"
    };

    int selectedCharacter = 0;
    int enemyCount = 1;
};

static GameState gameState = GameState::MENU;
static MenuState menu;

void DrawText(const std::string& text, int x, int y) {
    if (!font) return;

    SDL_Color color = {255, 255, 255, 255};

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), 0, color);
    if (!surface) {
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_DestroySurface(surface);
        return;
    }

    SDL_FRect rect;
    rect.x = static_cast<float>(x);
    rect.y = static_cast<float>(y);
    rect.w = static_cast<float>(surface->w);
    rect.h = static_cast<float>(surface->h);

    SDL_RenderTexture(renderer, texture, nullptr, &rect);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

void UpdateWindowTitle() {
    std::string title =
        "Monster Battle | Character: " +
        menu.characters[menu.selectedCharacter] +
        " | Enemies: " +
        std::to_string(menu.enemyCount) +
        " | Up/Down=Character Left/Right=Enemies Enter=Start Esc=Quit";

    SDL_SetWindowTitle(window, title.c_str());
}

void PrintMenuState() {
    std::cout << "Selected character: "
              << menu.characters[menu.selectedCharacter]
              << " | Enemy count: "
              << menu.enemyCount
              << '\n';
}

bool Init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
        return false;
    }

    if (!TTF_Init()) {
        std::cerr << "TTF_Init failed: " << SDL_GetError() << '\n';
        SDL_Quit();
        return false;
    }

    window = SDL_CreateWindow("Monster Battle", 1000, 700, 0);
    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << '\n';
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    ghostTexture = IMG_LoadTexture(renderer, "res/ghost.jpg");
    if (!ghostTexture) {
        std::cerr << "IMG_LoadTexture failed: " << SDL_GetError() << '\n';
    }

    font = TTF_OpenFont("font/ARIAL.TTF", 24);
    if (!font) {
        std::cerr << "TTF_OpenFont failed: " << SDL_GetError() << '\n';

        if (ghostTexture) {
            SDL_DestroyTexture(ghostTexture);
            ghostTexture = nullptr;
        }

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    UpdateWindowTitle();
    PrintMenuState();
    return true;
}

void HandleMenuKey(SDL_Keycode key) {
    switch (key) {
        case SDLK_UP:
            menu.selectedCharacter--;
            if (menu.selectedCharacter < 0) {
                menu.selectedCharacter = static_cast<int>(menu.characters.size()) - 1;
            }
            UpdateWindowTitle();
            PrintMenuState();
            break;

        case SDLK_DOWN:
            menu.selectedCharacter++;
            if (menu.selectedCharacter >= static_cast<int>(menu.characters.size())) {
                menu.selectedCharacter = 0;
            }
            UpdateWindowTitle();
            PrintMenuState();
            break;

        case SDLK_LEFT:
            if (menu.enemyCount > 1) {
                menu.enemyCount--;
                UpdateWindowTitle();
                PrintMenuState();
            }
            break;

        case SDLK_RIGHT:
            if (menu.enemyCount < 5) {
                menu.enemyCount++;
                UpdateWindowTitle();
                PrintMenuState();
            }
            break;

        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            gameState = GameState::STARTED;
            std::cout << "\n=== GAME STARTED ===\n";
            std::cout << "Player chose: " << menu.characters[menu.selectedCharacter] << '\n';
            std::cout << "Opponent count: " << menu.enemyCount << '\n';
            SDL_SetWindowTitle(window, "Monster Battle | Battle screen placeholder | ESC to quit");
            break;

        case SDLK_ESCAPE:
            running = false;
            break;

        default:
            break;
    }
}

void HandleStartedKey(SDL_Keycode key) {
    if (key == SDLK_ESCAPE) {
        running = false;
    }
}

void HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running = false;
        }

        if (event.type == SDL_EVENT_KEY_DOWN) {
            SDL_Keycode key = event.key.key;

            if (gameState == GameState::MENU) {
                HandleMenuKey(key);
            } else if (gameState == GameState::STARTED) {
                HandleStartedKey(key);
            }
        }
    }
}

void RenderMenu() {
    SDL_SetRenderDrawColor(renderer, 25, 25, 35, 255);
    SDL_RenderClear(renderer);

    DrawText("Choose Your Monster", 80, 15);
    DrawText("Opponents", 600, 100);

    for (int i = 0; i < static_cast<int>(menu.characters.size()); i++) {
        SDL_FRect rect = {80.0f, 60.0f + i * 70.0f, 300.0f, 50.0f};

        if (i == menu.selectedCharacter) {
            SDL_SetRenderDrawColor(renderer, 180, 120, 220, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 70, 70, 90, 255);
        }

        SDL_RenderFillRect(renderer, &rect);
        DrawText(menu.characters[i], 100, 70 + i * 70);
    }

    for (int i = 0; i < 5; i++) {
        SDL_FRect rect = {600.0f, 150.0f + i * 80.0f, 140.0f, 50.0f};

        if (i + 1 == menu.enemyCount) {
            SDL_SetRenderDrawColor(renderer, 220, 120, 120, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 90, 70, 70, 255);
        }

        SDL_RenderFillRect(renderer, &rect);
        DrawText(std::to_string(i + 1), 660, 160 + i * 80);
    }

    SDL_FRect bottomBar = {60.0f, 620.0f, 880.0f, 40.0f};
    SDL_SetRenderDrawColor(renderer, 80, 120, 80, 255);
    SDL_RenderFillRect(renderer, &bottomBar);
    DrawText("ENTER = Start   ESC = Quit", 320, 628);

    if (menu.characters[menu.selectedCharacter] == "Ghost" && ghostTexture) {
        SDL_FRect portraitRect = {420.0f, 220.0f, 140.0f, 140.0f};
        SDL_RenderTexture(renderer, ghostTexture, nullptr, &portraitRect);
        DrawText("Ghost Preview", 420, 190);
    }

    SDL_RenderPresent(renderer);
}

void RenderStarted() {
    SDL_SetRenderDrawColor(renderer, 40, 15, 15, 255);
    SDL_RenderClear(renderer);

    DrawText("Battle screen placeholder", 320, 50);
    DrawText("Player: " + menu.characters[menu.selectedCharacter], 80, 220);
    DrawText("Enemies: " + std::to_string(menu.enemyCount), 80, 260);
    DrawText("ESC = Quit", 80, 300);

    SDL_FRect playerBox = {80.0f, 350.0f, 220.0f, 220.0f};
    SDL_SetRenderDrawColor(renderer, 80, 160, 220, 255);
    SDL_RenderFillRect(renderer, &playerBox);

    for (int i = 0; i < menu.enemyCount; i++) {
        SDL_FRect enemyBox = {500.0f + i * 90.0f, 120.0f, 70.0f, 70.0f};
        SDL_SetRenderDrawColor(renderer, 220, 80, 80, 255);
        SDL_RenderFillRect(renderer, &enemyBox);
    }

    SDL_RenderPresent(renderer);
}

void Render() {
    if (gameState == GameState::MENU) {
        RenderMenu();
    } else if (gameState == GameState::STARTED) {
        RenderStarted();
    }
}

void CleanUp() {
    if (ghostTexture) {
        SDL_DestroyTexture(ghostTexture);
        ghostTexture = nullptr;
    }

    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    TTF_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!Init()) {
        return 1;
    }

    while (running) {
        HandleEvents();
        Render();
        SDL_Delay(16);
    }

    CleanUp();
    return 0;
}