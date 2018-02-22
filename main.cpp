#include <vector>
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <list>
#include <cstdlib>

using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void SetCursor()
{
CONSOLE_CURSOR_INFO cursorInfo;
GetConsoleCursorInfo(h, &cursorInfo);
cursorInfo.bVisible = false;
SetConsoleCursorInfo(h, &cursorInfo);
}

void Goto(short x, short y)
    {
        COORD c = { x, y };

        SetConsoleCursorPosition(h, c);
    }

    void TextColor(int color)
    {
        SetConsoleTextAttribute(h, color);
    }

class PlayerBullet
{
    int myX;
    int myY;
public:

    PlayerBullet(int X,int Y)
    {
        myX = X;
        myY = Y;
    }

    int getX()
    {
        return myX;
    }

    int getY()
    {
        return myY;
    }

    void setY(int givenY)
    {
        myY = givenY;
    }

    void Frame()
    {
        if(myY>1)
            {
                Goto(myX,myY);
                std::cout << ' ';
                TextColor(14);
                myY--;
                Goto(myX,myY);
                std::cout <<'!';
            }
        else
            {
                Goto(myX,myY);
                std::cout <<' ';
                delete this;
            }
    }
};

class Enemy
{
public:

    int myX;
    int myY;
    int startX;
    bool movingLeft = true;
    int framesSkipped = 0;

    Enemy(int X, int Y)
    {
        myX = X;
        myY = Y;
        startX = myX;

        Goto(X,Y);
        TextColor(12);
        std::cout<<'#';
    }
    ~Enemy()
    {

    }
    void MoveEnemy()
    {
        if(framesSkipped%4==0)
        {
        framesSkipped=1;
        if(movingLeft)
        {

            ChangePosition(myX-1,myY);
            myX--;
            if(myX <= startX-5)
            {
                movingLeft = false;
            }
        }
        else
        {

            ChangePosition(myX+1, myY);
            myX++;
            if(myX >= startX+5)
            {
                movingLeft = true;
                Goto(myX, myY);
                std::cout<<" ";
                myY++;

                if(myY >= 21)
                {

                }
            }
        }
        }
            framesSkipped++;
    }

    void ChangePosition(int X, int Y)
    {
        Goto(myX, myY);
        std::cout<<" ";

        Goto(X,Y);
        TextColor(12);
        std::cout<<char('#');
    }

    void Frame()
    {
        MoveEnemy();
    }
};

class EnemyProjectile
{
public:
    int myX;
    int myY;
    int framesSkipped = 0;
    EnemyProjectile(int X, int Y)
    {
        myX = X;
        myY = Y;
        Goto(myX,myY);
        std::cout <<'*';
    }

    void Frame()
    {
        if(framesSkipped%8 == 0)
        {
                framesSkipped=1;

                Goto(myX,myY);
                std::cout << ' ';

                myY++;

                Goto(myX,myY);
                TextColor(12);
                std::cout <<'*';
        }
        framesSkipped++;
    }
};

class SpaceInvaders
{
    public:
       int PlayerX;
       int PlayerY;
       int EnemySpawningFramesSkipped = 0;
       int Score = 0;

       vector<Enemy*> enemies;
       vector<EnemyProjectile*> enemyProjectiles;

       EnemyProjectile* newProjectile = nullptr;
       PlayerBullet* playerBullet = new PlayerBullet(2,1);

       bool gameOver = false;
       bool CanShoot = true;

    SpaceInvaders()
    {
        Start();
    }

    ~SpaceInvaders()
    {

    }

    void Start()
    {
        DrawBoard();
        PlayerX = 25;
        PlayerY = 22;
        UpdateScore();
        while(!gameOver)
        {
            Frame();
        }

    };

    void Frame()
    {
        InputController();
        MoveEnemies();
        MoveProjectiles();

        if(EnemySpawningFramesSkipped%300==0)
        {
            SpawnEnemies();
            EnemySpawningFramesSkipped = 1;
        }

        EnemySpawningFramesSkipped++;

        Sleep(40);
    }

    void SpawnEnemies()
    {
        Enemy* newEnemy = new Enemy(25,5);
        enemies.push_back(newEnemy);
        newEnemy = new Enemy(20,5);
        enemies.push_back(newEnemy);
        newEnemy = new Enemy(15,5);
        enemies.push_back(newEnemy);
        newEnemy = new Enemy(30,5);
        enemies.push_back(newEnemy);
    }

    void UpdateScore()
    {
        Goto(50,1);
        TextColor(2);
        std::cout << "SCORE: " << Score;
    }

    void MoveProjectiles()
    {
        int EnemyProjectileIndex = 0;
        if(enemyProjectiles.size()>0)
        for (EnemyProjectile* eb : enemyProjectiles)
        {
            eb->Frame();
            if(eb->myX == PlayerX && eb->myY == PlayerY)
            {
                gameOver = true;
                std::cout<<"YOU LOSE! :/ \n";
            }
            if(eb->myY>23)
            {
                Goto(eb->myX,eb->myY);
                cout<<' ';
                enemyProjectiles.erase(enemyProjectiles.begin()+EnemyProjectileIndex);
                delete &eb;
            }
            EnemyProjectileIndex++;
        }

        playerBullet->Frame();

        int EnemyIndex = 0;
        for(Enemy *enemy : enemies)
        {
            if(enemy->myX == playerBullet->getX() && enemy->myY == playerBullet->getY())
            {
                Goto(enemy->myX,enemy->myY);
                TextColor(12);
                std::cout<<'$';
                Sleep(50);

                Goto(enemy->myX,enemy->myY);
                TextColor(12);
                std::cout<<'@';
                Sleep(50);

                Goto(enemy->myX,enemy->myY);
                TextColor(12);
                std::cout<<'.';
                Sleep(50);
                Goto(enemy->myX,enemy->myY);
                std::cout<<' ';

                enemies.erase(enemies.begin()+EnemyIndex);
                delete &enemy;

                playerBullet->setY(1);

                Score+=50;
                UpdateScore();

                break;
            }

            if(playerBullet->getY()<2)
            {
                Goto(playerBullet->getX(),playerBullet->getY());
                std::cout <<' ';
                delete &playerBullet;
            }

            EnemyIndex++;
        }
    }

    void MoveEnemies()
    {
        if(enemies.size()>0)
            for (Enemy* e : enemies)
            {
                e->Frame();
                if(std::rand()%100+1 > 99)
                {
                    enemyProjectiles.push_back(new EnemyProjectile(e->myX,e->myY+1));
                }
            }
    }

    void MovePlayer(int newX)
    {
        if(newX < 47 && newX > 3)
        {
            Goto(PlayerX,PlayerY);
            TextColor(0);
            std::cout << ' ';
            Goto(newX, PlayerY);
            TextColor(10);
            std::cout << char(220);
            PlayerX = newX;
        }
    }

    void InputController()
    {
      if(_kbhit())
        {
            char key;
            key = _getch();

            switch (key)
            {
            case 'a':
                {
                    MovePlayer(PlayerX-1);
                    break;
                }
            case 'd':
                {
                    MovePlayer(PlayerX+1);
                    break;
                }
            case 's':
                {
                    Shoot();
                    break;
                }
            }
        }
    }


    void DrawBoard()
    {
        for(int u = 0;u < 50;u++)
            {
                TextColor(750);
                std::cout << char(219);
                TextColor(7);
            }

    for(int u = 1;u < 25;u++)
        {
            Goto(0,u);
            TextColor(750);
            std::cout << char(219);
            TextColor(7);
        }

    for(int u = 1;u < 25;u++)
        {
            Goto(49,u);
            TextColor(750);
            std::cout << char(219);
            TextColor(7);
        }

    for(int u = 0;u < 50;u++)
        {
            Goto(u,25);
            TextColor(750);
            std::cout << char(219);
            TextColor(7);
        }

    Goto(25,22);
    TextColor(10);
    std::cout<< char(220);
}

    void Shoot()
    {
        if(CanShoot)
        {
            Goto(PlayerX,PlayerY-1);
            TextColor(8);
            std::cout<< '!';
            playerBullet = new PlayerBullet(PlayerX,PlayerY-1);
            CanShoot=false;
        }
    }
};

int main()
{
    SetCursor();
    srand( time( NULL ) );
    SpaceInvaders myGame;
    _getch();
    return 0;
}
