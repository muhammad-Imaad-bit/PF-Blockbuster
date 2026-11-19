// Block Buster - starter skeleton (SFML 3)
// SFML scaffolding is provided. Every TODO is YOUR game logic.
#include <SFML/Graphics.hpp>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <optional>
#include <string>
using namespace std;

// ===================== CONSTANTS (add more as you need) =====================
const int   WIN_W = 600, WIN_H = 700, HUD_H = 50;
const int   ROWS = 8, COLS = 10;
const float BRICK_W = 60.f, BRICK_H = 25.f, BRICKS_Y = 100.f;
const float PADDLE_Y = 650.f, PADDLE_H = 14.f;
const int   PADDLE_W_NORMAL = 100, PADDLE_SPEED = 8;
const float BALL_SIZE = 12.f;
const int   START_LIVES = 3, POINTS_HIT = 10, POINTS_BREAK = 50;

// game states
const int MENU = 0, PLAYING = 1, GAME_OVER = 2;

// keys saved by handleInput (one press = one value)
const int KEY_NONE = 0, KEY_UP = 1, KEY_DOWN = 2, KEY_ENTER = 3,
KEY_SPACE = 4, KEY_ESC = 5;

// menu options
const int OPT_NEW = 0, OPT_CONTINUE = 1, OPT_EXIT = 2;

// ===================== GLOBAL GAME DATA =====================
int  gameState = MENU;
int  pressedKey = KEY_NONE;     // set in handleInput, used in updateGame
int  menuChoice = OPT_NEW;
bool exitGame = false;          // main() closes the window when this is true
bool noSavedGame = false;       // show "No saved game" on the menu
bool playerWon = false;         // true = YOU WIN, false = GAME OVER

int  bricks[ROWS][COLS];
int  paddleX = 250, paddleWidth = PADDLE_W_NORMAL;
int  ballX = 0, ballY = 0, dx = 0, dy = 0;
bool ballLaunched = false;
int  score = 0, lives = START_LIVES;

// ===================== YOUR LOGIC: function shells =====================

// Reads level.txt into bricks[][]. Return false if the file is missing.
bool loadLevel()
{
    // TODO: open level.txt, read rows and cols, then nested loops with fin >> bricks[r][c]
    return false;
}

// Puts the ball on the middle of the paddle, waiting for Space.
void resetBall()
{
    // TODO: ballX, ballY, dx, dy, ballLaunched
}

// New Game: score 0, 3 lives, reload bricks, paddle in the middle, ball on paddle.
void startNewGame()
{
    // TODO
}

void movePaddle()
{
    // TODO: Left/Right/A/D with sf::Keyboard::isKeyPressed, clamp to the window
}

// Space launches the ball: dy = -5, dx = 3 or -3 (rand() % 2)
void launchBall()
{
    // TODO
}

// Bricks the ball touches (use the 4 corners). Returns true if it touched a brick.
// Hint: this helper is called after the x step and after the y step.
bool hitBricks()
{
    // TODO: corners, row = (cy - 100) / 25, col = cx / 60, only inside the brick area
    // TODO: subtract a hit, give points (10 or 50), one hit per brick
    return false;
}

// Ball moving down and touching the paddle: put it on top, dy = -5, dx by the part hit.
void bounceOnPaddle()
{
    // TODO
}

// Moves the ball in two steps (x then y), walls, bricks, paddle, lost ball.
void moveBall()
{
    // TODO
}

int countBricks()
{
    // TODO: number of cells that are not 0
    return 0;
}

void saveGame()
{
    // TODO: savegame.txt -> "score lives" then 8 rows of 10 numbers
}

// Returns false if there is no saved game.
bool loadGame()
{
    // TODO: read savegame.txt, restore score, lives, bricks; paddle in the middle; resetBall()
    return false;
}

void clearSave()
{
    // TODO: open savegame.txt with ofstream and close it straight away
}

// ===================== UPDATE PER STATE =====================
void updateMenu()
{
    // TODO: Up/Down change menuChoice; Enter does New Game / Continue / Exit
    //       Continue with no save: noSavedGame = true and stay on the menu
    //       Exit: exitGame = true
}

void updatePlaying()
{
    // TODO: Esc -> saveGame() and back to MENU
    // TODO: movePaddle(); if not launched, ball follows paddle and Space launches
    // TODO: if launched, moveBall(); lost ball -> lives; win/lose checks
}

void updateEndScreen()
{
    // TODO: Enter -> clearSave() (if you have not already) and back to MENU
}

// ===================== DRAWING HELPERS (SFML only) =====================
void drawText(sf::RenderWindow& window, const sf::Font& font, const string& str,
    float x, float y, unsigned int size, sf::Color color = sf::Color::White)
{
    sf::Text text(font, str, size);
    text.setFillColor(color);
    text.setPosition({ x, y });
    window.draw(text);
}

void drawPaddle(sf::RenderWindow& window)
{
    sf::RectangleShape paddle({ (float)paddleWidth, PADDLE_H });
    paddle.setFillColor(sf::Color::Cyan);
    paddle.setPosition({ (float)paddleX, PADDLE_Y });
    window.draw(paddle);
}

void drawBall(sf::RenderWindow& window)
{
    sf::CircleShape ball(BALL_SIZE / 2);
    ball.setPosition({ (float)ballX, (float)ballY });
    window.draw(ball);
}

void drawHUD(sf::RenderWindow& window, const sf::Font& font)
{
    sf::RectangleShape strip({ (float)WIN_W, (float)HUD_H });
    strip.setFillColor(sf::Color(25, 25, 60));
    window.draw(strip);
    drawText(window, font, "SCORE " + to_string(score), 10.f, 10.f, 24);
    drawText(window, font, "LIVES " + to_string(lives), 460.f, 10.f, 24);
}

// TODO (Feature 2): draw the bricks with two nested loops.
// Use the pattern in Section 2.1 of the brief (one RectangleShape, colour by bricks[r][c]).
void drawBricks(sf::RenderWindow& window)
{
    // TODO
}

void drawMenu(sf::RenderWindow& window, const sf::Font& font)
{
    drawText(window, font, "BLOCK BUSTER", 150.f, 150.f, 48);
    const char* names[3] = { "New Game", "Continue", "Exit" };
    for (int i = 0; i < 3; i++)
    {
        sf::Color c = (i == menuChoice) ? sf::Color::Yellow : sf::Color::White;
        drawText(window, font, names[i], 230.f, 300.f + i * 60.f, 32, c);
    }
    if (noSavedGame)
        drawText(window, font, "No saved game", 215.f, 520.f, 24, sf::Color::Red);
}

void drawEndScreen(sf::RenderWindow& window, const sf::Font& font)
{
    string title = playerWon ? "YOU WIN" : "GAME OVER";
    drawText(window, font, title, 190.f, 250.f, 48);
    drawText(window, font, "Final score: " + to_string(score), 190.f, 340.f, 28);
    drawText(window, font, "Press Enter", 220.f, 420.f, 24);
}

// ===================== INPUT, UPDATE, DRAW =====================
void handleInput(sf::RenderWindow& window)
{
    pressedKey = KEY_NONE;   // forget last frame's key
    while (const optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
        else if (const auto* key = event->getIf<sf::Event::KeyPressed>())
        {
            if (key->code == sf::Keyboard::Key::Up)          pressedKey = KEY_UP;
            else if (key->code == sf::Keyboard::Key::Down)   pressedKey = KEY_DOWN;
            else if (key->code == sf::Keyboard::Key::Enter)  pressedKey = KEY_ENTER;
            else if (key->code == sf::Keyboard::Key::Space)  pressedKey = KEY_SPACE;
            else if (key->code == sf::Keyboard::Key::Escape) pressedKey = KEY_ESC;
        }
    }
}

void updateGame()
{
    switch (gameState)
    {
    case MENU:      updateMenu();      break;
    case PLAYING:   updatePlaying();   break;
    case GAME_OVER: updateEndScreen(); break;
    }
}

void drawGame(sf::RenderWindow& window, const sf::Font& font)
{
    window.clear(sf::Color(15, 15, 40));
    switch (gameState)
    {
    case MENU:
        drawMenu(window, font);
        break;
    case PLAYING:
        drawHUD(window, font);
        drawBricks(window);
        drawPaddle(window);
        drawBall(window);
        break;
    case GAME_OVER:
        drawEndScreen(window, font);
        break;
    }
    window.display();
}

int main()
{
    srand((unsigned)time(0));
    sf::RenderWindow window(sf::VideoMode({ WIN_W, WIN_H }), "Block Buster");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
        return 1;

    while (window.isOpen())
    {
        handleInput(window);
        updateGame();
        drawGame(window, font);
        if (exitGame) window.close();
    }
    return 0;
}