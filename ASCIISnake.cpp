// Copyright &copy FrenchToast //

#include <iostream>
#include <windows.h>
#include <time.h>
#include <conio.h>
using namespace std;

bool stop = 0;
bool lite = 1;
bool food = 0;
bool boostspeed = 1;
int speed = 3; // Speed can be changed
int framecount = 0;
int food_x,food_y;
const int m=10,n=20;
char field[m][n];

class Snake{
    public:
    struct Part
    {
        int x,y;
        Part* next;
    };
    Part*body,*tail;
    int x,y,vx,vy,size,speed;
    void MoveSnake();
    void AddPart();
    void RemovePart();
    void DeleteBody();
} snake;
void StartMenu();
void Loading();
void Init();
void KeySwitch();
void GameOver();
void GenerateFood();
void GenerateField();
void DrawField();

int main()
{
    Loading();
    StartMenu();
    while(!stop)
    {
        for(int i=0;i<2;++i)
        {
            if(kbhit()==true)
                KeySwitch();
            Sleep(100/snake.speed);
        }

        GenerateField();
        DrawField();
    }
}

void Snake::MoveSnake()
{
    Part* bodycheck=body;
    while(bodycheck)
    {
        if(bodycheck->x==snake.x&&bodycheck->y==snake.y)
        {
            GameOver();
            break;
        }
        bodycheck=bodycheck->next;
    }   
    
    // Check collision
    if(!lite&&(snake.x==0 || snake.x==n-1 ||  snake.y==0 || snake.y==m-1))
    {
        GameOver();
    }   
    
    // Hard mode
    else{
        if(snake.x==0 || snake.x==n-1)
        {
            snake.vx=snake.x==0? -1 : 1;
            snake.vy=0;
            snake.x=n-1-snake.x;
        }
        if(snake.y==0 || snake.y==m-1)
        {
            snake.vy=snake.y==0? -1 : 1;
            snake.vx=0;
            snake.y=m-1-snake.y;
        }
    }
    
    // Easy mode
    if(snake.x==food_x&&snake.y==food_y)
    {
        food=0;
        ++snake.size;
        AddPart();
        if(boostspeed)
            ++snake.speed;
    }
    else if(body)
    {
        AddPart();
        RemovePart();
    }
    snake.x+=snake.vx;
    snake.y+=snake.vy;
}

void Snake::AddPart()
{
    Part* NewPart = new Part;
    if(!body)
    {
        body=NewPart;
        tail=NewPart;
    }
    else
    {
        tail->next=NewPart;
        tail=NewPart;
    }
    NewPart->next=0;
    NewPart->x=snake.x;
    NewPart->y=snake.y;
}

void Snake::RemovePart()
{
    Part* Item=body;
    body=Item->next;
    delete Item;
}

void Snake::DeleteBody()
{
    Part* del = body;
    while(del)
    {
        body=del->next;
        delete del;
        del=body;
    }
}

void Init()
{
    snake.x = 1;
    snake.y = 1;
    snake.vx = 1;
    snake.vy = 0;
    snake.size = 0;
    snake.body = 0;
    snake.tail = 0;
    snake.speed=speed;
    GenerateField();
}

void StartMenu()
{
    system("cls");
    cout << "                              FrenchToast presents" <<endl;
    cout << "                                 |ASCII-----|" <<endl;
    cout << "                                 |-----SNAKE|" <<endl;
    cout << "                            Copyright 2019 FrenchToast" <<endl;
    cout << "-------------------------------------------------------------------------------" <<endl;
    cout << "Choose your difficulty!" <<endl;
    cout << "1 = Easy - Disabled wall, Infinite land" <<endl << "2 = Classic - Has walls enabled" <<endl << "Esc Key = Exit the game" <<endl;
    cout <<endl;
    char c=0;
    while(c!=27)
    {
        c=_getch();
        switch(c)
        {
            case '1':
                lite=1;
                c=27;
                break;
            case '2':
                lite=0;
                c=27;
                break;
            case 27: // ESC Key
                stop=1;
                break;
        }
    }

    Init();
}

void Loading()
{
    system("cls");
    system("color 0e");
    cout << "Version 1.2.3" <<endl;
    cout << "Made by FrenchToast" <<endl;
    cout << "--------------------------" <<endl;
    cout << "Loading..." <<endl; // Takes about a whopping 0.01 ms to boot because the file is like 6 KB so it's small.
    cout << "Done!" <<endl;
    cout << "--------------------------" <<endl;
    system("pause");
    Init();
}

void GameOver()
{
    snake.DeleteBody();
    system("cls");
    cout<< "Your score is... "<< snake.size << "! " << "Try again to beat your score!\n";
    cout << "-----------------------------------------------" <<endl;
    system("pause");
    StartMenu();
}

void KeySwitch()
{
    char c =_getch();
    switch(c)
    {
        case 27: stop=1; snake.DeleteBody(); break; // ESCAPE Key
        case 'w': if(snake.vx) {snake.vx=0;  snake.vy=-1;} break; // W Key //
        case 's': if(snake.vx) {snake.vx=0;  snake.vy=1;} break; // S Key //
        case 'a': if(snake.vy) {snake.vx=-1;  snake.vy=0;} break; // A Key //
        case 'd': if(snake.vy) {snake.vx=1;  snake.vy=0;} break; // D Key //
        default: break;
    }
}

void GenerateFood()
{
    bool error=1;
    while(error)
    {
        srand(time(NULL));
        food_x=1+rand()%(n-1);
        food_y=1+rand()%(m-1);
        if(field[food_y][food_x]==' ')
        {
            error=0;
        }
    }
    food=1;
}
void GenerateField()
{
    snake.MoveSnake();
    for(int i=0;i<m;++i)
    {
        for(int j=0;j<n;++j)
        {
            if(i==0 || i==m-1 || j==0 ||  j==n-1)
            {
                field[i][j]='#'; /** Wall sprite **/
            }
            else if(i==snake.y && j==snake.x)
            {
                field[i][j]='@'; // Snake head
            }
            else{
                field[i][j]=' '; // Empty spaces
            }
            if(food)
                field[food_y][food_x]='*'; // Food sprite
        }
    }
    Snake::Part* bodycheck=snake.body;
    while(bodycheck)
    {
        field[bodycheck->y][bodycheck->x]='O';
        bodycheck=bodycheck->next;
    }
    if(!food)
    {
        GenerateFood();
    }
}
void DrawField()
{
    system("cls");
    for(int i=0;i<m;++i)
    {
        for(int j=0;j<n;++j)
        {
            cout<<field[i][j];
        }
        cout <<endl;
    }

 }
 return 0;
}
