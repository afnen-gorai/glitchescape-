#include "header.h"

int main(int argc, char* argv[]) {
    SDL_Surface* screen = NULL;
    SDL_Event event;
    int quit = 0;
    Enigme enigme;
    Uint32 startTime;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
    SDL_WM_SetCaption("Énigme SDL", NULL);

    initEnigme(&enigme);
    loadImages(&enigme);

    startTime = SDL_GetTicks();

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            handleEvents(&event, &enigme, &quit);
        }

        updateEnigme(&enigme, &startTime);

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        renderEnigme(screen, &enigme);
        renderTimer(screen, &enigme);
        renderEssais(screen, &enigme);
        renderScore(screen, &enigme);

        SDL_Flip(screen);
        SDL_Delay(16); // 60 FPS
    }

    cleanup(&enigme);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

