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
bool playerWon = false; // true = YOU WIN, false = GAME OVER
bool levelError = false;

int  bricks[ROWS][COLS];
int  paddleX = 250, paddleWidth = PADDLE_W_NORMAL;
int  ballX = 0, ballY = 0, dx = 0, dy = 0;
bool ballLaunched = false;
int  score = 0, lives = START_LIVES;

// ===================== YOUR LOGIC: function shells =====================

// Reads level.txt into bricks[][]. Return false if the file is missing.
bool loadLevel()
{
    ifstream fin("level.txt");
    if (!fin.is_open())
        return false;
    int rows, cols;
    fin >> rows >> cols;
    for (int r = 0; r < ROWS ; r++) 
    {
        for (int c = 0; c < COLS; c++)
        {
            fin >> bricks[r][c];
        }
    }
    return true;
}

// Puts the ball on the middle of the paddle, waiting for Space.
// Puts the ball on the middle of the paddle, waiting for Space.
void resetBall()
{
    ballX = paddleX + 44;
    ballY = 638;
    dx = 0, dy = 0;
    ballLaunched = false;

}

// New Game: score 0, 3 lives, reload bricks, paddle in the middle, ball on paddle.
void startNewGame()
{
    levelError = false;
    score = 0, lives = START_LIVES;
    paddleWidth = PADDLE_W_NORMAL;
    paddleX = (WIN_W - paddleWidth) / 2;
    if (!loadLevel())
    {
        levelError = true;
        return;
    }
    resetBall();
    gameState = PLAYING;
}

void movePaddle()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        paddleX -= PADDLE_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        paddleX += PADDLE_SPEED;
    }
    if (paddleX < 0)
    {
        paddleX = 0;
    }
    if (paddleX + paddleWidth > WIN_W)
    {
       paddleX = WIN_W - paddleWidth;
    }
}

void launchBall()
{
    dx = 3, dy = -5;
    if (rand() % 2 == 0)
    {
        dx = -3;
    }
    ballLaunched = true;
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
    if (dy < 0)
    {
        return;
   }
    if (ballX > paddleX + paddleWidth || ballX+BALL_SIZE<paddleX || ballY>PADDLE_Y + PADDLE_H || ballY + BALL_SIZE < PADDLE_Y)
    {
        return;
    }
    ballY = 638;
    dy = -5;
    int part = (ballX + 6 - paddleX) * 4 / paddleWidth;
    if (part < 0)
    {
        part = 0;
    }
    if (part > 3)
    {
        part = 3;
    }
    if (part == 0)
    {
        dx = -6;
    }
    if (part == 1)
    {
        dx = -3;
    }
    if (part == 2)
    {
        dx = 3;
    }
    if (part == 3)
    {
        dx = 6;
    }
}

// Moves the ball in two steps (x then y), walls, bricks, paddle, lost ball.
void moveBall()
{
    ballX = ballX + dx;
    if (ballX<0 || ballX + BALL_SIZE>WIN_W)
    {
        ballX = ballX - dx;
        dx = -dx;
    }
    ballY = ballY + dy;
    if (ballY < HUD_H)
    {
        ballY = ballY - dy;
        dy = -dy;
    }
    bounceOnPaddle();
    if (ballY > WIN_H)
    {
        resetBall();
    }
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
    if (pressedKey == KEY_ENTER && menuChoice == OPT_NEW)
    {
        startNewGame();
    }
    if (pressedKey == KEY_DOWN)
    {
        menuChoice += 1;
    }
    if (menuChoice > OPT_EXIT)
    {
        menuChoice = OPT_NEW;
    }
    if (pressedKey == KEY_UP)
    {
        menuChoice -= 1;
    }
    if (menuChoice < OPT_NEW)
    {
        menuChoice = OPT_EXIT;
    }
    if (pressedKey == KEY_ENTER && menuChoice == OPT_EXIT)
    {
        exitGame = true;
    }
}

void updatePlaying()
{
    movePaddle();
    if (!ballLaunched)
    {
      ballX = paddleX + 44;
        if (pressedKey == KEY_SPACE)
          {
            launchBall();
          }
    }
    else
    {
        moveBall();
    }
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

// draws the bricks which are alive.
void drawBricks(sf::RenderWindow& window)
{
    const float BRICK_W = 60.f, BRICK_H = 25.f;
    const float BRICKS_Y = 100.f;

   sf::Color colors[4] = { sf::Color(25, 25, 45),
   sf::Color::Green,
   sf::Color::Yellow, 
   sf::Color::Red }; 
   sf::RectangleShape brick({ BRICK_W - 2 , BRICK_H - 2 });
      for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
       {
            if (bricks[r][c] == 0)
                continue;
            brick.setFillColor(colors[bricks[r][c]]);
            brick.setPosition({ c * BRICK_W + 1, BRICKS_Y + r * BRICK_H + 1 });
            window.draw(brick);
        }
    }
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