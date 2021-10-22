#include <cstdio>
#include <SDL2/SDL.h>

namespace {

constexpr auto width = 800;
constexpr auto height = 600;
constexpr auto delay = 3000;

} // namespace

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL failed to initialise: %s\n", SDL_GetError());
        return 1;
    }
    auto *const window = SDL_CreateWindow(
        "SDL Example",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        0);
    if (window == nullptr) {
        fprintf(
            stderr, "SDL window failed to initialise: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Delay(delay);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}