#include "TXLib.h"

struct btn
{
    int x;
    int y;
    const char* text;
    int vn;
    void draw()
    {
        txSetColor(TX_BLACK);
        txSetFillColor(TX_BLACK);
        Win32::RoundRect (txDC(), x+3,y+5, x+150+3, y+50+5, 52, 52);
        txSetColor(TX_BLACK,5);
        txSetFillColor(vn);
        Win32::RoundRect (txDC(), x,y, x+150, y+50, 52, 52);
        txSelectFont("Times New Roman",20);
        txDrawText(x,y,x+150,y+50, text);
    }
    bool click()
    {
        return(txMouseX() < x+150 and txMouseX() > x and txMouseY() > y and txMouseY() < y+50 and txMouseButtons() == 1);
    }
};



int main()
{
    txCreateWindow (1500, 800);
    txTextCursor (false);

    btn b1 = {100,30,"ÊÍÎÏÊÀ",TX_GREY};
    btn b2 = {350,30,"ÊÍÎÏÊÀ",TX_GREY};


    while(!GetAsyncKeyState(VK_ESCAPE))
    {
        txSetColor(TX_WHITE);
        txSetFillColor(TX_WHITE);
        txClear();
        txBegin();

        //ðàáî÷àÿ îáëàñòü
        txRectangle(0,0,1500,800);
        txSetColor(TX_BLACK,5);
        txSetFillColor(TX_TRANSPARENT);
        txRectangle(100,100,1490,790);
        //êíîïêà
        b1.draw();
        b2.draw();

        if(b1.click())
        {
            txCircle(750,400,250);
        }
        if(b2.click())
        {
            txRectangle(750,400,250,250);
        }




        txEnd();

    }
    txSleep(10);
    txTextCursor (false);
    txDisableAutoPause();
    return 0;
}

