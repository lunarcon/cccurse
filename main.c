#include "windefs.h"
#include <stdio.h>

DWORD acrylicColor = 0x90101010;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_VREDRAW | CS_HREDRAW, WindowProc, 0L, 0L, hInstance, NULL, NULL, NULL, NULL, "C Form", NULL };
    RegisterClassEx(&wc);

    HWND hwnd = CreateWindow(wc.lpszClassName, "Hello Sayer", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 350, 200, NULL, NULL, hInstance, NULL);

    HWND hButton = Button("Display", 10, 10, 100, 30, hwnd, hInstance, 1);

    HWND hChangeColorButton = Button("Change Color", 120, 10, 100, 30, hwnd, hInstance, 6);

    HWND hButton2 = Button("Clear TextBox", 10, 50, 100, 30, hwnd, hInstance, 2);

    HWND hlabel2 = Label("Opacity", 120, 50, 50, 20, hwnd, hInstance, 7);
    HWND hOpButtonDec = Button("-", 180, 50, 20, 20, hwnd, hInstance, 8);
    HWND hOpButtonInc = Button("+", 200, 50, 20, 20, hwnd, hInstance, 9);
    HWND hOpLabel = Label("0.56", 230, 50, 50, 20, hwnd, hInstance, 10);

    HWND hlabel1 = Label("Enter Text", 10, 90, 100, 20, hwnd, hInstance, 5);

    HWND hTextBox = TextBox(getUserName(), 100, 90, 200, 20, hwnd, hInstance, 3);

    HWND hProgressBar = ProgressBar(10, 120, 300, 20, hwnd, hInstance, 4);
    SetProgressBarValue(hProgressBar, 50);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void changeColor(HWND hwnd) {
    SetBGEffect(hwnd, acrylicColor=(DWORD)(0x90000000 + (rand() % 0x00FFFFFF)), TRUE, 0);
}

void changeOpacity(HWND hwnd, int opacity) {
    opacity = opacity < 0 ? 0 : opacity > 255 ? 255 : opacity;
    SetBGEffect(hwnd, acrylicColor=(acrylicColor & 0x00FFFFFF) + (opacity << 24), TRUE, 0);
}

int getOpacity() {
    return (acrylicColor & 0xFF000000) >> 24;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
        {
            APPDATA* appData = (APPDATA*)malloc(sizeof(APPDATA));
            appData->hwnd = hwnd;
            appData->hBrush = CreateSolidBrush(RGB(0,0,0));
            appData->hIcon = (HICON)LoadImage(NULL, "wicon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)appData);
            SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)appData->hIcon);
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)appData->hIcon);
            SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)appData->hBrush);
            SetBGEffect(hwnd, acrylicColor, TRUE, 0);

        }
        return 0;

        case WM_ACTIVATE:
            return 0;


        case WM_COMMAND:
            if (HIWORD(wParam) == BN_CLICKED)
            {
                int id = LOWORD(wParam);
                if (id == 1) {
                    HWND hTextBox = GetDlgItem(hwnd, 3);
                    int len = GetWindowTextLength(hTextBox);
                    char* text = (char*)malloc(len + 1);
                    GetWindowText(hTextBox, text, len + 1);
                    if (strlen(text) == 0) {
                        MessageBox(hwnd, "Empty field", "Notice", MB_OK);
                        return 0;
                    }
                    char* msg = (char*)malloc(len + 8);
                    strcpy(msg, "Hello, ");
                    strcat(msg, text);
                    MessageBox(hwnd, msg, "Notice", MB_OK);
                    free(text);
                    free(msg);
                }
                else if (id == 2) {
                    HWND hTextBox = GetDlgItem(hwnd, 3);
                    SetWindowText(hTextBox, "");
                    MessageBox(hwnd, "TextBox cleared", "Notice", MB_OK);
                }
                else if (id == 6) {
                    changeColor(hwnd);
                }
                else if (id == 8) {
                    int opacity = getOpacity();
                    if (opacity > 0) {
                        changeOpacity(hwnd, opacity - 10);
                    }
                    char* opformat = (char*)malloc(5);
                    sprintf(opformat, "%.2f", (float)opacity / 255);
                    SetWindowText(GetDlgItem(hwnd, 10), opformat);
                    free(opformat);
                }
                else if (id == 9) {
                    int opacity = getOpacity();
                    if (opacity < 255) {
                        changeOpacity(hwnd, opacity + 10);
                    }
                    char* opformat = (char*)malloc(5);
                    sprintf(opformat, "%.2f", (float)opacity / 255);
                    SetWindowText(GetDlgItem(hwnd, 10), opformat);
                    free(opformat);
                }
            }
            return 0;

        case WM_DESTROY:
            DestroyIcon(((APPDATA*)GetWindowLongPtr(hwnd, GWLP_USERDATA))->hIcon);
            DeleteObject(((APPDATA*)GetWindowLongPtr(hwnd, GWLP_USERDATA))->hBrush);
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}
