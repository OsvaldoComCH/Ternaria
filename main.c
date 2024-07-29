/* Compile:
gcc main.c -o Ternaria.exe -l gdi32 -l msimg32
*/

#include "imports.h"
#include "input.c"

//Flag para indicar se a thread está rodando
int ThreadRunning = 1;
HANDLE Thread;

//Função da thread principal
DWORD WINAPI MainThread(LPVOID lpParam)
{
    srand(time(NULL));
    //Se o jogo for reiniciado depois do game over, esta flag é falsa e o mapa não é gerado
    if(ThreadRunning)
    {
        CreateArchive();
    }
    ThreadRunning = 1;
    HWND hwnd = *((HWND *)lpParam);//Recupera-se o Handle para a janela do parâmetro da thread

    DArrayCreate(&Map, 200);
    ReadArchive(&Map); 

    int gameover = 0, count = 0;
    character player;
    SpawnPlayer(&player);

    zombie zombie;
    zombie.baseLife = 10;
    SpawnZombie(&zombie, &player);
    
    /*
    Normalmente, se usa um sleep para esperar uma determinada quantidade de tempo. As funções de sleep são
    imprecisas, pois liberam a thread para o sistema operacional, que pode te devolver depois do prazo.
    Os timers do Windows travam a thread, mas não a liberam, tornando-os muito precisos e eficientes.
    */

    HANDLE Timer = CreateWaitableTimer(NULL, 0, NULL);
    LARGE_INTEGER DueTime;
    DueTime.QuadPart = -333333;
    SetWaitableTimer(Timer, &DueTime, 33, NULL, NULL, 0);//Timer com intervalo de 33ms (30 fps)
    
    HDC hdc = GetDC(hwnd);//Handle com as informações necessárias para se desenhar na tela
    RECT R;
    GetClientRect(hwnd, &R);
    RenderBkgd(hdc);
    RenderMap(&Map, hdc);
    RenderLife(hdc, player.life);
    RenderInv(hdc, &player);
    ReleaseDC(hwnd, hdc);

    RECT hud = {10, 10, 317, 82};
    int renderHud = 0;
    while(player.life > 0)
    {
        HDC hdc = GetDC(hwnd);
        RECT R;
        GetClientRect(hwnd, &R);
        /*
        Para se desenhar na tela, se cria um handle HDC compatível com a tela na memória (TempDC).
        Selecionamos um bitmap neste HDC para definir o tamanho e as cores.
        Desenhamos tudo neste DC da memória primeiro, depois fazemos uma única transferência para a tela
        usando o BitBlt
        */
        HDC TempDC = CreateCompatibleDC(hdc);
        HBITMAP Bitmap = CreateCompatibleBitmap(hdc, (R.right-R.left), (R.bottom - R.top));
        SelectObject(TempDC, Bitmap);
        BitBlt(TempDC, 0, 0, R.right-R.left, R.bottom-R.top, hdc, 0, 0, SRCCOPY);

        //Isto redesenha o hud caso o jogador ou o zumbi passem na frente dele
        if(Collision(&player.hitbox, &hud) || Collision(&zombie.hitbox, &hud))
        {
            renderHud = 1;
        }else
        {
            renderHud = 0;
        }

        EraseRect(TempDC, &player.hitbox);
        EraseRect(TempDC, &zombie.hitbox);
        Input(TempDC, &player, &zombie, &Map);
        MoveZombie(&player, &zombie);
        
        DefineMap(&Map);
        RenderBkgd(TempDC);
        RenderMap(&Map, TempDC);

        // if(renderHud)
        // {
        //     RenderInv(TempDC, &player);
        //     RenderLife(TempDC, player.life);
        // }

        if(zombie.life > 0)
        {
            RenderZombie(&zombie, TempDC);
        }
        RenderPlayer(&player, TempDC);
        RenderTool(&player, TempDC);

        count += 1;
        if(count == 300)
        {
            Regeneration(&player);
            RenderLife(TempDC, player.life);
            count = 0;
        }

        if(player.hitbox.top > 1000)
        {
            player.life = 0;
        }
        if(zombie.hitbox.top > 1000)
        {
            if(!zombie.respawn)
            {
                DamageZombie(&zombie, 200);
            }
        }
        printf("%d, %d\n", mapax, mapay);

        BitBlt(hdc, 0, 0, R.right-R.left, R.bottom-R.top, TempDC, 0, 0, SRCCOPY);
        DeleteDC(TempDC);
        DeleteObject(Bitmap);
        ReleaseDC(hwnd, hdc);

        WaitForSingleObject(Timer, INFINITE);//Aqui se espera até o timer terminar
    }

    HFONT Font = CreateFont(72, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
    hdc = GetDC(hwnd);
    RECT R1 = {200, 300, 675, 450};
    SelectObject(hdc, Font);
    DrawRect(hdc, &R1, RGB(100,100,100));
    SetBkColor(hdc, RGB(100,100,100));
    TextOut(hdc, R1.left + 50, R1.top + 40, L"GAME OVER", 10);
    ReleaseDC(hwnd, hdc);
    DArrayDestroy(&Map);
    ThreadRunning = 0;
}

//Função para o tratamanto de mensagens
LRESULT CALLBACK WndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch(Msg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT PS;
            HDC hdc = BeginPaint(hwnd, &PS);
            FillRect(hdc, &PS.rcPaint, CreateSolidBrush(RGB(255,255,255)));
            EndPaint(hwnd, &PS);
        }
        break;
        case WM_CLOSE:
            DArrayDestroy(&Map);
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        case WM_KEYDOWN:
        {
            //Se a thread estiver acabado e o jogador pressionar Return (Enter), a thread roda novamente
            if(wParam == VK_RETURN && ThreadRunning == 0)
            {
                TerminateThread(Thread, 0);
                Thread = CreateThread(NULL, 0, MainThread, &hwnd, 0, NULL);
            }
        }
        default:
            return DefWindowProc(hwnd, Msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const wchar_t WClassName[] = L"Ternaria";
    MSG Msg;
    WNDCLASS Window = {};
    Window.lpfnWndProc = WndProc;
    Window.hInstance = hInstance;
    Window.lpszClassName = WClassName;
    Window.hCursor = (HCURSOR)LoadImage(NULL, L"imagens/Hand.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    Window.hIcon = (HICON)LoadImage(NULL, L"imagens/iconeTernaria.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

    if(!RegisterClass(&Window))
    {
        MessageBox(NULL, L"Window Class Registration Failed", L"Error", MB_ICONERROR | MB_OK);
        return 0;
    }

    HWND hwnd = CreateWindowEx
    (
        0,
        WClassName,
        L"Ternaria",
        WS_OVERLAPPEDWINDOW,
        0, 0, 1920, 1080,
        NULL, NULL, hInstance, NULL
    );

    if(hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed", L"Error", MB_ICONERROR | MB_OK);
        return 0;
    }
    Ghwnd = hwnd;
    
    
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    //Abrindo uma thread separada para rodar o jogo
    Thread = CreateThread(NULL, 0, MainThread, &hwnd, 0, NULL);

    //Loop para receber as mensagens da WINAPI
    while(GetMessage(&Msg, NULL, 0, 0))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return 0;
}
