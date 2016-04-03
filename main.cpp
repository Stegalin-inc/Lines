#include <SFML/Graphics.hpp>
#include <ctime>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "button.h"

using namespace sf;
using namespace std;

int x, y, shapeSize, countBalls;
int **field;
bool **wave;
bool **possibleMove;
int selected[2] = {-1,-1};
int scores = 0;

void initArrrays()
{
    srand(time(NULL));
    field = new int*[y];
    wave = new bool*[y];
    possibleMove = new bool*[y];
    for(int i=0;i<y;i++)
    {
        field[i] = new int[x];
        wave[i] = new bool[x];
        possibleMove[i] = new bool[x];
    }
    for(int i=0;i<y;i++)
    {
        for (int z=0;z<x;z++)
        {
            field[i][z] = 0;
            wave[i][z] = false;
            possibleMove[i][z] = false;
        }
    }
}

void loadSettings()
{
    std::fstream file;
    file.open("settings",ios_base::in);
    file >> x >> y >> shapeSize>> countBalls;
    x+=2; y+=2;
    file.close();
}

void putBalls()
{
    int countIterations = 0, randy, randx;
    while (countIterations != countBalls)
    {
        randy = 1+rand()%(y-2); randx = 1+rand()%(x-2);
        if (field[randy][randx] == 0)
        {
            field[randy][randx] = 1+rand()%7;
            ++countIterations;
            cout<<"field["<<randx<<"]["<<randy<<"] = "<<field[randy][randx]<<endl; /* DEBUG */
        }
    }
}

bool check()
{

    int countInLine = 1;
    for (int i = 1; i < (y-1); ++i)
    {
        for (int z = 1; z < (x-1); ++z)
        {
            if (field[i][z] == field[i][z+1] && field[i][z] != 0)
                ++countInLine;
            else
                countInLine = 1;
            //cout<<" countInLine = "<<countInLine; /* DEBUG */
            if(countInLine == 5)
            {
                field[i][z+1] = 0;
                for (int a = 0; a < 5; ++a)
                {
                    field[i][z-a] = 0;
                }
                countInLine = 0;
                scores += 5;
                return true;
            }
        }
    }

    for (int z = 1; z < (y-1); ++z)
    {
        for (int i = 1; i < (x-1); ++i)
        {
            if (field[i][z] == field[i+1][z] && field[i][z] != 0)
                ++countInLine;
            else
                countInLine = 1;
            //cout<<" countInLine = "<<countInLine; /* DEBUG */
            if(countInLine == 5)
            {
                field[i+1][z] = 0;
                for (int a = 0; a < 4; ++a)
                {
                    field[i-a][z] = 0;
                }
                countInLine = 0;
                scores += 5;
                return true;
            }
        }
    }
    system("cls");
    cout<<"Очки: "<<scores;
    return false;
}

bool possibles()
{
    for (int z = 1; z < (y+1); ++z)
    {
        for (int i = 1; (i+1) < x; ++i)
        {
            wave[i][z] = false;
            possibleMove[i][z] = false;
        }
    }

    bool isWave = false;

    if (selected[0] != -1 && selected[1] != -1)
    {
        wave[selected[0]][selected[1]] = true;
        isWave = true;
    }

    //cout<<"isWave = "<<isWave; /* DEBUG */

    for (int r = 0; r < 900; ++r)
    {
        isWave = false;
        for (int z = 1; z < (y-1); ++z)
        {
            for (int i = 1; i < (x-1); ++i)
            {
                if(wave[i][z])
                {
                    //cout<<"wave["<<i<<"]["<<z<<"] = "<<wave[i][z]; /* DEBUG */
                    isWave = true;

                    if(!possibleMove[i][z-1] && field[i][z-1] == 0) {possibleMove[i][z-1] = true; wave[i][z-1] = true; wave[i][z] = false;}
                    if(!possibleMove[i+1][z] && field[i+1][z] == 0) {possibleMove[i+1][z] = true; wave[i+1][z] = true; wave[i][z] = false;}
                    if(!possibleMove[i][z+1] && field[i][z+1] == 0) {possibleMove[i][z+1] = true; wave[i][z+1] = true; wave[i][z] = false;}
                    if(!possibleMove[i-1][z] && field[i-1][z] == 0) {possibleMove[i-1][z] = true; wave[i-1][z] = true; wave[i][z] = false;}
                }
            }
        }
    }
}

void newGame()
{
    initArrrays();
    countBalls = 10;
    putBalls();
}

void draw()
{

    RenderWindow window(sf::VideoMode(shapeSize*(x-2), shapeSize*(y-1)), "Lines");//, Style::Fullscreen);

    Texture texture;
    texture.loadFromFile("texture.png");
    Sprite sprite;
    sprite.setTexture(texture);
    countBalls = 3;

    Image icon;
    icon.loadFromFile("icon.bmp");
    window.setIcon(50,50,icon.getPixelsPtr());

    /* BUTTONS */

    Button quit;
    quit.newButton(90, 50, 10, shapeSize*(y-1)-40);
    quit.setLabel("выйти", "times.ttf");

    /* BUTTONS */

    while (window.isOpen())
    {
        Event event;
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f pos = window.mapPixelToCoords(pixelPos);

        while (window.pollEvent(event))
        {
           if (event.type == Event::Closed)
               window.close();

            if(event.type == Event::MouseButtonPressed)
            {
                if (selected[1] == int(pos.x/shapeSize)+1 && selected[0] == int(pos.y/shapeSize)+1)
                {
                    selected[1] = -1;
                    selected[0] = -1;
                } else if (selected[1] == -1 && selected[0] == -1)
                {
                    //cout<<int(pos.x/shapeSize)<<" "<<int(pos.y/shapeSize); /* DEBUG */
                    if (field[int(pos.y/shapeSize)+1][int(pos.x/shapeSize)+1] != 0)
                    {
                        selected[1] = pos.x/shapeSize+1;
                        selected[0] = pos.y/shapeSize+1;
                    }
                } else if (field[int(pos.y/shapeSize)+1][int(pos.x/shapeSize)+1] == 0 && possibleMove[int(pos.y/shapeSize)+1][int(pos.x/shapeSize)+1])
                {
                    int temp = field[int(pos.y/shapeSize)+1][int(pos.x/shapeSize)+1];
                    field[int(pos.y/shapeSize)+1][int(pos.x/shapeSize)+1] = field[selected[0]][selected[1]];
                    field[selected[0]][selected[1]] = temp;
                    selected[1] = -1;
                    selected[0] = -1;

                    if (!check())
                        putBalls();
                }
                //cout <<"In "<<selected[1]<<" "<<selected[0]<<" count = "<<field[selected[0]][selected[1]]; /* DEBUG */
                possibles();
            }
        }

        if(quit.isCursor(pos.x, pos.y) && event.type == Event::MouseButtonReleased)
            newGame();

        //cout <<pos.x<<" "<<pos.y<<endl; /* DEBUG */
        window.clear(sf::Color(0,0,0));

        for (int i = 1; i < (y-1); ++i)
        {
            for (int z = 1; z < (x-1); ++z)
            {
                switch (field[i][z])
                {
                case 0 :
                    {
                        sprite.setTextureRect(IntRect(shapeSize*3,shapeSize,shapeSize*4,shapeSize*2));
                        break;
                    }
                case 1 :
                    {
                        sprite.setTextureRect(IntRect(shapeSize,shapeSize*0,shapeSize*2,shapeSize));
                        break;
                    }
                case 2 :
                    {
                        sprite.setTextureRect(IntRect(shapeSize*2,shapeSize*0,shapeSize*3,shapeSize));
                        break;
                    }
                case 3 :
                    {
                        sprite.setTextureRect(IntRect(shapeSize*3,shapeSize*0,shapeSize*4,shapeSize));
                        break;
                    }
                case 4 :
                    {
                        sprite.setTextureRect(IntRect(shapeSize*0,shapeSize,shapeSize,shapeSize*2));
                        break;
                    }
                case 5 :
                    {
                        sprite.setTextureRect(IntRect(shapeSize,shapeSize,shapeSize*2,shapeSize*2));
                        break;
                    }
                case 6 :
                    {
                        sprite.setTextureRect(IntRect(shapeSize*2,shapeSize,shapeSize*3,shapeSize*2));
                        break;
                    }
                case 7 :
                    {
                        sprite.setTextureRect(IntRect(shapeSize*0,shapeSize*0,shapeSize,shapeSize));
                        break;
                    }
                default :
                    {
                        break;
                    }
                }

                if ((selected[0] == i) && (selected[1] == z))
                    sprite.setColor(Color(255,0,0,127));
                else if (possibleMove[i][z])
                    sprite.setColor(Color(0,255,0,255));
                else
                    sprite.setColor(Color(255,255,255));

                sprite.setPosition(Vector2f((z-1)*shapeSize,(i-1)*shapeSize));
                window.draw(quit.button);
                window.draw(sprite);
            }
        }
        window.display();
    }
}

int main()
{
    setlocale(LC_ALL, "");
    loadSettings();
    initArrrays();
    putBalls();
    draw();
    return 0;
}
