#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define JOY_V 35
#define JOY_H 34
int joyVer;
int joyHor;


// Game variables

bool gameover;

const int width = 19;
const int height = 5;

int x, y, fruitX, fruitY, score;

int tailX[10], tailY[10]; // snake coordinates

int nTail;

enum eDirecton
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
}; // Controls

eDirecton dir;

Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT, &Wire, -1);

void setup(){
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println(F("Display started"));
  }
  display.display();
  delay(5000);
  display.clearDisplay();
  // Joystick
  pinMode(JOY_V, INPUT_PULLUP);
  pinMode(JOY_H, INPUT_PULLUP);


  // Setup game
  gameover = false;
  dir = STOP;
  x = width / 2;
  y = height / 2;
  // display fruit in a random place
  fruitX = rand() % width; 
  fruitY = rand() % height;
  score = 0;
}
void Draw()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

    for (int i = 0; i < width + 2; i++)
        display.print("#");
    display.print("\n");
    for (int i = 0; i < height; i++)
    {

        for (int j = 0; j < width; j++)
        {

            if (j == 0)
              display.print("#");
            if (i == y && j == x)
              display.print("*");// Snake tail
            else if (i == fruitY && j == fruitX)
              display.print("%");// Fruit
            else
            {
                bool print = false;
                for (int k = 0; k < nTail; k++)
                {
                    if (tailX[k] == j && tailY[k] == i)
                    {
                        display.print("*");
                        print = true;
                    }
                }
                if (!print)
                  display.print(" ");
            }

            if (j == width - 1)
              display.print("#");
        }
        display.print("\n");
    }

    for (int i = 0; i < width + 2; i++)
      display.print("#");

    display.print("\n");
    display.print("Score: ");
    display.print(score);
    display.display();
    display.setCursor(0,0);
}
void algorithm()
{

    int prevX = tailX[0];

    int prevY = tailY[0];

    int prev2X, prev2Y;

    tailX[0] = x;

    tailY[0] = y;

    for (int i = 1; i < nTail; i++)
    {

        prev2X = tailX[i];

        prev2Y = tailY[i];

        tailX[i] = prevX;

        tailY[i] = prevY;

        prevX = prev2X;

        prevY = prev2Y;
    }

    switch (dir)
    {

    case LEFT:

        x--;

        break;

    case RIGHT:

        x++;

        break;

    case UP:

        y--;

        break;

    case DOWN:

        y++;

        break;

    default:

        break;
    }

    if (x >= width)
        x = 0;
    else if (x < 0)
        x = width - 1;

    if (y >= height)
        y = 0;
    else if (y < 0)
        y = height - 1;

    for (int i = 0; i < nTail; i++)

        if (tailX[i] == x && tailY[i] == y)
            gameover = true;

    if (x == fruitX && y == fruitY)
    {

        score += 10;

        fruitX = rand() % width;

        fruitY = rand() % height;

        nTail++;
    }
}
void Input()
{
  if (joyVer > 2200 && joyVer <= 4095)
  {
    dir = UP;
  }
  else if (joyVer <= 1900)
  {
    dir = DOWN;
  }
  else if (joyHor > 2200 && joyHor <= 4095)
  {
    dir = LEFT;
  }
  else if (joyHor <= 1900)
  {
    dir = RIGHT;
  }
  else
  {
    // Nothing
    ;
  }
}

void loop(){
  // Read joystick values
  joyVer = analogRead(JOY_V);
  joyHor = analogRead(JOY_H);

  Serial.printf("VER: %d | HOR: %d\n",joyVer, joyHor);

  if(!gameover){
    Draw();
    Input();
    algorithm();
    delay(100);
  }else{
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.print("Game Over");
    display.display();
    delay(5000);
    gameover = false;
    nTail = 0;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    dir = STOP;
  }
}