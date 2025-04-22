#include "header.h"

<<<<<<< HEAD
int main(int argc, char *argv[]) {
    (void)argc;  // Marque le paramètre comme utilisé
    (void)argv;  // Marque le paramètre comme utilisé
    SDL_Surface *screen = NULL;
    Player joueur1, joueur2;
    Background background;
    Platform platforms[PLATFORM_COUNT];
    Uint32 start_time = 0;
    int running = 1;
    int show_help = 1;
    
    initSDL();
    screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if(!screen) {
        printf("Erreur création fenêtre: %s\n", SDL_GetError());
        return 1;
    }
    
    if(loadImages(&joueur1, &joueur2, &background, platforms) == -1) {
        return 1;
    }
    
    initGame(&joueur1, &joueur2, &background, platforms);
    
    while(running) {
        handleEvents(&running, &joueur1, &joueur2, &background, &show_help);
        updateGame(&joueur1, &joueur2, &background, platforms, &start_time);
        renderGame(screen, &background, &joueur1, &joueur2, platforms, start_time, show_help);
        SDL_Delay(1000/FPS);
    }
    
    SDL_FreeSurface(joueur1.image);
    SDL_FreeSurface(joueur2.image);
    SDL_FreeSurface(background.image);
    
    for(int i = 0; i < PLATFORM_COUNT; i++) {
        SDL_FreeSurface(platforms[i].image);
    }
    
    TTF_Quit();
    SDL_Quit();
=======
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

>>>>>>> 3a681c4166833e286d261b44e68d755a5a40f3d9
    return 0;
}

