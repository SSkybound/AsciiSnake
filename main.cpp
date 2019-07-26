// Copyright &copy French Toast //
#include <iostream>
#include <windows.h>
#include <time.h>
#include <conio.h>
using namespace std;

bool stop=0;
bool lite=1;
bool food=0;
bool boostspeed=1;
int speed=3; // Speed can be changed
int framecount=0;
int food_x,food_y;
const int m=10,n=20;
char field[m][n];

class Snake{
    public:
    struct Part{
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

int main(){
    Loading();
    StartMenu();
    while(!stop){
        for(int i=0;i<2;++i){
            if(kbhit()==true)
                KeySwitch();
            Sleep(100/snake.speed);
        }

        GenerateField();
        DrawField();
    }
}

void Snake::MoveSnake(){
    Part* bodycheck=body;
    while(bodycheck){
        if(bodycheck->x==snake.x&&bodycheck->y==snake.y){
            GameOver();
            break;
        }
        bodycheck=bodycheck->next;
    }    // Check collision
    if(!lite&&(snake.x==0 || snake.x==n-1 ||  snake.y==0 || snake.y==m-1)){
        GameOver();
    }    // Hard mode
    else{
        if(snake.x==0 || snake.x==n-1){
            snake.vx=snake.x==0? -1 : 1;
            snake.vy=0;
            snake.x=n-1-snake.x;
        }
        if(snake.y==0 || snake.y==m-1){
            snake.vy=snake.y==0? -1 : 1;
            snake.vx=0;
            snake.y=m-1-snake.y;
        }
    }    // Easy mode
    if(snake.x==food_x&&snake.y==food_y){
        food=0;
        ++snake.size;
        AddPart();
        if(boostspeed)
            ++snake.speed;
    }
    else if(body){
        AddPart();
        RemovePart();
    }
    snake.x+=snake.vx;
    snake.y+=snake.vy;
}
void Snake::AddPart(){
    Part* NewPart = new Part;
    if(!body){
        body=NewPart;
        tail=NewPart;
    }
    else{
        tail->next=NewPart;
        tail=NewPart;
    }
    NewPart->next=0;
    NewPart->x=snake.x;
    NewPart->y=snake.y;
}
void Snake::RemovePart(){
    Part* Item=body;
    body=Item->next;
    delete Item;
}
void Snake::DeleteBody(){
    Part* del = body;
    while(del){
        body=del->next;
        delete del;
        del=body;
    }
}
void Init(){
    snake.x=1;
    snake.y=1;
    snake.vx=1;
    snake.vy=0;
    snake.size=0;
    snake.body=0;
    snake.tail=0;
    snake.speed=speed;
    GenerateField();
}
void StartMenu(){
    system("cls");
    cout << "                             French Toast presents" <<endl;
    cout << "                                 ASCIISnake" <<endl;
    cout << "                                Moddable Ver.\n";
    cout << "--------------------------------------------------------------------------------" <<endl;
    cout <<endl;
    cout << "Choose your difficulty!" <<endl;
    cout << "1 = Easy" <<endl << "2 = Classic" <<endl << "Esc Key = Exit" <<endl;
    char c=0;
    while(c!=27){
        c=_getch();
        switch(c){
            case '1':
                lite=1;
                c=27;
                break;
            case '2':
                lite=0;
                c=27;
                break;
            case 27: //ESC
                stop=1;
                break;
        }
    }

    Init();
}

void Loading(){
    system("cls");
    cout << "Version 1.2.3" <<endl;
    cout << "Made by French Toast" <<endl;
    cout << "--------------------------" <<endl;
    cout << "Loading..." <<endl;
    cout << "Done!" <<endl;
    cout << "--------------------------" <<endl;
    system("pause");
    Init();
}
void GameOver(){
    snake.DeleteBody();
    system("cls");
    cout<< "Your score is... "<< snake.size << "! "<<"Want to play again?\n";
    cout << "-----------------------------------------------" <<endl;
    system("pause");
    StartMenu();
}
void KeySwitch(){
    char c =_getch();
    switch(c){
        case 27: stop=1; snake.DeleteBody(); break; //ESC
        case 72: if(snake.vx) {snake.vx=0;  snake.vy=-1;} break; //up
        case 80: if(snake.vx) {snake.vx=0;  snake.vy=1;} break; //down
        case 75: if(snake.vy) {snake.vx=-1;  snake.vy=0;} break; //left
        case 77: if(snake.vy) {snake.vx=1;  snake.vy=0;} break; //right
        default: break;
    }
}
void GenerateFood(){
    bool error=1;
    while(error){
        srand(time(NULL));
        food_x=1+rand()%(n-1);
        food_y=1+rand()%(m-1);
        if(field[food_y][food_x]==' ') {
            error=0;
        }
    }
    food=1;
}
void GenerateField(){
    snake.MoveSnake();
    for(int i=0;i<m;++i){
        for(int j=0;j<n;++j){
            if(i==0 || i==m-1 || j==0 ||  j==n-1){
                field[i][j]='#';
            }
            else if(i==snake.y && j==snake.x){
                field[i][j]='@';
            }
            else{
                field[i][j]=' ';
            }
            if(food)
                field[food_y][food_x]='o';
        }
    }
    Snake::Part* bodycheck=snake.body;
    while(bodycheck){
        field[bodycheck->y][bodycheck->x]='*';
        bodycheck=bodycheck->next;
    }
    if(!food){
        GenerateFood();
    }
}
void DrawField(){
    system("cls");
    for(int i=0;i<m;++i){
        for(int j=0;j<n;++j){
            cout<<field[i][j];
        }
        cout <<endl;
    }

}
