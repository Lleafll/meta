#include "metacore/GameState.h"
#include "metacore/MetaEngine.h"
#include <format>
#include <iostream>
#include <ranges>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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
constexpr auto game_progress_position = ScreenPosition{300, 200};

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

template<Uint8 red, Uint8 green, Uint8 blue>
void render_rectangle_at_position(
    SDL_Renderer& renderer,
    metacore::Position const& world_position,
    int const width = 50,
    int const height = 50)
{
    auto const screen_position =
        world_position_to_screen_position(world_position);
    auto r = SDL_Rect{
        screen_position.x - width / 2 + 25,
        screen_position.y - height / 2 - 25,
        width,
        height};
    if (SDL_SetRenderDrawColor(&renderer, red, green, blue, 255) != 0) {
        throw_sdl_error();
    }
    if (SDL_RenderFillRect(&renderer, &r) != 0) {
        throw_sdl_error();
    }
}
void render_texture_at_position(
    SDL_Renderer& renderer,
    metacore::Position const& position,
    SDL_Texture* const texture)
{
    auto const screen_position = world_position_to_screen_position(position);
    auto dstrect = SDL_Rect{screen_position.x, screen_position.y - 50, 50, 50};
    if (SDL_RenderCopy(&renderer, texture, nullptr, &dstrect) != 0) {
        throw_sdl_error();
    }
}

void render_character_texture_at_position(
    SDL_Renderer& renderer,
    metacore::Position const& position,
    metacore::CharacterTexture const character_texture)
{
    auto* const image = [character_texture]() -> SDL_Surface* {
        static auto* const alien_texture = IMG_Load("alien.png");
        static auto* const car_texture = IMG_Load("car.png");
        static auto* const knight_texture = IMG_Load("knight.png");
        static auto* const robot_texture = IMG_Load("robot.png");
        switch (character_texture) {
            case metacore::CharacterTexture::None:
                return nullptr;
            case metacore::CharacterTexture::Car:
                return car_texture;
            case metacore::CharacterTexture::Knight:
                return knight_texture;
            case metacore::CharacterTexture::Alien:
                return alien_texture;
            case metacore::CharacterTexture::Robot:
                return robot_texture;
        }
        return nullptr;
    }();
    if (image == nullptr) {
        throw_sdl_error();
    }
    auto* const texture = SDL_CreateTextureFromSurface(&renderer, image);
    if (texture == nullptr) {
        throw_sdl_error();
    }
    render_texture_at_position(renderer, position, texture);
    SDL_DestroyTexture(texture);
}

template<Uint8 red, Uint8 green, Uint8 blue>
void render_at_position(
    SDL_Renderer& renderer,
    metacore::Position const& position,
    metacore::CharacterTexture const texture)
{
    if (texture == metacore::CharacterTexture::None) {
        render_rectangle_at_position<red, green, blue>(renderer, position);
    } else {
        render_character_texture_at_position(renderer, position, texture);
    }
}

void render_enemies(
    SDL_Renderer& renderer,
    std::vector<metacore::Position> const& positions,
    metacore::CharacterTexture const texture)
{
    for (auto const& position : positions) {
        render_at_position<255, 0, 0>(renderer, position, texture);
    }
}

void render_slash_attack(
    SDL_Renderer& renderer,
    metacore::Position position,
    metacore::Orientation const orientation)
{
    constexpr auto slash_reach = 100;
    int height = 0;
    int width = 0;
    switch (orientation) {
        case metacore::Orientation::Up:
            height = slash_reach / 2;
            width = slash_reach;
            position.y += height / 2;
            break;
        case metacore::Orientation::Down:
            height = slash_reach / 2;
            width = slash_reach;
            position.y -= height / 2;
            break;
        case metacore::Orientation::Left:
            height = slash_reach;
            width = slash_reach / 2;
            position.x -= width / 2;
            break;
        case metacore::Orientation::Right:
            height = slash_reach;
            width = slash_reach / 2;
            position.x += width / 2;
            break;
    }
    render_rectangle_at_position<255, 255, 0>(
        renderer, position, width, height);
}

void render_player(
    SDL_Renderer& renderer,
    metacore::Position const& position,
    metacore::CharacterTexture const texture)
{
    render_at_position<0, 0, 255>(renderer, position, texture);
}

void render_upgrade(SDL_Renderer& renderer, metacore::Position const& position)
{
    render_rectangle_at_position<255, 255, 255>(renderer, position);
}

template<ScreenPosition position>
void render_text_at_position(
    SDL_Renderer& renderer, TTF_Font& font, std::string const& text)
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
        case metacore::PickupUpgrade::Dungeon:
            return "Dungeon";
        case metacore::PickupUpgrade::OpenWorld:
            return "Open World";
        case metacore::PickupUpgrade::Car:
            return "Car Player";
        case metacore::PickupUpgrade::Knight:
            return "Knight Player";
        case metacore::PickupUpgrade::Alien:
            return "Alien Enemies";
        case metacore::PickupUpgrade::Robot:
            return "Robot Enemies";
        case metacore::PickupUpgrade::SpaceStation:
            return "Space Station";
        case metacore::PickupUpgrade::Castle:
            return "Castle";
        case metacore::PickupUpgrade::RealTime:
            return "Real Time";
        case metacore::PickupUpgrade::Superhot:
            return "SUPERHOT";
    }
    throw std::runtime_error{std::format(
        "{} not handled in {}", static_cast<int>(upgrade), __func__)};
}

void render_upgrade_choices(
    SDL_Renderer& renderer,
    TTF_Font& font,
    metacore::UpgradeChoices const& choices)
{
    render_text_at_position<first_choice_position>(
        renderer, font, std::format("1: {}", to_string(choices.first)));
    render_text_at_position<second_choice_position>(
        renderer, font, std::format("2: {}", to_string(choices.second)));
}

void render_game_progress(
    SDL_Renderer& renderer,
    TTF_Font& font,
    metacore::GameProgress const progress)
{
    if (progress == metacore::GameProgress::Lost) {
        render_text_at_position<game_progress_position>(
            renderer, font, "Game Over! (Restart: F5)");
    }
}

void render_projectiles(
    SDL_Renderer& renderer, std::vector<metacore::Position> const& projectiles)
{
    for (auto const& position : projectiles) {
        render_rectangle_at_position<255, 255, 255>(renderer, position, 10, 10);
    }
}

void render_tile(
    SDL_Renderer& renderer,
    metacore::Tile const& tile,
    metacore::EnvironmentTexture const environment_texture)
{
    auto* const image = [environment_texture, &tile]() -> SDL_Surface* {
        switch (environment_texture) {
            case metacore::EnvironmentTexture::None:
                return nullptr;
            case metacore::EnvironmentTexture::SpaceStation:
                switch (tile.type) {
                    case metacore::TileType::Obstacle:
                        static auto* const obstacle =
                            IMG_Load("spacestationobstacle.png");
                        return obstacle;
                    case metacore::TileType::Stairs:
                        static auto* const stairs =
                            IMG_Load("spacestationstairs.png");
                        return stairs;
                }
            case metacore::EnvironmentTexture::Castle:
                switch (tile.type) {
                    case metacore::TileType::Obstacle:
                        static auto* const obstacle =
                            IMG_Load("castleobstacle.png");
                        return obstacle;
                    case metacore::TileType::Stairs:
                        static auto* const stairs =
                            IMG_Load("castlestairs.png");
                        return stairs;
                }
        }
        return nullptr;
    }();
    if (image == nullptr) {
        throw_sdl_error();
    }
    auto* const texture = SDL_CreateTextureFromSurface(&renderer, image);
    if (texture == nullptr) {
        throw_sdl_error();
    }
    render_texture_at_position(renderer, tile.position, texture);
    SDL_DestroyTexture(texture);
}

void render_tiles(
    SDL_Renderer& renderer,
    std::vector<metacore::Tile> const& tiles,
    metacore::EnvironmentTexture const texture)
{
    for (auto const& tile : tiles) {
        if (texture == metacore::EnvironmentTexture::None) {
            switch (tile.type) {
                case metacore::TileType::Obstacle:
                    render_rectangle_at_position<64, 64, 64>(
                        renderer, tile.position);
                    break;
                case metacore::TileType::Stairs:
                    render_rectangle_at_position<123, 63, 0>(
                        renderer, tile.position);
                    break;
            }
        } else {
            render_tile(renderer, tile, texture);
        }
    }
}

void render_ground(
    SDL_Renderer& renderer,
    metacore::LayoutBounds const& bounds,
    metacore::EnvironmentTexture const environment_texture)
{
    if (environment_texture == metacore::EnvironmentTexture::None) {
        return;
    }
    auto* const image = [environment_texture]() -> SDL_Surface* {
        switch (environment_texture) {
            case metacore::EnvironmentTexture::SpaceStation:
                static auto* const space_station_ground =
                    IMG_Load("spacestationground.png");
                return space_station_ground;
            case metacore::EnvironmentTexture::Castle:
                static auto* const castle_ground = IMG_Load("castleground.png");
                return castle_ground;
            case metacore::EnvironmentTexture::None:
                return nullptr;
        }
        return nullptr;
    }();
    if (image == nullptr) {
        throw_sdl_error();
    }
    auto* const texture = SDL_CreateTextureFromSurface(&renderer, image);
    if (texture == nullptr) {
        throw_sdl_error();
    }
    for (auto x = bounds.left; x < bounds.right; x += 50) {
        for (auto y = bounds.bottom; y < bounds.top; y += 50) {
            render_texture_at_position(renderer, {x, y}, texture);
        }
    }
    SDL_DestroyTexture(texture);
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
    render_ground(renderer, state.layout_bounds, state.environment_texture);
    render_tiles(renderer, state.tiles, state.environment_texture);
    render_enemies(renderer, state.enemy_positions, state.enemies_texture);
    if (state.slash_attack.has_value()) {
        render_slash_attack(
            renderer, state.player_position, *state.slash_attack);
    }
    render_player(renderer, state.player_position, state.player_texture);
    if (state.upgrade_position.has_value()) {
        render_upgrade(renderer, *state.upgrade_position);
    }
    if (state.projectiles.has_value()) {
        render_projectiles(renderer, *state.projectiles);
    }
    if (state.upgrade_choices.has_value()) {
        render_upgrade_choices(renderer, font, *state.upgrade_choices);
    }
    render_game_progress(renderer, font, state.progress);
    SDL_RenderPresent(&renderer);
}

enum class CloseRequested : bool { Yes, No };

CloseRequested read_and_pass_input(metacore::MetaEngine& engine)
{
    static auto last_key = std::optional<SDL_Keycode>{};
    auto close_requested = CloseRequested::No;
    auto event = SDL_Event{};
    auto key = std::optional<SDL_Keycode>{};
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                if (last_key != event.key.keysym.sym) {
                    key = event.key.keysym.sym;
                    last_key = key;
                }
                break;
            case SDL_KEYUP:
                if (last_key == event.key.keysym.sym) {
                    last_key = {};
                }
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                    close_requested = CloseRequested::Yes;
                }
                break;
        }
    }
    if (key.has_value()) {
        switch (*key) {
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
            case SDLK_F5:
                engine.input_restart();
                break;
            case SDLK_ESCAPE:
                close_requested = CloseRequested::Yes;
                break;
        }
    } else if (!last_key.has_value()) {
        engine.input_stop();
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
        constexpr auto imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags)) {
            throw std::runtime_error{std::format(
                "SDL_image could not initialize! SDL_image Error: {}\n",
                IMG_GetError())};
        }
        window = SDL_CreateWindow(
            "Meta",
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