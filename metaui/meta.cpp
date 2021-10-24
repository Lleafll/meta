#include "metacore/GameState.h"
#include "metacore/MetaEngine.h"
#include <format>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace {

struct ScreenPosition final {
    int x;
    int y;
};

constexpr auto screen_width = 800;
constexpr auto screen_height = 600;
constexpr auto first_choice_position = ScreenPosition{400, 300};
constexpr auto second_choice_position = ScreenPosition{400, 350};

[[noreturn]] void throw_sdl_error()
{
    throw std::runtime_error{SDL_GetError()};
}

ScreenPosition
world_position_to_screen_position(metacore::Position const& world_position)
{
    auto const x = world_position.x + screen_width / 2;
    auto const y = screen_height / 2 - world_position.y;
    return {x, y};
}

template<Uint8 red, Uint8 green, Uint8 blue, int size = 50>
void render_rectangle_at_position(
    SDL_Renderer& renderer, metacore::Position const& world_position)
{
    auto const screen_position =
        world_position_to_screen_position(world_position);
    auto r = SDL_Rect{
        screen_position.x - size / 2, screen_position.y - size / 2, size, size};
    if (SDL_SetRenderDrawColor(&renderer, red, green, blue, 255) != 0) {
        throw_sdl_error();
    }
    if (SDL_RenderFillRect(&renderer, &r) != 0) {
        throw_sdl_error();
    }
}

void render_enemies(
    SDL_Renderer& renderer, std::vector<metacore::Position> const& positions)
{
    for (auto const& position : positions) {
        render_rectangle_at_position<255, 0, 0, 50>(renderer, position);
    }
}

void render_slash_attack(
    SDL_Renderer& renderer, metacore::Position const& position)
{
    render_rectangle_at_position<255, 255, 0, 100>(renderer, position);
}

void render_player(SDL_Renderer& renderer, metacore::Position const& position)
{
    render_rectangle_at_position<0, 0, 255>(renderer, position);
}

void render_upgrade(SDL_Renderer& renderer, metacore::Position const& position)
{
    render_rectangle_at_position<255, 255, 255>(renderer, position);
}

void render_text_at_position(
    SDL_Renderer& renderer,
    TTF_Font& font,
    std::string const& text,
    ScreenPosition const& position)
{
    auto const color = SDL_Color{255, 255, 255};
    auto* const surface = TTF_RenderText_Solid(&font, text.c_str(), color);
    auto* const texture = SDL_CreateTextureFromSurface(&renderer, surface);
    auto width = 0;
    auto height = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    auto dstrect = SDL_Rect{position.x, position.y, width, height};
    SDL_RenderCopy(&renderer, texture, nullptr, &dstrect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

constexpr char const* to_string(metacore::PickupUpgrade const upgrade)
{
    switch (upgrade) {
        case metacore::PickupUpgrade::Slash:
            return "Slash";
        case metacore::PickupUpgrade::Shoot:
            return "Shoot";
    }
    throw std::runtime_error{std::format(
        "{} not handled in {}", static_cast<int>(upgrade), __func__)};
}

void render_upgrade_choices(
    SDL_Renderer& renderer,
    TTF_Font& font,
    metacore::UpgradeChoices const& choices)
{
    render_text_at_position(
        renderer,
        font,
        std::format("1: {}", to_string(choices.first)),
        first_choice_position);
    render_text_at_position(
        renderer,
        font,
        std::format("2: {}", to_string(choices.second)),
        second_choice_position);
}

void render_gamestate(
    SDL_Renderer& renderer, metacore::GameState const& state, TTF_Font& font)
{
    if (SDL_SetRenderDrawColor(&renderer, 0, 0, 0, 255) != 0) {
        throw_sdl_error();
    }
    if (SDL_RenderClear(&renderer) != 0) {
        throw_sdl_error();
    }
    render_enemies(renderer, state.enemy_positions);
    if (state.slash_attack) {
        render_slash_attack(renderer, state.player_position);
    }
    render_player(renderer, state.player_position);
    if (state.upgrade_position.has_value()) {
        render_upgrade(renderer, *state.upgrade_position);
    }
    if (state.upgrade_choices.has_value()) {
        render_upgrade_choices(renderer, font, *state.upgrade_choices);
    }
    SDL_RenderPresent(&renderer);
}

enum class CloseRequested : bool { Yes, No };

CloseRequested read_and_pass_input(metacore::MetaEngine& engine)
{
    auto close_requested = CloseRequested::No;
    auto event = SDL_Event{};
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_d:
                    case SDLK_RIGHT:
                        engine.input_right();
                        break;
                    case SDLK_a:
                    case SDLK_LEFT:
                        engine.input_left();
                        break;
                    case SDLK_w:
                    case SDLK_UP:
                        engine.input_up();
                        break;
                    case SDLK_s:
                    case SDLK_DOWN:
                        engine.input_down();
                        break;
                    case SDLK_1:
                        engine.select_first_upgrade();
                        break;
                    case SDLK_2:
                        engine.select_second_upgrade();
                        break;
                    case SDLK_SPACE:
                        engine.input_attack();
                        break;
                    case SDLK_ESCAPE:
                        close_requested = CloseRequested::Yes;
                        break;
                }
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                    close_requested = CloseRequested::Yes;
                }
                break;
        }
    }
    return close_requested;
}

} // namespace

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    SDL_Window* window = nullptr;
    try {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            throw_sdl_error();
        }
        if (TTF_Init() != 0) {
            throw_sdl_error();
        }
        window = SDL_CreateWindow(
            "SDL Example",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            screen_width,
            screen_height,
            0);
        if (window == nullptr) {
            throw_sdl_error();
        }
        auto* const renderer =
            SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr) {
            throw_sdl_error();
        }
        auto* const font =
            TTF_OpenFont("Pixel_Berry_08_84_Ltd.Edition.TTF", 24);
        if (font == nullptr) {
            throw_sdl_error();
        }
        auto engine = metacore::MetaEngine{};
        while (true) {
            render_gamestate(*renderer, engine.calculate_state(), *font);
            if (read_and_pass_input(engine) == CloseRequested::Yes) {
                break;
            }
            SDL_Delay(50);
        }
        TTF_CloseFont(font);
    } catch (std::exception const& error) {
        std::cerr << error.what() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}