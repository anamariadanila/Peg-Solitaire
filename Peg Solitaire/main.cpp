#include <iostream>
#include <winbgim.h>
#include <stdlib.h>
#include <mmsystem.h>
#include <string.h>
#include <stdio.h>
#include <graphics.h>
using namespace std;

#define MAX 20
#define DIM_ELEMENT

///variabile care definesc culorile

int culoareFundal = BLACK, culoareTabla = GREEN, culoarePiesa = RED;

///variabile care se ocupa de mecanicile jocului

int iPiesa, jPiesa, iTinta, jTinta, iPatratGol, jPatratGol;

///variabile care se ocupa de generarea jocului si alte detalii ale sale

int TablaDeJoc[MAX][MAX], numarPatrateLatura=7, tipTabla=0, tipJoc=0, randJucator=1, contorPieseJucator1=0, contorPieseJucator2=0;

///variabile care se ocupa de partea grafica

int stanga, sus, width, height, latura;

char contor1[20], contor2[20];

///variabile pentru meniuri

bool iesire=false, optiune_stg=false , optiune_help=false, optiune_sp=false, optiune_pvp=false;
bool optiune_help_exit=false, optiune_stg_exit=false, optiune_sp_exit=false, optiune_pvp_exit=false;

///functii

bool esteInTabla(int i,int j)
{
    if((i>2 && i<numarPatrateLatura-1) || (j>2 && j<numarPatrateLatura-1))
        return true;
    if(tipTabla==1)
        if((i==2 && (j==2 || j==numarPatrateLatura-1)) || (i==numarPatrateLatura-1 && (j==2 || j==numarPatrateLatura-1)))
            return true;
    return false;
}

void last_click() /// functie pentru a vedea coordonatele ultimului click dat (apar in consola)
{
    if(ismouseclick(WM_LBUTTONDOWN))
    {
        clearmouseclick(WM_LBUTTONDOWN);
        cout<<mousex()<<" "<<mousey()<<endl;
    }
}

///functii pentru procesele grafice

void desenPiesa(int xPiesa,int yPiesa)
{
    setcolor(culoarePiesa);
    setfillstyle(SOLID_FILL,culoarePiesa);
    fillellipse(xPiesa,yPiesa,latura/3,latura/3);
    setcolor(culoareTabla);
}

void stergerePiesa(int xPiesa, int yPiesa)
{
    setcolor(culoareFundal);
    setfillstyle(SOLID_FILL,culoareFundal);
    fillellipse(xPiesa,yPiesa,latura/3,latura/3);
    setcolor(culoareTabla);
}

void desenPatratJoc(int i,int j)
{
    int xCentruPatrat=((stanga+latura*(i-1))+(stanga+latura*i))/2,yCentruPatrat=((sus+latura*(j-1))+(sus+latura*j))/2;
    rectangle(stanga+latura*(i-1),sus+latura*(j-1),stanga+latura*i,sus+latura*j);
    if(i!=(numarPatrateLatura+1)/2 || j!=(numarPatrateLatura+1)/2)
        desenPiesa(xCentruPatrat,yCentruPatrat);
}

void desenTabla()
{
    int i,j;
    width=400; height=400; latura=width/numarPatrateLatura;
    sus=(getmaxy()-width)/2; stanga=(getmaxx()-height)/2;
    setbkcolor(culoareFundal); clearviewport(); setcolor(culoareTabla);
    for(i=1; i<=numarPatrateLatura; i++)
        for (j=1; j<=numarPatrateLatura; j++)
            if(esteInTabla(i,j))
                desenPatratJoc(i,j);
}

void bulina_de_rand()
{
    if(randJucator==1)
    {
        setcolor(culoareFundal);
        setfillstyle(SOLID_FILL,culoareFundal);
        fillellipse(730,300,20,20);
        setcolor(YELLOW);
        setfillstyle(SOLID_FILL,YELLOW);
        fillellipse(70,300,20,20);
    }
    else
        if(randJucator==2)
        {
            setcolor(culoareFundal);
            setfillstyle(SOLID_FILL,culoareFundal);
            fillellipse(70,300,20,20);
            setcolor(YELLOW);
            setfillstyle(SOLID_FILL,YELLOW);
            fillellipse(730,300,20,20);
        }
}

void actualizare_contor()
{
    setcolor(YELLOW);
    sprintf(contor1, "%d", contorPieseJucator1);
    settextstyle(10,4,4);
    outtextxy(100,200,contor1);
    sprintf(contor2, "%d", contorPieseJucator2);
    outtextxy(700,200,contor2);
}

///functii pentru crearea matricii de joc

void crearePiesa(int i,int j)
{
    TablaDeJoc[i][j]=1;
}

void crearePatratJoc(int i,int j)
{
    if(i!=iPatratGol || j!=jPatratGol)
        crearePiesa(i,j);
}

void crearePatratFinal()
{
        iPatratGol=(numarPatrateLatura+1)/2;
        jPatratGol=(numarPatrateLatura+1)/2;
}

void creareTabla()
{
    int i,j;
    crearePatratFinal();
    for(i=1; i<=numarPatrateLatura; i++)
        for (j=1; j<=numarPatrateLatura; j++)
            TablaDeJoc[i][j]=0;
    for(i=1; i<=numarPatrateLatura; i++)
        for (j=1; j<=numarPatrateLatura; j++)
            if(esteInTabla(i,j))
                crearePatratJoc(i,j);
}

///functii pentru realizarea mutarilor

void alegerePiesa(int &iPiesa, int &jPiesa)
{
    iPiesa=0, jPiesa=0;
    while(iPiesa==0)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            iPiesa=(mousey()-sus)/latura+1;
            jPiesa=(mousex()-stanga)/latura+1;
        }
    }
    if(!esteInTabla(iPiesa,jPiesa) || TablaDeJoc[iPiesa][jPiesa]==0)
    {
        alegerePiesa(iPiesa,jPiesa);
    }
}

int alegereDirectie(int &iTinta, int &jTinta)
{
    int directie;
    iTinta=0;
    while(iTinta==0)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            iTinta=(mousey()-sus)/latura+1;
            jTinta=(mousex()-stanga)/latura+1;
        }
    }
    if(iTinta==iPiesa-2 && jTinta==jPiesa)
    {
        directie=8;
    }
    if(iTinta==iPiesa && jTinta==jPiesa+2)
    {
        directie=6;
    }
    if(iTinta==iPiesa+2 && jTinta==jPiesa)
    {
        directie=2;
    }
    if(iTinta==iPiesa && jTinta==jPiesa-2)
    {
        directie=4;
    }
    if(tipJoc==1)
    {
        if(iTinta==iPiesa+2 && jTinta==jPiesa+2)
        {
            directie=3;
        }
        if(iTinta==iPiesa+2 && jTinta==jPiesa-2)
        {
            directie=1;
        }
        if(iTinta==iPiesa-2 && jTinta==jPiesa+2)
        {
            directie=9;
        }
        if(iTinta==iPiesa-2 && jTinta==jPiesa-2)
        {
            directie=7;
        }
    }
    return directie;
}

void mutarePiesa(int directie)
{
    int xPatrat1=((stanga+latura*(jPiesa-1))+(stanga+latura*jPiesa))/2,yPatrat1=((sus+latura*(iPiesa-1))+(sus+latura*iPiesa))/2;
    int xPatrat2=((stanga+latura*(jTinta-1))+(stanga+latura*jTinta))/2,yPatrat2=((sus+latura*(iTinta-1))+(sus+latura*iTinta))/2;
    if(directie==8)
    {
        TablaDeJoc[iPiesa][jPiesa]=0;
        stergerePiesa(xPatrat1,yPatrat1);

        TablaDeJoc[iPiesa-1][jPiesa]=0;
        stergerePiesa(xPatrat1,yPatrat1-latura);

        TablaDeJoc[iTinta][jTinta]=1;
        desenPiesa(xPatrat2,yPatrat2);
    }
    if(directie==9)
    {
        TablaDeJoc[iPiesa][jPiesa]=0;
        stergerePiesa(xPatrat1,yPatrat1);

        TablaDeJoc[iPiesa-1][jPiesa+1]=0;
        stergerePiesa(xPatrat1+latura,yPatrat1-latura);

        TablaDeJoc[iTinta][jTinta]=1;
        desenPiesa(xPatrat2,yPatrat2);
    }
    if(directie==6)
    {
        TablaDeJoc[iPiesa][jPiesa]=0;
        stergerePiesa(xPatrat1,yPatrat1);

        TablaDeJoc[iPiesa][jPiesa+1]=0;
        stergerePiesa(xPatrat1+latura,yPatrat1);

        TablaDeJoc[iTinta][jTinta]=1;
        desenPiesa(xPatrat2,yPatrat2);
    }
    if(directie==3)
    {
        TablaDeJoc[iPiesa][jPiesa]=0;
        stergerePiesa(xPatrat1,yPatrat1);

        TablaDeJoc[iPiesa+1][jPiesa+1]=0;
        stergerePiesa(xPatrat1+latura,yPatrat1+latura);

        TablaDeJoc[iTinta][jTinta]=1;
        desenPiesa(xPatrat2,yPatrat2);
    }
    if(directie==2)
    {
        TablaDeJoc[iPiesa][jPiesa]=0;
        stergerePiesa(xPatrat1,yPatrat1);

        TablaDeJoc[iPiesa+1][jPiesa]=0;
        stergerePiesa(xPatrat1,yPatrat1+latura);

        TablaDeJoc[iTinta][jTinta]=1;
        desenPiesa(xPatrat2,yPatrat2);
    }
    if(directie==1)
    {
        TablaDeJoc[iPiesa][jPiesa]=0;
        stergerePiesa(xPatrat1,yPatrat1);

        TablaDeJoc[iPiesa+1][jPiesa-1]=0;
        stergerePiesa(xPatrat1-latura,yPatrat1+latura);

        TablaDeJoc[iTinta][jTinta]=1;
        desenPiesa(xPatrat2,yPatrat2);
    }
    if(directie==4)
    {
        TablaDeJoc[iPiesa][jPiesa]=0;
        stergerePiesa(xPatrat1,yPatrat1);

        TablaDeJoc[iPiesa][jPiesa-1]=0;
        stergerePiesa(xPatrat1-latura,yPatrat1);

        TablaDeJoc[iTinta][jTinta]=1;
        desenPiesa(xPatrat2,yPatrat2);
    }
    if(directie==7)
    {
        TablaDeJoc[iPiesa][jPiesa]=0;
        stergerePiesa(xPatrat1,yPatrat1);

        TablaDeJoc[iPiesa-1][jPiesa-1]=0;
        stergerePiesa(xPatrat1-latura,yPatrat1-latura);

        TablaDeJoc[iTinta][jTinta]=1;
        desenPiesa(xPatrat2,yPatrat2);
    }
    system("CLS");
}

bool esteMutareValida(int directie)
{
    if(!esteInTabla(iTinta,jTinta))
        return 0;
    if(TablaDeJoc[iTinta][jTinta]!=0)
        return 0;
    if(directie==8)
    {
        if(TablaDeJoc[iPiesa-1][jPiesa]!=1 || iPiesa==2)
            return 0;
        else
            return 1;
    }
    if(directie==9)
    {
        if(TablaDeJoc[iPiesa-1][jPiesa+1]!=1 || iPiesa==2 || jPiesa==numarPatrateLatura-1)
            return 0;
        else
            return 1;
    }
    if(directie==6)
    {
        if(TablaDeJoc[iPiesa][jPiesa+1]!=1 || jPiesa==numarPatrateLatura-1)
            return 0;
        else
            return 1;
    }
    if(directie==3)
    {
        if(TablaDeJoc[iPiesa+1][jPiesa+1]!=1 || iPiesa==numarPatrateLatura-1 || jPiesa==numarPatrateLatura-1)
            return 0;
        else
            return 1;
    }
    if(directie==2)
    {
        if(TablaDeJoc[iPiesa+1][jPiesa]!=1 || iPiesa==numarPatrateLatura-1)
            return 0;
        else
            return 1;
    }
    if(directie==1)
    {
        if(TablaDeJoc[iPiesa+1][jPiesa-1]!=1 || iPiesa==numarPatrateLatura-1 || jPiesa==2)
            return 0;
        else
            return 1;
    }
    if(directie==4)
    {
        if(TablaDeJoc[iPiesa][jPiesa-1]!=1 || jPiesa==2)
            return 0;
        else
            return 1;
    }
    if(directie==7)
    {
        if(TablaDeJoc[iPiesa-1][jPiesa-1]!=1 || iPiesa==2 || jPiesa==2)
            return 0;
        else
            return 1;
    }
    return 0;
}

bool SePoateMutaPiesa(int i,int j);

void mutareBonus(int i,int j)
{
    system("CLS");
    int directie = alegereDirectie(iTinta,jTinta);
            if(esteMutareValida(directie))
            {
                mutarePiesa(directie);
                if(randJucator==1)
                {
                    contorPieseJucator1++;
                    actualizare_contor();
                }
                else
                    if(randJucator==2)
                    {
                        contorPieseJucator2++;
                        actualizare_contor();
                    }
                if(SePoateMutaPiesa(iTinta,jTinta))
                {
                    iPiesa=iTinta;
                    jPiesa=jTinta;
                    mutareBonus(iPiesa,jPiesa);
                }
            }
}

void alegereMutareSolo()
{
    iPiesa=0; jPiesa=0; iTinta=0; jTinta=0;
    alegerePiesa(iPiesa,jPiesa);
    if(iPiesa!=0 && jPiesa!=0)
        {
            int directie = alegereDirectie(iTinta,jTinta);
            if(esteMutareValida(directie))
            {
                mutarePiesa(directie);
            }
            else
            {
                alegereMutareSolo();
            }
        }
}

void alegereMutarePVP()
{
    bulina_de_rand();
    iPiesa=0; jPiesa=0; iTinta=0; jTinta=0;
    alegerePiesa(iPiesa,jPiesa);
    if(iPiesa!=0 && jPiesa!=0)
        {
            int directie = alegereDirectie(iTinta,jTinta);
            if(esteMutareValida(directie))
            {
                mutarePiesa(directie);
                if(randJucator==1)
                {
                    contorPieseJucator1++;
                    actualizare_contor();
                }
                else
                    if(randJucator==2)
                    {
                        contorPieseJucator2++;
                        actualizare_contor();
                    }
                if(SePoateMutaPiesa(iTinta,jTinta))
                {
                    iPiesa=iTinta;
                    jPiesa=jTinta;
                    mutareBonus(iPiesa,jPiesa);
                }
                if(randJucator==1 && !SePoateMutaPiesa(iTinta,jTinta))
                {
                    randJucator=2;
                }
                else
                    if(randJucator==2 && !SePoateMutaPiesa(iTinta,jTinta))
                    {
                        randJucator=1;
                    }
            }
            else
            {
                alegereMutarePVP();
            }
        }
}

bool SePotFaceMutari()
{
    int test=0;
    for(int i=1;i<=numarPatrateLatura;i++)
        for(int j=1;j<=numarPatrateLatura;j++)
        {
            if(TablaDeJoc[i][j]==1)
            {
                if(TablaDeJoc[i-1][j]==1 && esteInTabla(i-2,j) && TablaDeJoc[i-2][j]==0 && i!=2)
                {
                    test=1;
                }
                if(TablaDeJoc[i][j+1]==1 && esteInTabla(i,j+2) && TablaDeJoc[i][j+2]==0 && j!=numarPatrateLatura-1)
                {
                    test=1;
                }
                if(TablaDeJoc[i+1][j]==1 && esteInTabla(i+2,j) && TablaDeJoc[i+2][j]==0 && i!=numarPatrateLatura-1)
                {
                    test=1;
                }
                if(TablaDeJoc[i][j-1]==1 && esteInTabla(i,j-2) && TablaDeJoc[i][j-2]==0 && j!=2)
                {
                    test=1;
                }
                if(tipJoc==1)
                {
                    if(TablaDeJoc[i-1][j+1]==1 && esteInTabla(i-2,j+2) && TablaDeJoc[i-2][j+2]==0 && i!=2 && j!=numarPatrateLatura-1)
                    {
                        test=1;
                    }
                    if(TablaDeJoc[i+1][j+1]==1 && esteInTabla(i+2,j+2) && TablaDeJoc[i+2][j+2]==0 && j!=numarPatrateLatura-1 && i!=numarPatrateLatura-1)
                    {
                        test=1;
                    }
                    if(TablaDeJoc[i+1][j-1]==1 && esteInTabla(i+2,j-2) && TablaDeJoc[i+2][j-2]==0 && i!=numarPatrateLatura-1 && j!=2)
                    {
                        test=1;
                    }
                    if(TablaDeJoc[i-1][j-1]==1 && esteInTabla(i-2,j-2) && TablaDeJoc[i-2][j-2]==0 && i!=2 && j!=2)
                    {
                        test=1;
                    }
                }
            }
        }
    return test;
}

bool SePoateMutaPiesa(int i,int j)
{
    int test=0;
    if(TablaDeJoc[i][j]==1)
    {
        if(TablaDeJoc[i-1][j]==1 && esteInTabla(i-2,j) && TablaDeJoc[i-2][j]==0 && i!=2)
        {
            test=1;
        }
        if(TablaDeJoc[i-1][j+1]==1 && esteInTabla(i-2,j+2) && TablaDeJoc[i-2][j+2]==0 && i!=2 && j!=numarPatrateLatura-1)
        {
            test=1;
        }
        if(TablaDeJoc[i][j+1]==1 && esteInTabla(i,j+2) && TablaDeJoc[i][j+2]==0 && j!=numarPatrateLatura-1)
        {
            test=1;
        }
        if(TablaDeJoc[i+1][j+1]==1 && esteInTabla(i+2,j+2) && TablaDeJoc[i+2][j+2]==0 && j!=numarPatrateLatura-1 && i!=numarPatrateLatura-1)
        {
            test=1;
        }
        if(TablaDeJoc[i+1][j]==1 && esteInTabla(i+2,j) && TablaDeJoc[i+2][j]==0 && i!=numarPatrateLatura-1)
        {
            test=1;
        }
        if(TablaDeJoc[i+1][j-1]==1 && esteInTabla(i+2,j-2) && TablaDeJoc[i+2][j-2]==0 && i!=numarPatrateLatura-1 && j!=2)
        {
            test=1;
        }
        if(TablaDeJoc[i][j-1]==1 && esteInTabla(i,j-2) && TablaDeJoc[i][j-2]==0 && j!=2)
        {
            test=1;
        }
        if(TablaDeJoc[i-1][j-1]==1 && esteInTabla(i-2,j-2) && TablaDeJoc[i-2][j-2]==0 && i!=2 && j!=2)
        {
            test=1;
        }
    }
    return test;
}

///functii pentru conditiile de victorie

bool VictorieSolo()
{
    if(TablaDeJoc[iPatratGol][jPatratGol]==0)
        return 0;
    for(int i=1;i<=numarPatrateLatura;i++)
        for(int j=1;j<=numarPatrateLatura;j++)
            if(TablaDeJoc[i][j]==1 && (i!=iPatratGol || j!=jPatratGol))
                return 0;
    return 1;
}

int VictoriePVP()
{
    if(contorPieseJucator1>contorPieseJucator2)
        return 1;
    else
    {
        if(contorPieseJucator1<contorPieseJucator2)
            return 2;
    }
    return 0;
}

///functii pentru tipul de joc

void jocSolo()
{
    tipJoc=0;
    creareTabla();
    desenTabla();
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    setcolor(YELLOW);
    settextstyle(10,4,5);
    outtextxy(400,50,"UN JUCATOR");
    PlaySound(TEXT("muzica/MuzicaJocSolo.wav"), NULL, SND_FILENAME|SND_LOOP|SND_ASYNC);
    while(SePotFaceMutari())
    {
        alegereMutareSolo();
    }
    setcolor(YELLOW);
    settextstyle(10,4,4);
    outtextxy(400,540,"Joc terminat");
    if(VictorieSolo())
    {
        settextstyle(10,4,4);
        outtextxy(400,575,"Ai castigat!");
        PlaySound(TEXT("muzica/MuzicaVictorie.wav"), NULL, SND_FILENAME|SND_ASYNC);
        system("pause");
    }
    else
    {
        settextstyle(10,4,4);
        outtextxy(400,575,"Ai pierdut!");
        PlaySound(TEXT("muzica/MuzicaPierdereSolo.wav"), NULL, SND_FILENAME|SND_ASYNC);
        system("pause");
    }

}

void jocPVP()
{
    tipJoc=1;
    creareTabla();
    desenTabla();
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    setcolor(YELLOW);
    settextstyle(10,4,5);
    outtextxy(400,50,"JOC INTRE DOI JUCATORI");
    settextstyle(10,4,3);
    outtextxy(120,120,"Jucatorul 1:");
    outtextxy(680,120,"Jucatorul 2:");
    actualizare_contor();
    PlaySound(TEXT("muzica/MuzicaJocPVP.wav"), NULL, SND_FILENAME|SND_LOOP|SND_ASYNC);
    while(SePotFaceMutari())
    {
        alegereMutarePVP();
    }
    setcolor(YELLOW);
    settextstyle(10,4,4);
    outtextxy(400,540,"Joc terminat");
    int victorie=VictoriePVP();
    if(victorie==1)
    {
        settextstyle(10,4,4);
        outtextxy(400,575,"Jucatorul 1 castiga!");
        PlaySound(TEXT("muzica/MuzicaVictorie.wav"), NULL, SND_FILENAME|SND_ASYNC);
        system("pause");
    }
    else
    {
        if(victorie==2)
        {
            settextstyle(10,4,4);
            outtextxy(400,575,"Jucatorul 2 castiga!");
            PlaySound(TEXT("muzica/MuzicaVictorie2.wav"), NULL, SND_FILENAME|SND_ASYNC);
            system("pause");
        }
        else
        {
            settextstyle(10,4,4);
            outtextxy(400,575,"Remiza!");
            PlaySound(TEXT("muzica/MuzicaRemiza.wav"), NULL, SND_FILENAME|SND_ASYNC);
            system("pause");
        }
    }
}

///functii pentru meniuri

void meniu_pagina()
{

   setactivepage(0);
   setcolor(WHITE);
   setbkcolor(culoareFundal);
   settextjustify(CENTER_TEXT,CENTER_TEXT);
   settextstyle(3,4,7);
   outtextxy(350,80, "SOLITARUL");
   settextstyle(3,4,3);
   outtextxy(350, 140, "CUM SE JOACA");
   outtextxy(350, 190, "SETARI");
   outtextxy(350, 240, "UN JUCATOR");
   outtextxy(350, 290, "DOI JUCATORI");
   last_click();
   outtextxy(50,500, "IESIRE");
   setvisualpage(0);
}



void ch_meniu(int x, int y)
{
    int visual=getvisualpage();
    if(x >= 10 && x <= 90 && y >= 485 && y<= 500 && visual==0)
        iesire=true;

    if(x >= 265 && x <= 430 && y >= 125 && y<= 140 && visual==0)
        optiune_help=true;

    if(x >= 50 && x <= 115 && y >= 480 && y<= 500 && visual==1)
        optiune_help_exit=true;

    if(x >= 315 && x <= 390 && y >= 170 && y <= 190 && visual==0)
        optiune_stg=true;

    if(x >= 50 && x <= 115 && y >= 480 && y<= 500 && visual==2)
        optiune_stg_exit=true;

    if (x >= 260 && x <= 445 && y >= 225 && y <= 238 && visual==0)
        optiune_sp=true;

    if(x >= 10 && x <= 95 && y >= 530 && y <= 555 && visual==3)
        optiune_sp_exit=true;

    if(x >= 225 && x <= 475 && y >= 275 && y <= 295 && visual==0)
        optiune_pvp=true;

    if(x >= 10 && x <= 95 && y >= 530 && y <= 555 && visual==3)
        optiune_pvp_exit=true;

///optiuni pentru meniul de setari

    if(x >= 170 && x <= 330 && y >= 175 && y<= 190 && visual==2)
        {
            tipTabla=0;
        }
    if(x >= 360 && x <= 540 && y >= 175 && y<= 190 && visual==2)
        {
            tipTabla=1;
        }
    if(x >= 150 && x <= 200 && y >= 275 && y <= 295 && visual==2)
        culoareFundal = WHITE;

    if(x >= 310 && x <= 390 && y >= 275 && y <= 295 && visual==2)
        culoareFundal = BLACK;

    if(x >= 415 && x <= 635 && y >= 275 && y <= 295 && visual==2)
        culoareFundal = BLUE;

    if (x >= 130 && x <= 225 && y >= 375 && y <= 395 && visual==2)
        culoarePiesa = YELLOW;

    if(x >= 320 && x <= 380 && y >= 375 && y <= 395 && visual==2)
        culoarePiesa = RED;

    if(x >= 485 && x <= 565 && y >= 375 && y <= 395 && visual==2)
        culoarePiesa = GREEN;

    if (x >= 115 && x <= 235 && y >= 475 && y <= 495 && visual==2)
        culoareTabla = LIGHTBLUE;

    if(x >= 320 && x <= 380 && y >= 475 && y <= 495 && visual==2)
        culoareTabla = RED;

    if(x >= 485 && x <= 565 && y >= 475 && y <= 495 && visual==2)
        culoareTabla = GREEN;

    if(x >= 390 && x <= 410 && y >= 520 && y <= 550 && visual==2)
        numarPatrateLatura = 7;

    if(x >= 425 && x <= 450 && y >= 520 && y <= 550 && visual==2)
        numarPatrateLatura = 9;

    if(x >= 465 && x <= 495 && y >= 520 && y <= 550 && visual==2)
        numarPatrateLatura = 11;

    if(x >= 505 && x <= 535 && y >= 520 && y <= 550 && visual==2)
        numarPatrateLatura = 13;

    if(x >= 545 && x <= 580 && y >= 520 && y <= 550 && visual==2)
        numarPatrateLatura = 15;

}

void help()
{
    optiune_help_exit=false;
    setactivepage(1);
    setcolor(WHITE);
    setbkcolor(culoareFundal);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    settextstyle(10,4,3);
    outtextxy(350,50,"REGULI SOLITARUL");
    settextstyle(COMPLEX_FONT,0,1);
    settextjustify(LEFT_TEXT,CENTER_TEXT);
    outtextxy(10,100,"La inceputul jocului toate campurile, in afara unuia singur, sunt  " );
    outtextxy(10,130,"ocupate cu piese. Fiecare pion poate sari peste un altul, orizontal ");
    outtextxy(10,160,"sau vertical, dar nu se poate deplasa pe diagonala. Trebuie ca in");
    outtextxy(10,190,"spatele pionului sa existe o casuta libera." );
    outtextxy(10,220,"IMPORTANT! Pionul peste care se sare este eliminat. Problema standard");
    outtextxy(10,250,"pleaca de la casuta goala din centrul tablei si presupune ca, prin");
    outtextxy(10,280,"mutari adecvate, sa fie eliminati toti pionii, mai putin unul care");
    outtextxy(10,310,"va ajunge in final exact in centrul tablei, acolo unde initial era");
    outtextxy(10,340,"casuta libera. Distractie placuta!");
    settextstyle(3,4,3);
    outtextxy(50,500,"IESIRE");
    readimagefile("imagini/solitarul2.jpg", 450, 325, 800, 600);
    setvisualpage(1);
    do
    {
        last_click();
    } while(optiune_help_exit==false);
    setactivepage(0);
    setvisualpage(0);
}

void settings()
{
    optiune_stg=false;
    optiune_stg_exit=false;
    setactivepage(2);
    setcolor(WHITE);
    setbkcolor(culoareFundal);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    settextstyle(3,4,5);
    outtextxy(350,50,"SETARI");
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    settextstyle(3,4,3);
    outtextxy(350, 140, "TIP TABLA:");
    setcolor(LIGHTRED);
    outtextxy(250, 190, "ENGLEZEASCA");
    setcolor(GREEN);
    outtextxy(450, 190, "CONTINENTALA");
    setcolor(WHITE);
    outtextxy(350, 240, "CULOARE FUNDAL:");
    setcolor(LIGHTGRAY);
    outtextxy(175, 290, "ALB");
    setcolor(DARKGRAY);
    outtextxy(350, 290, "NEGRU");
    setcolor(LIGHTBLUE);
    outtextxy(525, 290, "ALBASTRU INCHIS");
    setcolor(WHITE);
    outtextxy(350, 340, "CULOARE PIESE:");
    setcolor(YELLOW);
    outtextxy(175, 390, "GALBEN");
    setcolor(LIGHTRED);
    outtextxy(350, 390, "ROSU");
    setcolor(GREEN);
    outtextxy(525, 390, "VERDE");
    setcolor(WHITE);
    outtextxy(350, 440, "CULOARE TABLA:");
    setcolor(LIGHTBLUE);
    outtextxy(175, 490, "ALBASTRU");
    setcolor(LIGHTRED);
    outtextxy(350, 490, "ROSU");
    setcolor(GREEN);
    outtextxy(525, 490, "VERDE");
    setcolor(WHITE);
    outtextxy(200, 540, "NUMAR PATRATE PE LATURA:");
    outtextxy(400, 540, "7");
    outtextxy(440, 540, "9");
    outtextxy(480, 540, "11");
    outtextxy(520, 540, "13");
    outtextxy(560, 540, "15");
    setvisualpage(2);
    settextstyle(COMPLEX_FONT,0,1);
    settextjustify(LEFT_TEXT,CENTER_TEXT);
    setcolor(WHITE);
    settextstyle(3,4,3);
    outtextxy(50,500,"IESIRE");
    do
    {
        last_click();
    } while(optiune_stg_exit==false);

    setactivepage(0);
    setvisualpage(0);
}

int main()
{
    initwindow(800,600);
    meniu_pagina();
    PlaySound(TEXT("muzica/MuzicaMeniu.wav"), NULL, SND_FILENAME|SND_LOOP|SND_ASYNC);
    int gdriver = IBM8514, gmode=IBM8514LO, errorcode;
    errorcode = graphresult();
    if (errorcode != grOk)
    {
      printf("Graphics error: %s\n", grapherrormsg(errorcode));
      printf("Press any key to halt:");
      getch();
      exit(1);
    }
    int x=getmaxx(), ht=textheight("W");
    do
   {
       iesire=false;
       registermousehandler(WM_LBUTTONDOWN, ch_meniu);
       if(optiune_help==true)
       {
           help();
           optiune_help=false;
       }
     if(optiune_stg==true)
     {
        settings();
        optiune_stg=false;
     }
    if(optiune_sp==true)
     {
        jocSolo();
        optiune_sp=false;
     }
     if(optiune_pvp==true)
     {
        jocPVP();
        optiune_pvp=false;
     }
       last_click();
   }
   while(iesire==false);
   closegraph();
   return 0;

}

