#include <SDL2/SDL.h>
#include <stdlib.h>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 700
#define CELL_WIDTH 50
#define LINE_WIDTH 1

typedef struct Coordinate {
  int x;
  int y;
} Coordinate;

typedef struct SnackNode {
  Coordinate coor;
  struct SnackNode *next;
} SnackNode;

SnackNode *init_snack(Coordinate head_pos) {
  SnackNode *newNode = (SnackNode *)malloc(sizeof(SnackNode));
  newNode->coor = head_pos;
  newNode->next = NULL;

  return newNode;
}

void add_last(SnackNode **head, Coordinate head_pos) {
  SnackNode *newNode = (SnackNode *)malloc(sizeof(SnackNode));
  newNode->coor = head_pos;
  newNode->next = NULL;

  SnackNode *temp = *head;
  while (temp->next != NULL) {
    temp = temp->next;
  }
  temp->next = newNode;
}

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

void draw_cell(SDL_Renderer *renderer, Coordinate coor) {
  SDL_Rect rect = (SDL_Rect){coor.x * CELL_WIDTH, coor.y * CELL_WIDTH,
                             CELL_WIDTH, CELL_WIDTH};
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 1.0);
  SDL_RenderFillRect(renderer, &rect);
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

  SnackNode *head = init_snack((Coordinate){1, 2});
  add_last(&head, (Coordinate){2, 3});
  add_last(&head, (Coordinate){5, 4});

  SnackNode *temp = head;
  while (temp != NULL) {
    printf("%d %d\n", temp->coor.x, temp->coor.y);
    temp = temp->next;
  }

  int running = 1;
  while (running) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_QUIT) {
        running = 0;
      }
    }

    draw_cell(renderer, head->coor);
    draw_grid(renderer);

    SDL_Delay(16);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}