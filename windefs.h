#include <Windows.h>
typedef struct _ACCENT_POLICY {
    DWORD AccentState;
    DWORD AccentFlags;
    DWORD GradientColor;
    DWORD AnimationId;
} ACCENT_POLICY;

typedef enum _WINDOWCOMPOSITIONATTRIB {
    WCA_UNDEFINED = 0,
    WCA_NCRENDERING_ENABLED = 1,
    WCA_NCRENDERING_POLICY = 2,
    WCA_TRANSITIONS_FORCEDISABLED = 3,
    WCA_ALLOW_NCPAINT = 4,
    WCA_CAPTION_BUTTON_BOUNDS = 5,
    WCA_NONCLIENT_RTL_LAYOUT = 6,
    WCA_FORCE_ICONIC_REPRESENTATION = 7,
    WCA_EXTENDED_FRAME_BOUNDS = 8,
    WCA_HAS_ICONIC_BITMAP = 9,
    WCA_THEME_ATTRIBUTES = 10,
    WCA_NCRENDERING_EXILED = 11,
    WCA_NCADORNMENTINFO = 12,
    WCA_EXCLUDED_FROM_LIVEPREVIEW = 13,
    WCA_VIDEO_OVERLAY_ACTIVE = 14,
    WCA_FORCE_ACTIVEWINDOW_APPEARANCE = 15,
    WCA_DISALLOW_PEEK = 16,
    WCA_CLOAK = 17,
    WCA_CLOAKED = 18,
    WCA_ACCENT_POLICY = 19,
    WCA_FREEZE_REPRESENTATION = 20,
    WCA_EVER_UNCLOAKED = 21,
    WCA_VISUAL_OWNER = 22,
    WCA_LAST = 23
} WINDOWCOMPOSITIONATTRIB;

typedef enum ACCENT_STATE {
    ACCENT_DISABLED = 0,
    ACCENT_ENABLE_GRADIENT = 1,
    ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
    ACCENT_ENABLE_BLURBEHIND = 3,
    ACCENT_ENABLE_ACRYLICBLURBEHIND = 4,
    ACCENT_ENABLE_HOSTBACKDROP = 5,
    ACCENT_INVALID_STATE = 6
} ACCENT_STATE;

typedef struct _WINDOWCOMPOSITIONATTRIBDATA {
    WINDOWCOMPOSITIONATTRIB Attrib;
    PVOID pvData;
    SIZE_T cbData;
} WINDOWCOMPOSITIONATTRIBDATA;

typedef BOOL(WINAPI *pSetWindowCompositionAttribute)(HWND, WINDOWCOMPOSITIONATTRIBDATA*);

typedef struct _IMMERSIVE_COLOR_SET_PARAMETERS {
    UINT32 dwImmersiveColorSet;
    UINT32 dwImmersiveColorType;
    BOOL bIgnoreHighContrast;
    UINT32 dwHighContrastCacheMode;
} IMMERSIVE_COLOR_SET_PARAMETERS;


typedef struct{
    HWND hwnd;
    HBRUSH hBrush;
    HICON hIcon;
} APPDATA;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

char* getUserName () {
    char* userName = (char*)malloc(100);
    DWORD userNameLen = 100;
    GetUserName(userName, &userNameLen);
    return userName;
}

void SetBGEffect(HWND hWnd, DWORD gradientColor, BOOL isEnableShadow, DWORD animationId) {
    HMODULE hModule = LoadLibrary(TEXT("user32.dll"));
    if (hModule)
    {
        pSetWindowCompositionAttribute SetWindowCompositionAttributeFunc = (pSetWindowCompositionAttribute)GetProcAddress(hModule, "SetWindowCompositionAttribute");
        if (SetWindowCompositionAttributeFunc)
        {
            ACCENT_POLICY accentPolicy;
            WINDOWCOMPOSITIONATTRIBDATA winCompAttrData;

            accentPolicy.AccentState = ACCENT_ENABLE_ACRYLICBLURBEHIND;
            accentPolicy.AccentFlags = isEnableShadow ? (0x20 | 0x40 | 0x80 | 0x100) : 0;
            accentPolicy.GradientColor = gradientColor;
            accentPolicy.AnimationId = animationId;

            winCompAttrData.Attrib = WCA_ACCENT_POLICY;
            winCompAttrData.pvData = &accentPolicy;
            winCompAttrData.cbData = sizeof(accentPolicy);

            SetWindowCompositionAttributeFunc(hWnd, &winCompAttrData);
        }
        FreeLibrary(hModule);
    }
}

HWND Button(char* text, int x, int y, int width, int height, HWND parent, HINSTANCE hInstance, int id) {
    return CreateWindow("BUTTON", text, WS_TABSTOP | WS_VISIBLE | WS_CHILD, x, y, width, height, parent, (HMENU)id, hInstance, NULL);
}

HWND TextBox(char* text, int x, int y, int width, int height, HWND parent, HINSTANCE hInstance, int id) {
    return CreateWindow("EDIT", text, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, x, y, width, height, parent, (HMENU)id, hInstance, NULL);
}

HWND Label(char* text, int x, int y, int width, int height, HWND parent, HINSTANCE hInstance, int id) {
    return CreateWindow("STATIC", text, WS_VISIBLE | WS_CHILD, x, y, width, height, parent, (HMENU)id, hInstance, NULL);
}

HWND ComboBox(int x, int y, int width, int height, HWND parent, HINSTANCE hInstance, int id) {
    return CreateWindow("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, x, y, width, height, parent, (HMENU)id, hInstance, NULL);
}

HWND ListBox(int x, int y, int width, int height, HWND parent, HINSTANCE hInstance, int id) {
    return CreateWindow("LISTBOX", "", WS_VISIBLE | WS_CHILD | LBS_NOTIFY | WS_VSCROLL, x, y, width, height, parent, (HMENU)id, hInstance, NULL);
}

HWND Panel(int x, int y, int width, int height, HWND parent, HINSTANCE hInstance, int id) {
    return CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD | WS_BORDER, x, y, width, height, parent, (HMENU)id, hInstance, NULL);
}

HWND ProgressBar(int x, int y, int width, int height, HWND parent, HINSTANCE hInstance, int id) {
    return CreateWindow("msctls_progress32", "", WS_VISIBLE | WS_CHILD, x, y, width, height, parent, (HMENU)id, hInstance, NULL);
}

void SetProgressBarValue(HWND progressBar, int value) {
    SendMessage(progressBar, (UINT)0x0402, (WPARAM)value, 0);
}