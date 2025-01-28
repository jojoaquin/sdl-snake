#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 700
#define CELL_WIDTH 50
#define LINE_WIDTH 1

typedef struct Coordinate {
  int x;
  int y;
} Coordinate;

typedef struct SnakeNode {
  Coordinate coor;
  struct SnakeNode *next;
} SnakeNode;

void draw_grid(SDL_Renderer *renderer) {
  for (int i = CELL_WIDTH; i < SCREEN_WIDTH; i += CELL_WIDTH) {
    SDL_Rect rect = (SDL_Rect){i, 0, LINE_WIDTH, SCREEN_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);

    SDL_RenderFillRect(renderer, &rect);
  }

  for (int i = CELL_WIDTH; i < SCREEN_HEIGHT; i += CELL_WIDTH) {
    SDL_Rect rect = (SDL_Rect){0, i, SCREEN_WIDTH, LINE_WIDTH};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);

    SDL_RenderFillRect(renderer, &rect);
  }
}

void draw_cell(SDL_Renderer *renderer, Coordinate coor, int clear) {
  SDL_Rect rect = (SDL_Rect){coor.x * CELL_WIDTH, coor.y * CELL_WIDTH,
                             CELL_WIDTH, CELL_WIDTH};
  if (clear == 0) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 1.0);
  } else {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);
  }
  SDL_RenderFillRect(renderer, &rect);
}

SnakeNode *init_snake(Coordinate head_pos) {
  SnakeNode *newNode = (SnakeNode *)malloc(sizeof(SnakeNode));
  newNode->coor = head_pos;
  newNode->next = NULL;

  return newNode;
}

void draw_snake_body(SDL_Renderer *renderer, SnakeNode *head, int clear) {
  SnakeNode *temp = head;

  while (temp != NULL) {
    draw_cell(renderer, (Coordinate){temp->coor.x, temp->coor.y}, clear);
    temp = temp->next;
  }
}

static bool spawn = true;
static Coordinate apple_coor = {0, 0};

Coordinate spawn_random_apple(SDL_Renderer *renderer) {
  if (spawn == true) {
    int randX = rand() % ((SCREEN_WIDTH / CELL_WIDTH - 2) - 1 + 1) + 1;
    int randY = rand() % ((SCREEN_HEIGHT / CELL_WIDTH - 2) - 1 + 1) + 1;

    apple_coor = (Coordinate){randX, randY};
    spawn = false;
  }

  SDL_Rect rect = (SDL_Rect){apple_coor.x * CELL_WIDTH,
                             apple_coor.y * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH};
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(renderer, &rect);

  return apple_coor;
}

void add_apple(SDL_Renderer *renderer, SnakeNode *head) {
  Coordinate coor = spawn_random_apple(renderer);

  if (coor.x == head->coor.x && coor.y == head->coor.y) {
    SnakeNode *temp = head;

    SnakeNode *newNode = (SnakeNode *)malloc(sizeof(SnakeNode));
    newNode->coor = head->coor;
    newNode->next = NULL;

    while (temp->next != NULL) {
      temp = temp->next;
    }
    temp->next = newNode;
    spawn = true;
  }
}

// void check_collision_wall(SnakeNode *head) {
//   if (head->coor.x < 1 || head->coor.x >= SCREEN_WIDTH / CELL_WIDTH - 1 ||
//       head->coor.y < 1 || head->coor.y >= SCREEN_HEIGHT / CELL_WIDTH - 1) {
//     SDL_Quit();
//   }
// }

#define MOVE_RIGHT 1
#define MOVE_LEFT 2
#define MOVE_UP 3
#define MOVE_DOWN 4

int currentDirection;

void move_snake(SDL_Renderer *renderer, SnakeNode **head,
                const Uint8 *keyState) {

  if (keyState[SDL_SCANCODE_RIGHT] && currentDirection != MOVE_LEFT) {
    currentDirection = MOVE_RIGHT;
  } else if (keyState[SDL_SCANCODE_DOWN] && currentDirection != MOVE_UP) {
    currentDirection = MOVE_DOWN;
  } else if (keyState[SDL_SCANCODE_UP] && currentDirection != MOVE_DOWN) {
    currentDirection = MOVE_UP;
  } else if (keyState[SDL_SCANCODE_LEFT] && currentDirection != MOVE_RIGHT) {
    currentDirection = MOVE_LEFT;
  }

  Coordinate prev = (*head)->coor;

  if (currentDirection == MOVE_RIGHT &&
      (*head)->coor.x < SCREEN_WIDTH / CELL_WIDTH - 2) {
    draw_cell(renderer, (*head)->coor, 1);
    (*head)->coor.x += 1;
  } else if (currentDirection == MOVE_DOWN &&
             (*head)->coor.y < SCREEN_HEIGHT / CELL_WIDTH - 2) {
    draw_cell(renderer, (*head)->coor, 1);
    (*head)->coor.y += 1;
  } else if (currentDirection == MOVE_LEFT && (*head)->coor.x > 1) {
    draw_cell(renderer, (*head)->coor, 1);
    (*head)->coor.x -= 1;
  } else if (currentDirection == MOVE_UP && (*head)->coor.y > 1) {
    draw_cell(renderer, (*head)->coor, 1);
    (*head)->coor.y -= 1;
  }

  SnakeNode *temp = (*head)->next;
  while (temp != NULL) {
    Coordinate temp_prev = temp->coor;
    draw_cell(renderer, temp->coor, 1);

    temp->coor = prev;
    prev = temp_prev;
    temp = temp->next;
  }
}

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window =
      SDL_CreateWindow("Hello snake", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);
  SDL_RenderClear(renderer);

  SnakeNode *head = init_snake((Coordinate){5, 2});

  SnakeNode *temp = head;
  while (temp != NULL) {
    printf("%d %d\n", temp->coor.x, temp->coor.y);
    temp = temp->next;
  }

  int running = 1;
  const Uint8 *keyState = SDL_GetKeyboardState(NULL);

  while (running) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_QUIT) {
        running = 0;
      }
    }
    add_apple(renderer, head);
    move_snake(renderer, &head, keyState);
    // check_collision_wall(head);

    draw_snake_body(renderer, head, 0);
    draw_grid(renderer);

    SDL_Delay(90);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}