#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

typedef struct ljudi
{
    char *ime;
    int godiste;
}Ljudi;

Ljudi ljudi[100];
char *imena[] = {"petar","marko","ana","dragana","jelena","jovan","ivana","igor","bojan","jasmina"};
int flag = 1;
int i;
int generisanjeListeEnded = 0;
int sortingOn = 0;
DWORD WINAPI generisanjeListe(LPVOID lpParam)
{
    time_t t;
    srand((unsigned)time(&t));

    int r,ii,br,l,godiste,m;
    char str[10];
    char c[4];
    for(ii = 0; ii < 100; ii++)
    {
        r = rand() % 10;
        br = rand() % 100;

        l = strlen(imena[r]);
        ljudi[ii].ime = (char*)malloc(l*sizeof(char)+4);
        sprintf(c,"%d",br);
        strcpy(ljudi[ii].ime, imena[r]);
        strcat(ljudi[ii].ime,c);
        printf("%s i:%d\n",ljudi[ii].ime,ii);

        godiste = rand() % (1999 - 1970 + 1) + 1970;
        ljudi[ii].godiste = godiste;
        m = ii; //pomocna promenljiva za ispisivanje %
        i = ii; //broj generisanih korisnika
        sprintf(str,"%d %%",m+1);
        SetConsoleTitle(str);

        Sleep(100);
    }
    generisanjeListeEnded = 1;

    return 0;
}
DWORD WINAPI sortThread(LPVOID lpParam)
{
    while(1)
    {
        if(generisanjeListeEnded == 1)
        {
            sortingOn = 1;
            int k, j;
            int swapped;
            for (k = 0; k < i-1; k++)
            {
                swapped = 0;
                for (j = 0; j < i-k-1; j++)
                {
                    if (ljudi[j].ime[0] > ljudi[j+1].ime[0])
                    {
                        swap(&ljudi[j], &ljudi[j+1]);
                        swapped = 1;
                    }
                }

                // IF no two elements were swapped by inner loop, then break
                if (swapped == 0)
                {
                    printf("Lista je uspesno sortirana\n");
                    break;
                }
            }
            break;
        }
    }
    return 0;
}

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

int main()
{

    DWORD gl,st;
    HANDLE glID,stID;

    gl = CreateThread(NULL,0,generisanjeListe,(LPVOID)0,0,&glID);
    st = CreateThread(NULL,0,sortThread,(LPVOID)0,0,&stID);

    char c;

    while(flag)
    {
        switch (c=getch())
        {
            case 'k'://prekid
                {
                    if(generisanjeListeEnded)
                    {
                        CloseHandle(gl);
                        if(sortingOn)
                            CloseHandle(st);

                    }else SuspendThread(gl);
                }
                break;
            case 's'://statistika
                {
                    int j;
                    for(j = 0; j < i;j++)
                    {
                        printf("%12s %100d godiste\n",ljudi[j].ime, ljudi[j].godiste);
                    }
                }break;
            case 'n'://nastavak
                {
                   if (i <= 100) {
                       printf("Korisnici jos nisu generisani potrebno je jos formatirati %d korisnika\n",(100-i));
                       CloseHandle(gl);
                       generisanjeListeEnded = 1;
                   }

                }break;
            case 'p'://sortirana lista
                {
                    int j;
                    for(j=0;j<i;j++)
                    {
                        printf("%s , %d godiste\n",ljudi[j].ime,ljudi[j].godiste);
                        free(ljudi[j].ime);
                    }
                    flag = 0;
                }break;
            default:
                printf("Greska\n");
                break;
        }
    }

    CloseHandle(gl);
    CloseHandle(st);

    return 0;
}
