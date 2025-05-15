#include "ennemi.h"

void init_enemie(enemie *E) {
    E->spritesheet = IMG_Load("hero_spr.png");
    E->death_spritesheet = IMG_Load("death_spr.png"); // Load the death sprite sheet
    E->pos.x = 690;
    E->pos.y = 520;
    E->direction = 0;

    E->pos.w = 80; // Adjust size for collision to match death sprite frame
    E->pos.h = 80;

    E->posspritesheet.x = 0;
    E->posspritesheet.y = 0;
    E->posspritesheet.h = 100; // Regular sprite sheet frame height (adjust if needed)
    E->posspritesheet.w = 100; // Regular sprite sheet frame width (adjust if needed)

    E->xminimale.x = 500;
    E->xmaximale.x = 700;

    E->vie = 3; // 3 hearts at start
    E->is_dying = 0; // Not dying initially
    E->death_frame = 0;
    E->death_frame_count = 4; // 4 frames in the death animation
}

void animate_death(enemie *E) {
    if (E->is_dying) {
        // Use smaller dimensions for death animation
        E->posspritesheet.w = 80; // Death sprite frame width
        E->posspritesheet.h = 80; // Death sprite frame height
        E->posspritesheet.x = E->death_frame * E->posspritesheet.w;
        E->posspritesheet.y = 0; // Assuming death animation is in the first row
        E->death_frame++;
        if (E->death_frame >= E->death_frame_count) {
            E->is_dying = 0; // Stop animation after playing once
            E->vie = -1; // Mark enemy as fully dead
            // Reset posspritesheet dimensions for regular animation if needed
            E->posspritesheet.w = 100;
            E->posspritesheet.h = 100;
        }
    }
}

void animate_enemie(enemie *E) {
    E->posspritesheet.w = 100; // Ensure regular animation uses correct dimensions
    E->posspritesheet.h = 100;
    E->posspritesheet.y = E->direction * E->posspritesheet.h;
    if (E->posspritesheet.x >= 300) {
        E->posspritesheet.x = 0;
    } else {
        E->posspritesheet.x += E->posspritesheet.w; // +=100
    }
}

void afficher_enemie(enemie *E, SDL_Surface *screen) {
    if (E->is_dying) {
        SDL_BlitSurface(E->death_spritesheet, &E->posspritesheet, screen, &E->pos);
    } else if (E->vie > 0) {
        SDL_BlitSurface(E->spritesheet, &E->posspritesheet, screen, &E->pos);
    }
}

void deplacer(enemie *E) {
    if (E->pos.x < E->xminimale.x) {
        E->direction = 1;
    } else if (E->pos.x > E->xmaximale.x) {
        E->direction = 0;
    }

    if (E->direction == 1) {
        E->pos.x += 15;
    }
    if (E->direction == 0) {
        E->pos.x -= 15;
    }
}

void deplacerIA(enemie *E, SDL_Rect posPerso) {
    int distance = E->pos.x - posPerso.x;
    if (distance < 200 && distance > 80) {
        E->direction = 1;
        E->pos.x -= 20;
    } else if (distance < -80 && distance > -200) {
        E->direction = 0;
        E->pos.x += 20;
    } else if (distance <= 80 && distance >= 0) {
        E->direction = 3;
    } else if (distance <= 0 && distance >= -80) {
        E->direction = 2;
    }
}

int collision(SDL_Rect E, SDL_Rect p) {
    if ((p.x + p.w) < E.x || p.x > (E.x + E.w) || (p.y + p.h) < E.y || p.y > (E.y + E.h)) {
        return 0;
    } else {
        return 1;
    }
}

void initialiser_perso(personnage *p) {
    p->perso = IMG_Load("perso1.png");
    p->perso_pos.x = 100;
    p->perso_pos.y = 560;
    p->perso_pos.h = 281;
    p->perso_pos.w = 217;
}

void afficher_perso(personnage *p, SDL_Surface *screen) {
    SDL_BlitSurface(p->perso, NULL, screen, &p->perso_pos);
    SDL_Delay(100); // Adds a pause of 100 milliseconds to the program
}

void afficher_vie(enemie *E, SDL_Surface *screen, SDL_Surface *coeur) {
    SDL_Rect pos;
    for (int i = 0; i < E->vie; i++) {
        pos.x = E->pos.x + i * 25;
        pos.y = E->pos.y - 30;
        SDL_BlitSurface(coeur, NULL, screen, &pos);
    }
}

void init_bonus(bonus *b) {
    char path[100];
    for (int i = 0; i < 10; i++) {
        sprintf(path, "bonus/image%d.png", i + 1);
        b->img[i] = IMG_Load(path);
    }
    b->pos.x = 900;
    b->pos.y = 600;
    b->active = 1;
    b->score = 0;
}

void afficher_bonus(bonus *b, SDL_Surface *screen) {
    if (b->active)
        SDL_BlitSurface(b->img[rand() % 10], NULL, screen, &b->pos);
}
