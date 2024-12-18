#include "TXLib.h"
#include <iostream>
#include <fstream>
#include "dirent.h"
using namespace std;

struct btn
{
    int x;
    int y;
    const char* text;
    int textC;
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
        txSelectFont("Time New Roman",20);
        txSetColor(textC);
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
    string adress;
    HDC name;
    int w;
    int h;
    int W;
    int H;
    string category;
    bool visble;
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
void space(HDC image)
{
    //рабочая область
    txSetColor(TX_BLACK,5);
    txSetFillColor(TX_TRANSPARENT);
    txRectangle(150,100,1490,790);
    txBitBlt(txDC(), 150,100,1340,690,image);

}
string dialog(bool sSave)
{
    string FileName = "";

    OPENFILENAME ofn;
    TCHAR szFile[260];
    HANDLE hf;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Text Files\0*.txt\0Any File\0*.\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;



    if(sSave)
    {
        if (GetSaveFileName(&ofn)==TRUE)
        {
            FileName = ofn.lpstrFile;
            FileName = FileName + ".txt";
        }
    }
    else
    {
        if (GetOpenFileName(&ofn)==TRUE)
        {
            FileName = ofn.lpstrFile;
        }
    }

    return FileName;
}
int wight(string adress)
{
    char header[54];
    ifstream bmp;
    bmp.open(adress, ios::in | ios::binary);
    bmp.read(header, 54);
    int W = *(int *)&header[18];
    return W;
}
int height(string adress)
{
    char header[54];
    ifstream bmp;
    bmp.open(adress, ios::in | ios::binary);
    bmp.read(header, 54);
    int H = *(int *)&header[22];
    return H;
}
int readDir(string DirAdress,int countFile,pic Mp[])
{
    DIR *directory = opendir(DirAdress.c_str());
    struct dirent *direntStruct;
    int Y = 100;

    if (directory != NULL)
    {
        while (direntStruct = readdir(directory))
        {
            if((string)direntStruct->d_name != "." and (string)direntStruct->d_name != "..")
            {
            Mp[countFile].adress = DirAdress + direntStruct->d_name;
            Mp[countFile].x = 0;
            Mp[countFile].y = Y;
            countFile ++;
            Y+=120;
            }

        }
    }
    return countFile;
}
inline int GetFilePointer(HANDLE FileHandle)
{
    return SetFilePointer(FileHandle, 0, 0, FILE_CURRENT);
}

bool SaveBMPFile(char *filename, HBITMAP bitmap, HDC bitmapDC, int width, int height)
{
    bool Success=0;
    HBITMAP OffscrBmp=NULL;
    HDC OffscrDC=NULL;
    LPBITMAPINFO lpbi=NULL;
    LPVOID lpvBits=NULL;
    HANDLE BmpFile=INVALID_HANDLE_VALUE;
    BITMAPFILEHEADER bmfh;
    if ((OffscrBmp = CreateCompatibleBitmap(bitmapDC, width, height)) == NULL)
        return 0;
    if ((OffscrDC = CreateCompatibleDC(bitmapDC)) == NULL)
        return 0;
    HBITMAP OldBmp = (HBITMAP)SelectObject(OffscrDC, OffscrBmp);
    BitBlt(OffscrDC, 0, 0, width, height, bitmapDC, 0, 0, SRCCOPY);
    if ((lpbi = (LPBITMAPINFO)(new char[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)])) == NULL)
        return 0;
    ZeroMemory(&lpbi->bmiHeader, sizeof(BITMAPINFOHEADER));
    lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    SelectObject(OffscrDC, OldBmp);
    if (!GetDIBits(OffscrDC, OffscrBmp, 0, height, NULL, lpbi, DIB_RGB_COLORS))
        return 0;
    if ((lpvBits = new char[lpbi->bmiHeader.biSizeImage]) == NULL)
        return 0;
    if (!GetDIBits(OffscrDC, OffscrBmp, 0, height, lpvBits, lpbi, DIB_RGB_COLORS))
        return 0;
    if ((BmpFile = CreateFile(filename,
                        GENERIC_WRITE,
                        0, NULL,
                        CREATE_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL)) == INVALID_HANDLE_VALUE)
        return 0;
    DWORD Written;
    bmfh.bfType = 19778;
    bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
    if (!WriteFile(BmpFile, &bmfh, sizeof(bmfh), &Written, NULL))
        return 0;
    if (Written < sizeof(bmfh))
        return 0;
    if (!WriteFile(BmpFile, &lpbi->bmiHeader, sizeof(BITMAPINFOHEADER), &Written, NULL))
        return 0;
    if (Written < sizeof(BITMAPINFOHEADER))
        return 0;
    int PalEntries;
    if (lpbi->bmiHeader.biCompression == BI_BITFIELDS)
        PalEntries = 3;
    else PalEntries = (lpbi->bmiHeader.biBitCount <= 8) ?
                      (int)(1 << lpbi->bmiHeader.biBitCount) : 0;
    if(lpbi->bmiHeader.biClrUsed)
    PalEntries = lpbi->bmiHeader.biClrUsed;
    if(PalEntries){
    if (!WriteFile(BmpFile, &lpbi->bmiColors, PalEntries * sizeof(RGBQUAD), &Written, NULL))
        return 0;
        if (Written < PalEntries * sizeof(RGBQUAD))
            return 0;
    }
    bmfh.bfOffBits = GetFilePointer(BmpFile);
    if (!WriteFile(BmpFile, lpvBits, lpbi->bmiHeader.biSizeImage, &Written, NULL))
        return 0;
    if (Written < lpbi->bmiHeader.biSizeImage)
        return 0;
    bmfh.bfSize = GetFilePointer(BmpFile);
    SetFilePointer(BmpFile, 0, 0, FILE_BEGIN);
    if (!WriteFile(BmpFile, &bmfh, sizeof(bmfh), &Written, NULL))
        return 0;
    if (Written < sizeof(bmfh))
        return 0;

    CloseHandle (BmpFile);

    delete [] (char*)lpvBits;
    delete [] lpbi;

    DeleteDC (OffscrDC);
    DeleteObject (OffscrBmp);


    return 1;
}

bool ScreenCapture(int x, int y, int width, int height, char *filename, HWND hwnd)
{
    HDC hDC = GetDC(hwnd);
    HDC hDc = CreateCompatibleDC(hDC);

    HBITMAP hBmp = CreateCompatibleBitmap(hDC, width, height);

    HGDIOBJ old= SelectObject(hDc, hBmp);
    BitBlt(hDc, 0, 0, width, height, hDC, x, y, SRCCOPY);

    bool ret = SaveBMPFile(filename, hBmp, hDc, width, height);

    SelectObject(hDc, old);

    DeleteObject(hBmp);

    DeleteDC (hDc);
    ReleaseDC (hwnd, hDC);

    return ret;
}

int main()
{
    txCreateWindow (1500, 800);
    txTextCursor (false);

    int countbtn = 11;
    int countMp = 0;
    int countCp = 0;

    char str[50];
    int vibor = -1;
    int MouseUp = false;
    string page = "kartina";

    btn btn[countbtn];
    btn[0] = {15,30,"ВАЗЫ",TX_BLACK,TX_GREY,"вазы"};
    btn[1] = {195,30,"ФРУКТОВНИЦЫ",TX_BLACK,TX_GREY,"фруктовницы"};
    btn[2] = {395,30,"ФРУКТЫ",TX_BLACK,TX_GREY,"фрукты"};
    btn[3] = {595,30,"ЯГОДЫ",TX_BLACK,TX_GREY,"ягоды"};
    btn[4] = {795,30,"ЦВЕТЫ",TX_BLACK,TX_GREY,"цветы"};
    btn[5] = {1155,30,"СОХРАНИТЬ",TX_WHITE,TX_GREEN,""};
    btn[6] = {1335,30,"ЗАГРУЗИТЬ",TX_WHITE,TX_GREEN,""};
    btn[7] = {10,650,"О ПРОГРАММЕ",TX_WHITE,TX_GREEN,""};
    btn[8] = {10,720,"ВЫХОД",TX_WHITE,TX_GREEN,""};
    btn[9] = {995,30,"ГРИБЫ",TX_BLACK,TX_GREY,"грибы"};
    btn[10] = {10,580,"СНИМОК ЭКРАНА",TX_WHITE,TX_GREEN,""};

    pic Mp[100];
    countMp = readDir("картинки/вазы/",countMp,Mp);
    countMp = readDir("картинки/фруктовницы/",countMp,Mp);
    countMp = readDir("картинки/цветы/",countMp,Mp);
    countMp = readDir("картинки/фрукты/",countMp,Mp);
    countMp = readDir("картинки/ягоды/",countMp,Mp);
    countMp = readDir("картинки/грибы/",countMp,Mp);



    for (int i=0; i<countMp; i++)
    {
        Mp[i].visble=false;
        int pos1 = Mp[i].adress.find("/");
        int pos2 = Mp[i].adress.find("/",pos1+1);
        Mp[i].category = Mp[i].adress.substr(pos1+1,pos2-(pos1+1));
        Mp[i].W = wight(Mp[i].adress);
        Mp[i].H = height(Mp[i].adress);
        if(Mp[i].category == "вазы" )
        {
            Mp[i].w = Mp[i].W/6;
            Mp[i].h = Mp[i].H/6;
        }
        else
        {
            Mp[i].w = 120;
            Mp[i].h = 120;
        }
        Mp[i].name = txLoadImage(Mp[i].adress.c_str());
    }


    pic Cp[100];
    string AdrFon = "картинки/фоны/фон1.bmp";
    HDC fon = txLoadImage(AdrFon.c_str());

    while(!btn[8].click())
    {
        txSetColor(TX_WHITE);
        txSetFillColor(TX_WHITE);
        txClear();
        txBegin();
        if(page == "kartina")
        {
            space(fon);
            //рисование кнопка
            if(GetAsyncKeyState ('1'))
            {
                AdrFon = "картинки/фоны/фон1.bmp";
                fon = txLoadImage(AdrFon.c_str());
            }
            if(GetAsyncKeyState ('2'))
            {
                AdrFon = "картинки/фоны/фон2.bmp";
                fon = txLoadImage(AdrFon.c_str());
            }
            if(GetAsyncKeyState ('3'))
            {
                AdrFon = "картинки/фоны/фон3.bmp";
                fon = txLoadImage(AdrFon.c_str());
            }
            for (int i=0; i<countbtn; i++)
            {
                btn[i].draw();
            }
            //рисование картинок в меню
            for (int i=0; i<countMp; i++)
            {
                Mp[i].draw();
            }
            //условие клика картинок в меню
            for (int i=0; i<countMp; i++)
            {
                if(Mp[i].click() == true and Mp[i].visble)
                {
                    while(txMouseButtons() == 1)
                    {
                        txSleep(1);
                    }
                    Cp[countCp] = {200,200,Mp[i].adress,Mp[i].name,Mp[i].W,Mp[i].H,Mp[i].W,Mp[i].H,Mp[i].category,Mp[i].visble};
                    countCp ++;
                }
            }
            //рисование картинок в центре
            for (int i=0; i<countCp; i++)
            {
                Cp[i].draw();
            }
            //смена картинок в меню
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
            //движение картинок в раб.области
            for(int i=0; i<countCp; i++)
            {
                if(Cp[i].click())
                {
                    vibor = i;
                    MouseUp = false;
                }
            }
            //перемещение мышкой обьектов в центре
            if(vibor>=0)
            {
                if(txMouseButtons() == 1 and !MouseUp)
                {
                Cp[vibor].x = txMouseX()-Cp[vibor].w/2;
                Cp[vibor].y = txMouseY()-Cp[vibor].h/2;
                }
                else
                {
                    if(txMouseButtons() != 1)
                    {
                        MouseUp = true;
                    }
                }
            }
            if(vibor>=0)
            {
                if(GetAsyncKeyState ('W'))
                {
                    Cp[vibor].y -= 5;
                }
                if(GetAsyncKeyState ('A'))
                {
                    Cp[vibor].x -= 5;
                }
                if(GetAsyncKeyState ('S'))
                {
                    Cp[vibor].y += 5;
                }
                if(GetAsyncKeyState ('D'))
                {
                    Cp[vibor].x += 5;
                }
            }
            //увеличение и уменьшение размеров
            if(vibor>=0)
            {
                if(GetAsyncKeyState(VK_OEM_PLUS))
                {
                    Cp[vibor].w = Cp[vibor].w * 1.03;
                    Cp[vibor].h = Cp[vibor].h * 1.03;
                }
                if(GetAsyncKeyState(VK_OEM_MINUS))
                {
                    Cp[vibor].w = Cp[vibor].w * 0.97;
                    Cp[vibor].h = Cp[vibor].h * 0.97;
                    if(Cp[vibor].w < 50)
                    {
                        Cp[vibor].w = 51;
                    }
                    if(Cp[vibor].h < 50)
                    {
                        Cp[vibor].h = 51;
                    }
                    if(Cp[vibor].w > 500)
                    {
                        Cp[vibor].w = 490;
                    }
                    if(Cp[vibor].h > 500)
                    {
                        Cp[vibor].h = 490;
                    }
                }
            }
            if(vibor>=0 and GetAsyncKeyState(VK_DELETE))
            {
                Cp[vibor] = Cp[countCp-1];
                countCp--;
                vibor = -1;
                MouseUp = true;
            }
            if(btn[5].click())
            {
                string FileName = dialog(true);

                std::ofstream out;
                out.open(FileName);
                if (out.is_open())
                {
                    out << AdrFon << std::endl;
                    for(int i=0; i<countCp; i++)
                    {
                        out << Cp[i].x << std::endl;
                        out << Cp[i].y << std::endl;
                        out << Cp[i].adress << std::endl;
                        out << Cp[i].w << std::endl;
                        out << Cp[i].h << std::endl;
                    }
                }
                out.close();
            }
            if(btn[6].click())
            {
                string FileName = dialog(false);

                while(txMouseButtons() == 1)
                {
                    txSleep(1);
                }

                char data[50];

                std::ifstream in(FileName); // окрываем файл для чтения
                if(in.good())
                {
                    in.getline(data, 50);
                    AdrFon = data;
                    fon = txLoadImage(AdrFon.c_str());
                }
                while (in.good())
                {


                    in.getline(data, 50);
                    int x = atoi(data);

                    in.getline(data, 50);
                    int y = atoi(data);

                    in.getline(data, 50);
                    string adress = data;

                    in.getline(data, 50);
                    int w = atoi(data);

                    in.getline(data, 50);
                    int h = atoi(data);


                    for (int i=0; i<countMp; i++)
                    {
                        if(Mp[i].adress == adress)
                        {
                            Cp[countCp] = {x,y,adress,Mp[i].name,w,h,Mp[i].W,Mp[i].H,Mp[i].category,true};
                            countCp ++;
                        }
                    }
                }
                in.close();
            }
            if(btn[7].click())
            {
                page = "help";
            }
            if(btn[10].click())
            {
                ScreenCapture(150,100,1340,690, "1.bmp", txWindow());
                txMessageBox("Сохранено в 1.bmp");
            }
        }
        if(page == "help")
        {
            if(GetAsyncKeyState(VK_ESCAPE))
            {
                page = "kartina";
            }
            txSetFillColor(RGB(106,191,198));
            txRectangle(50,30,1450,770);
            txSelectFont("Comic Sans MC",45);
            txSetColor(TX_BLACK);
            txDrawText(50,30,1450,100,"Программа составления натюрморта");
            txDrawText(50,100,1450,250,"С помощью данной программы вы можете составлять собственные композиции\n"" натюрморта, сохранять и делится с друзьями своим творением");
            txDrawText(50,250,1450,770,"ФУНКЦИОНАЛ:\n"
            "Чтобы добавить картинку на рабочую область нажмите на кнопку\n"
            "с соответсвующей категорией потом на картинку в меню\n"
            "Передвежение картинок мышкой и клавишами WASD, Увеличение и уменьшение '+' и '-' соотвественно\n"
            "Чтобы удалить картинку нажмите 'Delete'\n"
            "Можно сохранять и загружать композиции если нажать на эти кнопки\n"
            "Если нажать снимок экрана композиция сохраниться в формате 'bmp'\n"
            "Все что вы сохраняете отправляется в папку проекта\n"
            "Выход из программы осуществляется с помощью кнопки 'ВЫХОД'\n"
            "Чтобы выйти в меню нажмите 'ESC'");
        }
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

