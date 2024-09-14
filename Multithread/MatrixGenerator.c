#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

typedef struct tParam
{
    int kolona;
    int head;
}param;

DWORD WINAPI doThread (LPVOID lpParam)
{
    param *pParametri = (param*) lpParam;
    int kolona = pParametri->kolona;
    int head = pParametri->head;
    HANDLE h;
    COORD coord;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    h = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(h,&csbiInfo);
    int visina = csbiInfo.srWindow.Bottom-csbiInfo.srWindow.Top+1;
    coord.X = kolona;
    int i, brisanje = 0;
    DWORD cWritten;

    while(1)
    {
        for(i=head;i>=0;i--)
        {
            coord.Y = i;
            int pozicija = head - i;
            if (pozicija >=0-brisanje && pozicija<=5-brisanje)
                FillConsoleOutputAttribute(h,0b00001111,1,coord,&cWritten);
            if (pozicija >=6-brisanje && pozicija<=10-brisanje)
                FillConsoleOutputAttribute(h,0b00001010,1,coord,&cWritten);
            if (pozicija >=11-brisanje && pozicija<=18-brisanje)
                FillConsoleOutputAttribute(h,0b00000010,1,coord,&cWritten);
            if (pozicija >=18-brisanje)
                FillConsoleOutputAttribute(h,0b00000000,1,coord,&cWritten);

            char c1 = rand()%65+33;
            FillConsoleOutputCharacter(h,c1,1,coord,&cWritten);
            Sleep(4);

        }
        if (++head>visina)
        {
            ++brisanje;
            --head;
        }
        if(brisanje>19)
        {
            brisanje = 0;
            head = 0;
            Sleep(2000);
        }
        Sleep(20);
    }
    return 0;
}

int main (int argc, char *argv[])
{
    srand(time(NULL));
    DWORD dwThread[40];
    HANDLE hThread[40];
    int i;
    int j;
    param tParametri;
    param *pParametri;

    for(i=0;i<40;i++)
    {
        tParametri.kolona = i*2;
        tParametri.head = rand()%30;
        pParametri = &tParametri;
        hThread[i] = CreateThread(NULL,0,doThread,(LPVOID)pParametri,0,&dwThread[i]);

    }

    while(1)
    {
        char ch = getch();
        if (ch=='a');
        {
        for(j=0;j<i;j++)
        CloseHandle(hThread[j]);
        break;
        }
    }
    printf("\nkraj rada");
    return 0;
}
