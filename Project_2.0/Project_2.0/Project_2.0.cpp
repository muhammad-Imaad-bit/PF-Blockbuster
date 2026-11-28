// Block Buster - starter skeleton (SFML 3)
// SFML scaffolding is provided. Every TODO is YOUR game logic.
#include <SFML/Graphics.hpp>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <optional>
#include <string>
using namespace std;

const int   WIN_W = 600, WIN_H = 700, HUD_H = 50;
const int   ROWS = 8, COLS = 10;
const float BRICK_W = 60.f, BRICK_H = 25.f, BRICKS_Y = 100.f;
const float PADDLE_Y = 650.f, PADDLE_H = 14.f;
const int   PADDLE_W_NORMAL = 100, PADDLE_SPEED = 8;
const float BALL_SIZE = 12.f;
const int   START_LIVES = 3, POINTS_HIT = 10, POINTS_BREAK = 50;

const int MENU = 0, PLAYING = 1, GAME_OVER = 2;
const int KEY_NONE = 0, KEY_UP = 1, KEY_DOWN = 2, KEY_ENTER = 3,
KEY_SPACE = 4, KEY_ESC = 5;


const int OPT_NEW = 0, OPT_CONTINUE = 1, OPT_EXIT = 2;
int  gameState = MENU;
bool wideActive = false;
int wideFrameLeft = 0;

int  pressedKey = KEY_NONE;     
int  menuChoice = OPT_NEW;
bool exitGame = false;          
bool noSavedGame = false;       
bool playerWon = false; 
bool levelError = false;

int  bricks[ROWS][COLS];
int  paddleX = 250, paddleWidth = PADDLE_W_NORMAL;
int  ballX = 0, ballY = 0, dx = 0, dy = 0;
bool ballLaunched = false;
int  score = 0, lives = START_LIVES;
bool powerUpActive = false;
int powerUpType = 0;
float powerUpX = 0, powerUpY = 0;
const float POWERUP_SIZE = 20.f;

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

void resetBall()
{
    ballX = paddleX + 44;
    ballY = 638;
    dx = 0, dy = 0;
    ballLaunched = false;

}
void checkpowerupCollision()
{
    if (!powerUpActive)
        return;
    bool horizontalCollision = powerUpX + POWERUP_SIZE >= paddleX && powerUpX <= paddleWidth + paddleX;
    bool verticalCollision = powerUpY + POWERUP_SIZE >= PADDLE_Y && powerUpY <= PADDLE_Y + PADDLE_H;
    if (horizontalCollision && verticalCollision)
    {
        if (powerUpType == 1)
        {
            wideActive = true;
            paddleWidth = 150;
            wideFrameLeft = 600;
            if (paddleX + paddleWidth > WIN_W)
            {
                paddleX = WIN_W - paddleWidth;
            }
        }
        else if (powerUpType == 2)
        {
            lives++;
        }
        powerUpActive = false;
    }
}
void UpdateWidthPaddle()
{
    if (!wideActive)
    {
        return;
    }
    wideFrameLeft--;
    if (wideFrameLeft <= 0)
    {
        wideActive = false;
        paddleWidth = PADDLE_W_NORMAL;

        if (paddleX + paddleWidth > WIN_W)
            paddleX = WIN_W - paddleWidth;
    }
}


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
bool hitBricks()
{
    bool hit=false ;
    int hitRow = -1, hitCol = -1;
    int s = (int )BALL_SIZE-1;
    int cx[4] = { ballX, ballX + s, ballX,     ballX + s };
    int cy[4] = { ballY, ballY,     ballY + s, ballY + s };
    for (int i = 0; i < 4; i++)
    {
        if (cx[i] >= 0 && cx[i] < 600 && cy[i] >= 100 && cy[i] < 300)
        {
            int row = (cy[i]-100)/25;
            int col = cx[i] / 60;
            if (bricks[row][col] > 0 && !(row == hitRow && col == hitCol))
            {
                bricks[row][col]--;
                hitRow = row;
                hitCol = col;
                hit = true;

                if (bricks[row][col] == 0)
                    score += POINTS_BREAK;
                else
                    score += POINTS_HIT;
            }
        }
    }


    return hit;
}

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

int countBricks()
{
    int counts = 0;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (bricks[i][j] > 0)
            {
                counts++;
            }
        }
    }

    return counts;
}
void moveBall()
{
    ballX = ballX + dx;
    if (ballX<0 || ballX + BALL_SIZE>WIN_W)
    {
        ballX = ballX - dx;
        dx = -dx;
    }
    ballY = ballY + dy;
    if (ballX < 0 || ballX + BALL_SIZE > WIN_W || hitBricks())
    {
        ballY = ballY - dy;
        dy = -dy;
    }
    bounceOnPaddle();
    if (ballY > WIN_H)
    {   
        lives--;
        if (lives <= 0)
        {
            playerWon = false;
            gameState = GAME_OVER;
        }
        else
        resetBall();
    }
    if (countBricks() == 0)
    {
        playerWon = true;
        gameState = GAME_OVER;
    }

}



void saveGame()
{
    ofstream fout("savegame.txt");
    if (!fout)
        return ;
    fout << score << " "  << lives<<endl;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            fout << bricks[i][j] << " ";
        }
        fout << endl;
    }
    fout.close();
}

bool loadGame()
{
    ifstream fin("savegame.txt");
    if (!fin)
        return false;
    else if (!(fin >> score >> lives))
        return false;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            fin >> bricks[i][j];
        }

    }
    fin.close();
    paddleX = 250;
    ballX = paddleX + 44, ballY = 638;
    ballLaunched = false;
    return true;
}

void clearSave()
{
    ofstream fout("savegame.txt");
    fout.close();
}


void updateMenu()
{
    if (pressedKey == KEY_ENTER && menuChoice == OPT_NEW)
    {
        startNewGame();
    }
    if (pressedKey == KEY_ENTER && menuChoice == OPT_CONTINUE)
    {
        if (loadGame())
        {
            gameState = PLAYING;
        }
        else
        {
            noSavedGame = true;
        }
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
    checkpowerupCollision();
    UpdateWidthPaddle();

    if (pressedKey == KEY_ESC)
    {
        saveGame();
        gameState = MENU;
    }
}

void updateEndScreen()
{
    if (pressedKey == KEY_ENTER)
    {
        clearSave();
        noSavedGame = false;
        levelError = false;
        menuChoice = OPT_NEW;
        gameState = MENU;
    }

}


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
    if (levelError)
        drawText(window, font, "Level file missing!", 190.f, 560.f, 24, sf::Color::Red);
}

void drawEndScreen(sf::RenderWindow& window, const sf::Font& font)
{
    string title = playerWon ? "YOU WIN" : "GAME OVER";
    drawText(window, font, title, 190.f, 250.f, 48);
    drawText(window, font, "Final score: " + to_string(score), 190.f, 340.f, 28);
    drawText(window, font, "Press Enter", 220.f, 420.f, 24);
}


void handleInput(sf::RenderWindow& window)
{
    pressedKey = KEY_NONE;   
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