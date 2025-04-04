#include <windows.h>
#include <stdio.h>

LPCTSTR g_title = TEXT("윈도우 타이틀바에 표시할 문자열");
LPCTSTR g_szClassName = TEXT("윈도우 클래스 이름");

int g_width = 1024;
int g_height = 768;


// 콘솔 초기화
void InitConsole()
{
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	SetConsoleTitle(L"윈도우 메시지 콘솔 로그");
	printf("콘솔 로그 시작...\n\n");
}

void UninitConsole()
{
	// 표준 출력 스트림 닫기
	fclose(stdout);
	// 콘솔 해제
	FreeConsole();
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		printf("WM_CREATE: 윈도우가 생성되었습니다.\n");
		break;

	case WM_PAINT:
		printf("WM_PAINT: 윈도우가 WM_PAINT발생.\n");
		break;
	case WM_ERASEBKGND:
		printf("WM_ERASEBKGND: 윈도우가 WM_ERASEBKGND발생.\n");
		break;
	case WM_KEYDOWN:
		printf("WM_KEYDOWN: VK_CODE = %d\n", (int)wParam);
		break;
	case WM_ENTERSIZEMOVE:
		printf("WM_ENTERSIZEMOVE:  창 이동/크기 조절중\n");
		break;

	case WM_EXITSIZEMOVE:
		printf("WM_EXITSIZEMOVE: 창 이동/크기 조절 종료.\n");
		break;

	case WM_CHAR:
		printf("WM_CHAR: 문자 입력 = '%c'\n", (char)wParam);
		break;

	case WM_LBUTTONDOWN:
		printf("WM_LBUTTONDOWN: 클릭 위치 x=%d y=%d\n", LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_DESTROY:
		printf("WM_DESTROY: 프로그램 종료\n");
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void WndClassInit(HINSTANCE hInstance);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	InitConsole();  // 콘솔 출력 초기화

	// 윈도우 클래스 지정 하기 
	// 커서 모양 지정 가능
	WndClassInit(hInstance);

	// 원하는 크기가 조정되어 리턴
	RECT rcClient = { 0, 0, (LONG)g_width, (LONG)g_height };
	AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);

	//생성
	HWND hwnd = CreateWindow(
		g_szClassName,
		g_title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	HDC hDC = GetDC(hwnd);

	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		::PatBlt(hDC, 0, 0, g_width, g_height, WHITENESS);
		const int CELL_SIZE = 300;     // 셀 크기
		const int PADDING = 10;       // 셀 내부 여백
		int cols = g_width / CELL_SIZE;
		int rows = g_height / CELL_SIZE;

		RECT rect = { 0, 0, g_width, g_height };
		for (int y = 0; y < rows; ++y)
		{
			for (int x = 0; x < cols; ++x)
			{
				int left = x * CELL_SIZE + PADDING;
				int top = y * CELL_SIZE + PADDING;
				int right = (x + 1) * CELL_SIZE - PADDING;
				int bottom = (y + 1) * CELL_SIZE - PADDING;
				Ellipse(hDC, left, top, right, bottom);
			}
		}

	}

	ReleaseDC(hwnd, hDC);

	UninitConsole();  // 콘솔 출력 해제
	return (int)msg.wParam;
}

//윈도우 클래스 지정 하기 
// 커서 모양 지정 가능
void WndClassInit(HINSTANCE hInstance)
{ 
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;							// 윈도우 프로시저를 받아온다. 
	wc.hInstance = hInstance;							// 만들어진 프로세스를 받아온다. 
	wc.lpszClassName = g_szClassName;					// 아이디의 이름. 
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);		// 브러쉬로 변환한 Window 창의 백그라운드(배경) 을 받아온다. 
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// 기본 커서 모양
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);			// 기본 아이콘 모양
	RegisterClass(&wc);
}