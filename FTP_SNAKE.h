#define WIDTH 20
#define HEIGHT 20
#define SNAKE_SPEED 100

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point points[HEIGHT * WIDTH];
    int length;
} Snake;

typedef struct {
    Snake snake;
    Point food;
    int score;
    bool game_over;
} GameState;

void init_game(GameState *game) {
    game->snake.length = 1;
    game->snake.points[0].x = WIDTH / 2;
    game->snake.points[0].y = HEIGHT / 2;
    game->food.x = rand() % WIDTH;
    game->food.y = rand() % HEIGHT;
    game->score = 0;
    game->game_over = false;
}

void game_loop(GameState *game) {
    int ch;
    while (!game->game_over) {
        ch = getch();
        // Handle input, update game state, and render here
    }
}

void handle_input(GameState *game, int ch) {
    // Implement direction changing based on user input
}

void update_game_state(GameState *game) {
    // Update snake position, check for collisions, update score
}

void render_game(GameState *game) {
    // Clear screen, render snake, render food, display score
}
