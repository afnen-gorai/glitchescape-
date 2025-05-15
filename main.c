#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <time.h>
#include "ennemi.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 720
#define FPS 60
#define DEATH_ANIMATION_DELAY 150 // Delay between death animation frames (in milliseconds)

SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
TTF_Font *font = NULL;

int initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return 0;
    }

    if (TTF_Init() == -1) {
        printf("Unable to initialize SDL_ttf: %s\n", TTF_GetError());
        return 0;
    }

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (screen == NULL) {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    SDL_WM_SetCaption("ENTITE SECONDAIRE", NULL);
    return 1;
}

void cleanup(SDL_Surface *coeur) {
    SDL_FreeSurface(background);
    SDL_FreeSurface(coeur);
    if (font) TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}

int main() {
    if (!initSDL())
        return 1;

    srand(time(NULL));

    background = IMG_Load("back.png");
    if (background == NULL) {
        printf("Unable to load background image: %s\n", IMG_GetError());
        cleanup(NULL);
        return 1;
    }

    SDL_Surface *coeur = IMG_Load("coeur.png");
    if (coeur == NULL) {
        printf("Unable to load heart image: %s\n", IMG_GetError());
        cleanup(NULL);
        return 1;
    }

    font = TTF_OpenFont("arial.ttf", 24);
    if (font == NULL) {
        printf("Unable to load font: %s\n", TTF_GetError());
        cleanup(coeur);
        return 1;
    }

    enemie E;
    init_enemie(&E);

    personnage p;
    initialiser_perso(&p);

    bonus b;
    init_bonus(&b);

    SDL_Event event;
    int done = 1;
    int keyA_pressed = 0;

    SDL_Color textColor = {255, 255, 255};
    char scoreText[32];
    SDL_Surface *scoreSurface = NULL;

    Uint32 startTime, endTime, frameTime;
    Uint32 lastDeathFrameTime = 0; // Track time for death animation frames

    while (done) {
        startTime = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = 0;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_RIGHT:
                        p.perso_pos.x += 20;
                        break;
                    case SDLK_LEFT:
                        p.perso_pos.x -= 20;
                        break;
                    case SDLK_UP:
                        p.perso_pos.y -= 20;
                        break;
                    case SDLK_DOWN:
                        p.perso_pos.y += 20;
                        break;
                    case SDLK_a:
                        keyA_pressed = 1;
                        break;
                }
            }
        }

        SDL_BlitSurface(background, NULL, screen, NULL);

        afficher_bonus(&b, screen);
        if (b.active && collision(p.perso_pos, b.pos)) {
            b.score++;
            b.active = 0;
        }

        sprintf(scoreText, "Score: %d", b.score);
        scoreSurface = TTF_RenderText_Solid(font, scoreText, textColor);
        if (scoreSurface) {
            SDL_Rect scorePos = {10, 10, 0, 0};
            SDL_BlitSurface(scoreSurface, NULL, screen, &scorePos);
            SDL_FreeSurface(scoreSurface);
        }

        if (E.vie > 0) {
            animate_enemie(&E);
            deplacerIA(&E, p.perso_pos);
            deplacer(&E);

            if (collision(E.pos, p.perso_pos) && keyA_pressed) {
                E.vie--;
                keyA_pressed = 0;
                if (E.vie == 0) {
                    E.is_dying = 1; // Start death animation
                    E.death_frame = 0;
                    lastDeathFrameTime = SDL_GetTicks();
                }
                SDL_Delay(150);
            }

            afficher_enemie(&E, screen);
            afficher_vie(&E, screen, coeur);
        } else if (E.is_dying) {
            // Play death animation
            if (SDL_GetTicks() - lastDeathFrameTime >= DEATH_ANIMATION_DELAY) {
                animate_death(&E);
                lastDeathFrameTime = SDL_GetTicks();
            }
            afficher_enemie(&E, screen);
        }

        afficher_perso(&p, screen);
        SDL_Flip(screen);

        endTime = SDL_GetTicks();
        frameTime = endTime - startTime;
        if (frameTime < 1000 / FPS) {
            SDL_Delay(1000 / FPS - frameTime);
        }
    }

    SDL_FreeSurface(E.spritesheet);
    SDL_FreeSurface(E.death_spritesheet); // Free the death sprite sheet
    cleanup(coeur);
    return 0;
}
