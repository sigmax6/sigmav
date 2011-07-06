/*
*Designed by 游侠UFO @ 2008.2
*版本 v1.0
*/
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <vector>
#include <string>
#include <queue>
#include <cstdlib>
#include <ctime>
using namespace std;
class BoardClass
{
private:
    static const string Model[13];
    static const int POINTER=12, GROUND=11, MINE=10, FLAG=9;
    short StartRow, StartCol, Size;
    unsigned MineCount, FlagCount;
    struct PointerStruct
    {
        short Row, Col;
    } Pointer;
    vector<vector<bool> > Opened;
    vector<vector<int> > Data;
    BOOL GotoConsoleXY(short Row, short Col);
    BOOL GotoBoardXY(short Row, short Col);
    void UpdateFlagCount();
    void Open();
    void Win();
    void Lost();
public:
    BoardClass(short Row, short Col, short Size, unsigned Count);
    void Move(int Direction);
    int Test();
    bool Mark();
} ;
const string BoardClass::Model[13]={"□", "１", "２", "３", "４", "５", "６", "７", "８", "Ｆ", "¤", "■", "◢"};
BOOL BoardClass::GotoConsoleXY(short Row, short Col)
{
    HANDLE Stdout=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Position={Col, Row};

    return SetConsoleCursorPosition(Stdout, Position);
}
BOOL BoardClass::GotoBoardXY(short Row, short Col)
{
    if (0<=Row && Row<Size && 0<=Col && Col<Size)
    {
        Row=StartRow+Row;
        Col=StartCol+Col*2;
        return GotoConsoleXY(Row, Col);
    }
    else return 0;
}
void BoardClass::UpdateFlagCount()
{
    GotoConsoleXY(StartRow-2, StartCol-1);
    cout<<"Flags : "<<FlagCount<<"        "<<flush;
    GotoConsoleXY(0, 0);
}
void BoardClass::Open()
{
    queue<pair<short, short> > Queue;
    Queue.push(make_pair(Pointer.Row, Pointer.Col));
    Opened[Pointer.Row][Pointer.Col]=true;
    while (!Queue.empty())
    {
        short R=Queue.front().first, C=Queue.front().second;
        Queue.pop();
        GotoBoardXY(R, C);
        cout<<Model[Data[R][C]%100]<<flush;
        GotoConsoleXY(0, 0);
        if (Data[R][C]==0)
        {
            const pair<short, short> Table[8]={    make_pair(R-1, C-1),
                                                make_pair(R-1, C),
                                                make_pair(R-1, C+1),
                                                make_pair(R, C-1),
                                                make_pair(R, C+1),
                                                make_pair(R+1, C-1),
                                                make_pair(R+1, C),
                                                make_pair(R+1, C+1)    };

            for (int i=0; i<8; i++)
                if (0<=Table[i].first && Table[i].first<Size &&
                    0<=Table[i].second && Table[i].second<Size &&
                    !Opened[Table[i].first][Table[i].second])
                {
                    Queue.push(make_pair(Table[i].first, Table[i].second));
                    Opened[Table[i].first][Table[i].second]=true;
                }
        }
    }
}
void BoardClass::Win()
{
    for (int i=0; i<Size; i++)
        for (int j=0; j<Size; j++)
            if (Data[i][j]==MINE)
            {
                GotoBoardXY(i, j);
                cout<<Model[FLAG]<<endl;
                GotoConsoleXY(0, 0);
            }
    GotoConsoleXY(StartRow-2, StartCol-1);
    cout<<"(^_^)  你赢了！    "<<flush;
}
void BoardClass::Lost()
{
    for (int i=0; i<Size; i++)
        for (int j=0; j<Size; j++)
            if (Data[i][j]%100==FLAG)
            {
                if (Data[i][j]/100==MINE)
                {
                    GotoBoardXY(i, j);
                    cout<<Model[MINE]<<endl;
                    GotoConsoleXY(0, 0);
                }
                else
                {
                    GotoBoardXY(i, j);
                    cout<<"×"<<endl;
                    GotoConsoleXY(0, 0);
                }
            }
            else if (Data[i][j]==MINE)
            {
                GotoBoardXY(i, j);
                cout<<Model[MINE]<<endl;
                GotoConsoleXY(0, 0);
            }
    GotoConsoleXY(StartRow-2, StartCol-1);
    cout<<"(T_T)  你踩到地雷了！    "<<flush;
}
BoardClass::BoardClass(short Row, short Col, short Size, unsigned Count) :    StartRow(Row),
                                                                            StartCol(Col),
                                                                            Size(Size),
                                                                            MineCount(Count)
{
    FlagCount=MineCount;
    Pointer.Row=0;
    Pointer.Col=0;
    Opened=vector<vector<bool> >(Size, vector<bool>(Size, false));
    Data=vector<vector<int> >(Size, vector<int>(Size, 0));
    srand(unsigned(time(NULL)));
    for (unsigned i=0; i<MineCount; i++)
    {
        short R, C;
        do
        {
            R=rand()%Size;
            C=rand()%Size;
        } while(Data[R][C]!=0);
        Data[R][C]=MINE;
        if (R-1>=0 && C-1>=0) Data[R-1][C-1]++;
        if (R-1>=0) Data[R-1][C]++;
        if (R-1>=0 && C+1<Size) Data[R-1][C+1]++;
        if (C-1>=0) Data[R][C-1]++;
        if (C+1<Size) Data[R][C+1]++;
        if (R+1<Size && C-1>=0) Data[R+1][C-1]++;
        if (R+1<Size) Data[R+1][C]++;
        if (R+1<Size && C+1<Size) Data[R+1][C+1]++;
    }
    short R=StartRow-1;
    system("title 扫雷v1.0——Designed by 游侠UFO @ 2008.2");
    UpdateFlagCount();
    GotoConsoleXY(R, StartCol-2);
    cout<<"┏"<<flush;
    for (short i=0; i<Size; i++) cout<<"━"<<flush;
    cout<<"┓"<<flush;
    for (short i=0; i<Size; i++)
    {
        GotoConsoleXY(++R, StartCol-2);
        cout<<"┃"<<flush;
        for (short i=0; i<Size; i++) cout<<Model[GROUND]<<flush;
        cout<<"┃"<<flush;
    }
    GotoConsoleXY(++R, StartCol-2);
    cout<<"┗"<<flush;
    for (short i=0; i<Size; i++) cout<<"━"<<flush;
    cout<<"┛"<<flush;
    GotoBoardXY(0, 0);
    cout<<Model[POINTER]<<flush;
    GotoConsoleXY(++R, StartCol-1);
    cout<<"Designed by 游侠UFO @ 2008.2"<<flush;
    GotoConsoleXY(0, 0);
}
void BoardClass::Move(int Direction)
{
    const pair<short, short> Table[4]={    make_pair(Pointer.Row-1>=0 ? Pointer.Row-1 : Pointer.Row, Pointer.Col),
                                        make_pair(Pointer.Row, Pointer.Col+1<Size ? Pointer.Col+1 : Pointer.Col),
                                        make_pair(Pointer.Row+1<Size ? Pointer.Row+1 : Pointer.Row, Pointer.Col),
                                        make_pair(Pointer.Row, Pointer.Col-1>=0 ? Pointer.Col-1 : Pointer.Col)    };
    GotoBoardXY(Pointer.Row, Pointer.Col);
    if (Data[Pointer.Row][Pointer.Col]%100==FLAG) cout<<Model[FLAG]<<flush;
    else if (Opened[Pointer.Row][Pointer.Col]) cout<<Model[Data[Pointer.Row][Pointer.Col]]<<flush;
    else cout<<Model[GROUND]<<flush;
    Pointer.Row=Table[Direction].first;
    Pointer.Col=Table[Direction].second;
    GotoBoardXY(Pointer.Row, Pointer.Col);
    cout<<Model[POINTER]<<flush;
    GotoConsoleXY(0, 0);
}
int BoardClass::Test()
{
    if (!Opened[Pointer.Row][Pointer.Col])
    {
        if (Data[Pointer.Row][Pointer.Col]==MINE)
        {
            Lost();
            return -1;
        }
        else if (Data[Pointer.Row][Pointer.Col]%100==FLAG) return 0;
        else
        {
            Open();
            for (short i=0; i<Size; i++)
                for (short j=0; j<Size; j++)
                    if (!Opened[i][j] && Data[i][j]!=MINE && Data[i][j]%100!=FLAG) return 0;
            Win();
            return 1;
        }
    }
    else return 0;
}
bool BoardClass::Mark()
{
    if (Data[Pointer.Row][Pointer.Col]%100==FLAG)
    {
        FlagCount++;
        UpdateFlagCount();
        Data[Pointer.Row][Pointer.Col]=Data[Pointer.Row][Pointer.Col]/100;
        GotoBoardXY(Pointer.Row, Pointer.Col);
        if (Opened[Pointer.Row][Pointer.Col]) cout<<Model[Data[Pointer.Row][Pointer.Col]]<<flush;
        else cout<<Model[GROUND]<<flush;
        GotoConsoleXY(0, 0);
        return true;
    }
    else if (FlagCount>0 && !Opened[Pointer.Row][Pointer.Col])
    {
        FlagCount--;
        UpdateFlagCount();
        Data[Pointer.Row][Pointer.Col]=FLAG+100*Data[Pointer.Row][Pointer.Col];
        GotoBoardXY(Pointer.Row, Pointer.Col);
        cout<<Model[FLAG]<<flush;
        GotoConsoleXY(0, 0);
        return true;
    }
    else return false;
}
int main()
{
    BoardClass *Board;
    int Key, Difficulty=0, MineCount=20;

    Board=new BoardClass(2, 20, 20, MineCount);
    do
    {
        Key=getch();
        if (Key=='j' || Key=='J')
        {
            int Win=Board->Test();
            if (Win!=0)
            {
                system("pause");
                system("cls");
                if (Win==1)
                {
                    Difficulty++;
                    Difficulty=Difficulty%3;
                }
                delete Board;
                Board=new BoardClass(2, 20, 20, MineCount*(Difficulty+1));
            }
        }
        else if (Key=='k' || Key=='K') Board->Mark();
        else if (Key=='w' || Key=='W') Board->Move(0);
        else if (Key=='d' || Key=='D') Board->Move(1);
        else if (Key=='s' || Key=='S') Board->Move(2);
        else if (Key=='a' || Key=='A') Board->Move(3);
    } while(Key!=27);
    delete Board;
    return 0;
}
