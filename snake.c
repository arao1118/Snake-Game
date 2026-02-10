#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CELL_SIZE 30
#define SCREEN_SIZE 900

typedef struct snakeElement {
    int x, y;
    struct snakeElement *next;
} snakeElement;

snakeElement snakeHead = {30, 30, NULL};

typedef struct {
    snakeElement *head;
    snakeElement *tail;
} snake;

snake fullSnake = {&snakeHead, &snakeHead};

typedef struct {
    int x, y;
} Apple;

typedef struct {
    int dx;
    int dy;
} Direction;

Direction direction = {+1, 0};

void spawn_apple(Apple *apple, snakeElement *head) {
    int collision;
    do {
        collision = 0;
        apple->x = (rand() % (SCREEN_SIZE / CELL_SIZE)) * CELL_SIZE;
        apple->y = (rand() % (SCREEN_SIZE / CELL_SIZE)) * CELL_SIZE;

        snakeElement *temp = head;
        while (temp != NULL) {
            if (temp->x == apple->x && temp->y == apple->y) {
                collision = 1;
                break;
            }
            temp = temp->next;
        }
    } while (collision);
}

void draw_apple(SDL_Surface *surface, Apple *apple, Uint32 color) {
    SDL_Rect rect = {apple->x, apple->y, CELL_SIZE, CELL_SIZE};
    SDL_FillSurfaceRect(surface, &rect, color);
}

int check_self_collision(snakeElement *head) {
    if (!head || !head->next) return 0;

    snakeElement *temp = head->next;
    while (temp) {
        if (head->x == temp->x && head->y == temp->y)
            return 1;
        temp = temp->next;
    }
    return 0;
}

static inline void clear_screen(SDL_Surface *surface, Uint32 color) {
    SDL_FillSurfaceRect(surface, NULL, color);
}

void draw_grid(SDL_Surface *surface, Uint32 color) {
    SDL_Rect line;

    for (int y = 0; y <= SCREEN_SIZE; y += CELL_SIZE) {
        for (int x = 0; x < SCREEN_SIZE; x++) {
            line = (SDL_Rect){x, y, 1, 1};
            SDL_FillSurfaceRect(surface, &line, color);
        }
    }

    for (int x = 0; x <= SCREEN_SIZE; x += CELL_SIZE) {
        for (int y = 0; y < SCREEN_SIZE; y++) {
            line = (SDL_Rect){x, y, 1, 1};
            SDL_FillSurfaceRect(surface, &line, color);
        }
    }
}

static inline void move_snake(snakeElement *head, Direction *direction) {
    if (!head) return;

    int prev_x = head->x;
    int prev_y = head->y;

    head->x += direction->dx * CELL_SIZE;
    head->y += direction->dy * CELL_SIZE;

    if (head->x >= SCREEN_SIZE) head->x = 0;
    if (head->x < 0) head->x = SCREEN_SIZE - CELL_SIZE;
    if (head->y >= SCREEN_SIZE) head->y = 0;
    if (head->y < 0) head->y = SCREEN_SIZE - CELL_SIZE;

    snakeElement *temp = head->next;
    while (temp) {
        int current_x = temp->x;
        int current_y = temp->y;

        temp->x = prev_x;
        temp->y = prev_y;

        prev_x = current_x;
        prev_y = current_y;

        temp = temp->next;
    }
}

void draw_snake(SDL_Surface *surface, snakeElement *head, Uint32 color) {
    snakeElement *temp = head;
    while (temp) {
        SDL_Rect rect = {temp->x, temp->y, CELL_SIZE, CELL_SIZE};
        SDL_FillSurfaceRect(surface, &rect, color);
        temp = temp->next;
    }
}

void lengthen_snake(snake *fullSnake) {
    snakeElement *newPart = malloc(sizeof(snakeElement));
    *newPart = (snakeElement){fullSnake->tail->x, fullSnake->tail->y, NULL};

    fullSnake->tail->next = newPart;
    fullSnake->tail = newPart;
}

int main(int argc, char *argv[]) {
    srand((unsigned int)time(NULL));
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Snake Game", SCREEN_SIZE, SCREEN_SIZE, 0);

    SDL_Surface *surface = SDL_GetWindowSurface(window);

    const SDL_PixelFormatDetails *fmt =
        SDL_GetPixelFormatDetails(surface->format);

    Uint32 white = SDL_MapRGBA(fmt, NULL, 255, 255, 255, 255);
    Uint32 black = SDL_MapRGBA(fmt, NULL, 0, 0, 0, 255);
    Uint32 red   = SDL_MapRGBA(fmt, NULL, 255, 0, 0, 255);

    Apple apple;
    spawn_apple(&apple, fullSnake.head);

    int isRunning = 1;

    while (isRunning) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                isRunning = 0;

            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_RIGHT && direction.dx != -1)
                    direction = (Direction){1, 0};
                if (event.key.key == SDLK_LEFT && direction.dx != 1)
                    direction = (Direction){-1, 0};
                if (event.key.key == SDLK_UP && direction.dy != 1)
                    direction = (Direction){0, -1};
                if (event.key.key == SDLK_DOWN && direction.dy != -1)
                    direction = (Direction){0, 1};
            }
        }

        move_snake(fullSnake.head, &direction);

        if (check_self_collision(fullSnake.head)) {
            printf("Game Over! You hit yourself.\n");
            isRunning = 0;
        }

        if (fullSnake.head->x == apple.x && fullSnake.head->y == apple.y) {

            lengthen_snake(&fullSnake);
            spawn_apple(&apple, fullSnake.head);
        }

        clear_screen(surface, black);
        draw_grid(surface, white);
        draw_apple(surface, &apple, red);
        draw_snake(surface, fullSnake.head, white);

        SDL_UpdateWindowSurface(window);
        SDL_Delay(250);
    }

    snakeElement *curr = fullSnake.head->next;
    while (curr) {
        snakeElement *next = curr->next;
        free(curr);
        curr = next;
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
