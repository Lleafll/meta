#include "metacore/MetaEngine.h"
#include <iostream>
#include <SDL2/SDL.h>

namespace {

constexpr auto width = 800;
constexpr auto height = 600;

[[noreturn]] void throw_sdl_error()
{
    throw std::runtime_error{SDL_GetError()};
}

struct ScreenPosition final {
    int x;
    int y;
};

ScreenPosition
world_position_to_screen_position(metacore::Position const& world_position)
{
    auto const x = world_position.x + width / 2;
    auto const y = world_position.y + height / 2;
    return {x, y};
}

template <Uint8 red, Uint8 green, Uint8 blue>
void render_at_position(
    SDL_Renderer& renderer, metacore::Position const& world_position)
{
    auto const screen_position =
        world_position_to_screen_position(world_position);
    auto r = SDL_Rect{screen_position.x, screen_position.y, 50, 50};
    if (SDL_SetRenderDrawColor(&renderer, red, green, blue, 255) != 0) {
        throw_sdl_error();
    }
    if (SDL_RenderFillRect(&renderer, &r) != 0) {
        throw_sdl_error();
    }
}

void render_player(SDL_Renderer& renderer, metacore::Position const& position)
{
    render_at_position<0, 0, 255>(renderer, position);
}

void render_upgrade(SDL_Renderer& renderer, metacore::Position const& position)
{
    render_at_position<255, 255, 255>(renderer, position);
}

void render_gamestate(SDL_Renderer& renderer, metacore::GameState const& state)
{
    if (SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255) != 0) {
        throw_sdl_error();
    }
    if (SDL_RenderClear(&renderer) != 0) {
        throw_sdl_error();
    }
    render_player(renderer, state.player_position);
    render_upgrade(renderer, state.upgrade);
    SDL_RenderPresent(&renderer);
}

enum class EscapePressed : bool { Yes, No };

EscapePressed read_and_pass_input(metacore::MetaEngine& engine)
{
    auto esc_pressed = EscapePressed::No;
    auto event = SDL_Event{};
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_RIGHT:
                engine.input_right();
                break;
            case SDLK_ESCAPE:
                esc_pressed = EscapePressed::Yes;
                break;
            }
        }
    }
    return esc_pressed;
}

} // namespace

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    SDL_Window* window = nullptr;
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
        auto* const renderer =
            SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr) {
            throw_sdl_error();
        }
        auto engine = metacore::MetaEngine{};
        while (true) {
            render_gamestate(*renderer, engine.calculate_state());
            if (read_and_pass_input(engine) == EscapePressed::Yes) {
                break;
            }
            SDL_Delay(50);
        }
    } catch (std::exception const& error) {
        std::cerr << error.what() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}