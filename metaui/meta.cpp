#include "metacore/MetaEngine.h"
#include <format>
#include <iostream>
#include <SDL2/SDL.h>

namespace {

constexpr auto width = 800;
constexpr auto height = 600;

[[noreturn]] void throw_sdl_error()
{
    throw std::runtime_error{SDL_GetError()};
}

void render_gamestate(SDL_Renderer &renderer, metacore::GameState const &state)
{
    if (SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255) != 0) {
        throw_sdl_error();
    }
    if (SDL_RenderClear(&renderer) != 0) {
        throw_sdl_error();
    }
    auto const &player = state.player_position;
    auto r = SDL_Rect{
        static_cast<int>(player.x), static_cast<int>(player.y), 50, 50};
    if (SDL_SetRenderDrawColor(&renderer, 0, 0, 255, 255) != 0) {
        throw_sdl_error();
    }
    if (SDL_RenderFillRect(&renderer, &r) != 0) {
        throw_sdl_error();
    }
    SDL_RenderPresent(&renderer);
}

} // namespace

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    try {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            throw_sdl_error();
        }
        window = SDL_CreateWindow(
            "SDL Example",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            0);
        if (window == nullptr) {
            throw_sdl_error();
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr) {
            throw_sdl_error();
        }
        auto engine = metacore::MetaEngine{};
        while (true) {
            render_gamestate(*renderer, engine.calculate_state());
            SDL_Delay(50);
        }
    } catch (std::exception const &error) {
        std::cerr << error.what() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}