#include "header.h"
#include <SDL/SDL_rotozoom.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void initEnigme(Enigme* enigme) {
    enigme->puzzle = NULL;
    for (int i = 0; i < 3; i++) {
        enigme->propositions[i] = NULL;
    }
    enigme->cadre = NULL;

    enigme->selectedPiece = -1;
    enigme->timeLeft = 60;
    enigme->piecePosition = (SDL_Rect){50, 50, 100, 100};
    enigme->score = 0;
    enigme->essaisRestants = 3;
    enigme->winDisplayTime = 0;
    enigme->loseDisplayTime = 0;

    enigme->message[0] = '\0';
    enigme->messageStartTime = 0;

    enigme->selectedImage = NULL;
    enigme->selectedImagePos = (SDL_Rect){390, 190};

    TTF_Init();
    enigme->font = TTF_OpenFont("arial.ttf", 40);
    if (!enigme->font) {
        fprintf(stderr, "Erreur TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    // Positions fixes d'affichage
    SDL_Rect pos[3] = {
        {0, 400, 100, 100},
        {270, 400, 100, 100},
        {540, 400, 100, 100}
    };

    int used[3] = {0};
    for (int i = 0; i < 3; i++) {
        int idx;
        do {
            idx = rand() % 3;
        } while (used[idx]);
        used[idx] = 1;
        enigme->ordreAffichage[i] = idx;
        enigme->propositionPositions[i] = pos[i];

        if (idx == 2) { // 2.png est la bonne image (index 1)
            enigme->correctPiece = i;
        }
    }
}

void loadImages(Enigme* enigme) {
    enigme->puzzle = IMG_Load("pu.png");
    enigme->cadre = IMG_Load("cadre.png");
    enigme->propositions[0] = IMG_Load("1.png");
    enigme->propositions[1] = IMG_Load("2.png");
    enigme->propositions[2] = IMG_Load("3.png");
}

void handleEvents(SDL_Event* e, Enigme* enigme, int* quit) {
    if (e->type == SDL_QUIT) {
        *quit = 1;
    } else if (e->type == SDL_MOUSEBUTTONDOWN && enigme->essaisRestants > 0 && enigme->winDisplayTime == 0) {
        int mx = e->button.x, my = e->button.y;

        for (int i = 0; i < 3; i++) {
            SDL_Rect r = enigme->propositionPositions[i];
            if (mx >= r.x && mx <= r.x + 100 && my >= r.y && my <= r.y + 100) {
                enigme->selectedPiece = i;
                enigme->selectedImage = enigme->propositions[enigme->ordreAffichage[i]];

                if (i == enigme->correctPiece) {
                    enigme->score += 10;
                    enigme->winDisplayTime = SDL_GetTicks();
                    strcpy(enigme->message, "bravo!");
                } else {
                    enigme->essaisRestants--;
                    if (enigme->essaisRestants == 0) {
                        enigme->loseDisplayTime = SDL_GetTicks();
                    }
                    strcpy(enigme->message, "perdu !");
                }
                enigme->messageStartTime = SDL_GetTicks();
                break;
            }
        }
    }
}

void updateEnigme(Enigme* enigme, Uint32* startTime) {
    Uint32 now = SDL_GetTicks();
    if (enigme->winDisplayTime == 0 && enigme->loseDisplayTime == 0) {
        enigme->timeLeft = 30 - (now - *startTime) / 1000;
        if (enigme->timeLeft <= 0) {
            enigme->essaisRestants = 0;
            enigme->loseDisplayTime = now;
        }
    }
}

void renderEnigme(SDL_Surface* screen, Enigme* enigme) {
    SDL_BlitSurface(enigme->puzzle, NULL, screen, &enigme->piecePosition);


    for (int i = 0; i < 3; i++) {
        int imgIndex = enigme->ordreAffichage[i];
        SDL_BlitSurface(enigme->propositions[imgIndex], NULL, screen, &enigme->propositionPositions[i]);
    }

    if (enigme->selectedImage) {
        SDL_BlitSurface(enigme->selectedImage, NULL, screen, &enigme->selectedImagePos);
    }

    if (enigme->message[0] != '\0' && SDL_GetTicks() - enigme->messageStartTime < 2000) {
        SDL_Color color = {0, 255, 255};
        SDL_Surface* text = TTF_RenderText_Solid(enigme->font, enigme->message, color);
        float angle = (SDL_GetTicks() - enigme->messageStartTime) * 0.3f;
        SDL_Surface* rot = rotozoomSurface(text, angle, 1.0 + angle / 360.0, 1);
        SDL_Rect pos = {SCREEN_WIDTH / 2 - rot->w / 2, 250};
        SDL_BlitSurface(rot, NULL, screen, &pos);
        SDL_FreeSurface(rot);
        SDL_FreeSurface(text);
    }
}

void renderTimer(SDL_Surface* screen, Enigme* enigme) {
    char buffer[50];
    SDL_Color color = {255, 255, 255};
    sprintf(buffer, "Temps : %d", enigme->timeLeft);
    SDL_Surface* text = TTF_RenderText_Solid(enigme->font, buffer, color);
    SDL_Rect pos = {10, 10};
    SDL_BlitSurface(text, NULL, screen, &pos);
    SDL_FreeSurface(text);
}
void afficherTemps(SDL_Surface *ecran, Enigme *e) {
    // Calcul du temps restant
    Uint32 tempsEcoule = (SDL_GetTicks() - e->debutTemps) / 1000;
    e->tempsRestant = (tempsEcoule < 30) ? 30 - tempsEcoule : 0;
    
    // Création du texte du timer
    char tempsTexte[32];
    snprintf(tempsTexte, sizeof(tempsTexte), "Temps: %d", e->tempsRestant);
    
    // Couleur du texte (rouge si moins de 10 secondes)
    SDL_Color couleur = (e->tempsRestant <= 10) ? (SDL_Color){255, 0, 0, 255} : e->textColor;
    
    // Rendu du texte
    SDL_Surface *surfaceTemps = TTF_RenderText_Blended(e->font, tempsTexte, couleur);
    if (surfaceTemps) {
        SDL_Rect position = {SCREEN_WIDTH - 150, 20, 0, 0};
        SDL_BlitSurface(surfaceTemps, NULL, ecran, &position);
        SDL_FreeSurface(surfaceTemps);
    }
}

void renderEssais(SDL_Surface* screen, Enigme* enigme) {
    char buffer[50];
    SDL_Color color = {255, 255, 255};
    sprintf(buffer, "Essais : %d", enigme->essaisRestants);
    SDL_Surface* text = TTF_RenderText_Solid(enigme->font, buffer, color);
    SDL_Rect pos = {10, 50};
    SDL_BlitSurface(text, NULL, screen, &pos);
    SDL_FreeSurface(text);
}

void renderScore(SDL_Surface* screen, Enigme* enigme) {
    char buffer[50];
    SDL_Color color = {255, 0, 255};
    sprintf(buffer, "Score : %d", enigme->score);
    SDL_Surface* text = TTF_RenderText_Solid(enigme->font, buffer, color);
    SDL_Rect pos = {SCREEN_WIDTH - 200, 10};
    SDL_BlitSurface(text, NULL, screen, &pos);
    SDL_FreeSurface(text);
}

void cleanup(Enigme* enigme) {
    SDL_FreeSurface(enigme->puzzle);
    SDL_FreeSurface(enigme->cadre);
    for (int i = 0; i < 3; i++) {
        SDL_FreeSurface(enigme->propositions[i]);
    }
    TTF_CloseFont(enigme->font);
}

