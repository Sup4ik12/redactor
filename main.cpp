#include "TXLib.h"

struct btn
{
    int x;
    int y;
    const char* text;
    int vn;
    string category;
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

struct pic
{
    int x;
    int y;
    int w;
    int h;
    int W;
    int H;
    HDC name;
    bool visble;
    string category;

    void draw()
    {
        if(visble)
        {
        Win32::TransparentBlt (txDC(),x,y,w,h,name,0,0,W,H,TX_WHITE);
        }
    }
    bool click()
    {
        return(txMouseX() < x+w and txMouseX() > x and txMouseY() > y and txMouseY() < y+h and txMouseButtons() == 1);
    }
};

void space()
{
    //������� �������
    txRectangle(0,0,1500,800);
    txSetColor(TX_BLACK,5);
    txSetFillColor(TX_TRANSPARENT);
    txRectangle(150,100,1490,790);
}

int main()
{
    txCreateWindow (1500, 800);
    txTextCursor (false);

    int countbtn = 6;
    int countMp = 12;
    int countCp = 0;

    char str[50];
    int vibor = -1;
    int MouseUp = false;

    btn btn[countbtn];
    btn[0] = {100,30,"����",TX_GREY,"����"};
    btn[1] = {350,30,"�����������",TX_GREY,"�����������"};
    btn[2] = {600,30,"������",TX_GREY,"������"};
    btn[3] = {850,30,"�����",TX_GREY,"�����"};
    btn[4] = {1100,30,"�������",TX_GREY,"�������"};
    btn[5] = {1350,30,"�����",TX_GREY,"�����"};

    pic Mp[countMp];
    Mp[0] = {0,100,120,120,700,700,txLoadImage("��������\\����\\����1.bmp"),false,"����"};
    Mp[1] = {0,220,120,120,700,700,txLoadImage("��������\\����\\����2.bmp"),false,"����"};
    Mp[2] = {0,340,120,120,700,700,txLoadImage("��������\\����\\����3.bmp"),false,"����"};

    Mp[3] = {0,100,120,120,500,500,txLoadImage("��������\\�����������\\�����1.bmp"),false,"�����������"};
    Mp[4] = {0,220,120,120,500,500,txLoadImage("��������\\�����������\\�����2.bmp"),false,"�����������"};
    Mp[5] = {0,340,120,120,500,500,txLoadImage("��������\\�����������\\�����3.bmp"),false,"�����������"};

    Mp[6] = {0,100,120,120,200,266,txLoadImage("��������\\�����\\������1.bmp"),false,"�����"};
    Mp[7] = {0,220,120,120,200,266,txLoadImage("��������\\�����\\������2.bmp"),false,"�����"};
    Mp[8] = {0,340,120,120,200,266,txLoadImage("��������\\�����\\������3.bmp"),false,"�����"};

    Mp[9] = {0,100,120,120,150,150,txLoadImage("��������\\������\\�����1.bmp"),false,"������"};
    Mp[10] = {0,220,120,120,150,150,txLoadImage("��������\\������\\�����2.bmp"),false,"������"};
    Mp[11] = {0,340,120,120,150,150,txLoadImage("��������\\������\\�����3.bmp"),false,"������"};

    pic Cp[100];

    while(!GetAsyncKeyState(VK_ESCAPE))
    {
        txSetColor(TX_WHITE);
        txSetFillColor(TX_WHITE);
        txClear();
        txBegin();
        space();
        //��������� ������
        for (int i=0; i<countbtn; i++)
        {
            btn[i].draw();
        }
        //��������� �������� � ����
        for (int i=0; i<countMp; i++)
        {
            Mp[i].draw();
        }
        //������� ����� �������� � ����
        for (int i=0; i<countMp; i++)
        {
            if(Mp[i].click() == true and Mp[i].visble)
            {
                while(txMouseButtons() == 1)
                {
                    txSleep(1);
                }
                Cp[countCp] = {200,200,Mp[i].W,Mp[i].H,Mp[i].W,Mp[i].H,Mp[i].name,Mp[i].visble,Mp[i].category};
                countCp ++;
            }
        }
        //��������� �������� � ������
        for (int i=0; i<countCp; i++)
        {
            Cp[i].draw();
        }
        //����� �������� � ����
        for (int btn_i=0; btn_i<countbtn; btn_i++)
        {
            if(btn[btn_i].click())
            {
                for (int pic_i=0; pic_i<countMp; pic_i++)
                {
                    Mp[pic_i].visble = false;
                    if(Mp[pic_i].category == btn[btn_i].category)
                    {
                        Mp[pic_i].visble = true;
                    }
                }
            }
        }
        //�������� �������� � ���.�������
        for(int i=0; i<countCp; i++)
        {
            if(Cp[i].click())
            {
                vibor = i;
                MouseUp = false;
            }

        }
        //����������� ������ �������� � ������
        if(vibor>=0)
        {
            if(txMouseButtons() == 1 and !MouseUp)
            {
            Cp[vibor].x = txMouseX()-Cp[vibor].w/2;
            Cp[vibor].y = txMouseY()-Cp[vibor].h/2;
            }
        }
        else
        {
            if(txMouseButtons() != 1)
            {
                MouseUp = true;
            }
        }
        //���������� � ���������� ��������
        if(vibor>=0)
        {
            if(GetAsyncKeyState(VK_OEM_PLUS))
            {
                Cp[vibor].w = Cp[vibor].w * 1.01;
                Cp[vibor].h = Cp[vibor].h * 1.01;
            }
            if(GetAsyncKeyState(VK_OEM_MINUS))
            {
                Cp[vibor].w = Cp[vibor].w / 1.01;
                Cp[vibor].h = Cp[vibor].h / 1.01;
            }
        }


        sprintf(str, "%d", countCp);
        txTextOut(10,10,str);

        txEnd();

    }

for (int i=0; i<countMp; i++)
{
    txDeleteDC(Mp[i].name);
}
for (int i=0; i<countCp; i++)
{
    txDeleteDC(Cp[i].name);
}

    txTextCursor (false);
    txDisableAutoPause();
    return 0;
}

