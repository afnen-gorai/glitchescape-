#ifndef HEADER_H
#define HEADER_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef struct {
    SDL_Surface* puzzle;
    SDL_Surface* propositions[3];
    SDL_Surface* cadre;

    SDL_Rect piecePosition;
    SDL_Rect propositionPositions[3];

    int correctPiece;
    int selectedPiece;
    int timeLeft;
    int essaisRestants;
    int score;

    Uint32 winDisplayTime;
    Uint32 loseDisplayTime;

    TTF_Font* font;

    // Pour affichage message après clic
    char message[50];
    Uint32 messageStartTime;

    // Pour afficher la proposition déplacée
    SDL_Surface* selectedImage;
    SDL_Rect selectedImagePos;


int ordreAffichage[3]; // Permet de savoir quelle image est affichée à quelle position

} Enigme;

void initEnigme(Enigme* enigme);
void loadImages(Enigme* enigme);
void handleEvents(SDL_Event* e, Enigme* enigme, int* quit);
void updateEnigme(Enigme* enigme, Uint32* startTime);
void renderEnigme(SDL_Surface* screen, Enigme* enigme);
void renderTimer(SDL_Surface* screen, Enigme* enigme);
void renderEssais(SDL_Surface* screen, Enigme* enigme);
void renderScore(SDL_Surface* screen, Enigme* enigme);
void cleanup(Enigme* enigme);

#endif

