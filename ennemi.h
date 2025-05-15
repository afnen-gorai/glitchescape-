#ifndef ENNEMI_H_INCLUDED
#define ENNEMI_H_INCLUDED

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

typedef struct enemie {
    SDL_Surface *spritesheet;
SDL_Surface *death_spritesheet; // Death animation sprite sheet
    int direction;
    SDL_Rect pos, xminimale, xmaximale;
    SDL_Rect posspritesheet;

    int vie; // <-- Ajout pour les cœurs / santé

int is_dying;                   // Flag to indicate dying state
    int death_frame;                // Current frame of death animation
    int death_frame_count;          // Total number of frames in death animation

} enemie;

typedef struct {
    SDL_Surface *img[10];
    SDL_Rect pos;
    int active;
    int score;
} bonus;

void init_bonus(bonus *b);
void afficher_bonus(bonus *b, SDL_Surface *screen);
int collision(SDL_Rect a, SDL_Rect b);

typedef struct {
    SDL_Surface *perso;
    SDL_Rect perso_pos;
} personnage;

// Fonctions ennemie
void init_enemie(enemie *E);
void afficher_enemie(enemie *E, SDL_Surface *screen);
void animate_enemie(enemie *E);
void deplacer(enemie *E);
void deplacerIA(enemie *E, SDL_Rect posPerso);
void afficher_vie(enemie *E, SDL_Surface *screen, SDL_Surface *coeur); // <-- Ajoutée

void animate_death(enemie *E);  // Add this declaration

// Fonctions personnage
void initialiser_perso(personnage *p);
void afficher_perso(personnage *p, SDL_Surface *screen);

// Collision
int collision(SDL_Rect E, SDL_Rect p); // <-- Ajout si pas déjà dans un autre .h

#endif // ENNEMI_H_INCLUDED

