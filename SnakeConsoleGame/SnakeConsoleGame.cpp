#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <conio.h>
#include <ctime>
#include <vector>

bool gameOver;
const int width = 40;
const int height = 20;
int x, y, foodX, foodY, nTail, score;
std::vector<int> tailX;
std::vector<int> tailY;
enum eDirection
{
    STOP = 0,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

eDirection eDir;

void GenerateRandomFood()
{
    bool isEmptySpace;
    do
    {
        isEmptySpace = true;
        foodX = rand() % width;
        foodY = rand() % height;

        if (
            foodX == 0 || foodX == width - 1 || foodY == 0 || foodY == height - 1
            ||
            foodX == x || foodY == y
        )
            isEmptySpace = false;
        std::vector<int>::iterator it;
        for (it = tailX.begin(); it != tailX.end(); it++)
            if (foodX == *it)
                isEmptySpace = false;
        for (it = tailY.begin(); it != tailY.end(); it++)
            if (foodY == *it)
                isEmptySpace = false;
    } while (!isEmptySpace);
}

void Setup()
{
    eDir = STOP;
    gameOver = false;
    x = width / 2;
    y = height / 2;
    nTail = 0;
    score = 0;
    GenerateRandomFood();
}

void Draw()
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
    std::cout << "Score: " << score << std::endl;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0 || j == 0 || j == width - 1 || i == height - 1)
                std::cout << "#";
            else if (i == y && j == x)
                std::cout << "O";
            else if (i == foodY && j == foodX)
                std::cout << "%";
            else
            {
            	// if tail is printed, don't print " " again
                bool isPrint = false;
            	if(nTail > 0)
            	{
            		// loop through tail and display to screen
                    for(int k = 0; k < nTail; k++)
                    {
                        if (tailX.at(k) == j && tailY.at(k) == i)
                        {
                            std::cout << "o";
                            isPrint = true;
                        }
                    }
            	}
            	if(!isPrint)
					std::cout << " ";
            }
        }
        std::cout << "\n";
    }
}

void Input()
{
    if (_kbhit())
    {
        int ch = _getch();
        if (ch == 0 || ch == 224)
        {
            switch (_getch())
            {
            case 72: // UP
                eDir = UP;
                break;
            case 80: // DOWN
                eDir = DOWN;
                break;
            case 75: // LEFT
                eDir = LEFT;
                break;
            case 77: // RIGHT
                eDir = RIGHT;
                break;
            }
        }
    }
}

void Logic()
{
    // tail collision && tail shift logic
	if(nTail > 0)
	{
        int prevX, prevY, prevX2, prevY2;
		// record the first tail segment
        prevX = tailX.at(0);
        prevY = tailY.at(0);

		// make the first tail segment as head position
        tailX.at(0) = x;
        tailY.at(0) = y;
        std::vector<int>::iterator it;

		// loop through tail
        for (int i = 1; i < nTail; i++)
        {
        	// record second tail segment
            prevX2 = tailX.at(i);
            prevY2 = tailY.at(i);
        	// make second tail segment as first tail segment
            tailX.at(i) = prevX;
            tailY.at(i) = prevY;
        	// make second tail as first tail
            prevX = prevX2;
            prevY = prevY2;
        	// tail collision
            if (tailX.at(i) == x && tailY.at(i) == y)
            {
                gameOver = true;
                std::cout << "GAME OVER!" << std::endl;
            }
        }
	}
	
    switch (eDir)
    {
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    }

    // teleportable walls
    if (x <= 0)
        x = width - 1;
    else if (x >= width - 1)
        x = 1;
    else if (y <= 0)
        y = height - 1;
    else if (y >= height - 1)
        y = 1;

    // food collision
    if (x == foodX && y == foodY)
    {
        nTail++;
        tailX.push_back(x);
        tailY.push_back(y);
        score += 10;
        GenerateRandomFood();
    }
}


int main()
{
    srand(static_cast<unsigned> (time(nullptr)));
    Setup();
    while (!gameOver)
    {
        Draw();
        Input();
        Logic();
        Sleep(70);
    }
    return 0;
}
