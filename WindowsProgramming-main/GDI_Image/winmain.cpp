#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "Msimg32.lib")	

LPCTSTR g_title = TEXT("윈도우 타이틀바에 표시할 문자열");
LPCTSTR g_szClassName = TEXT("윈도우 클래스 이름");

int g_width = 1024;
int g_height = 768;

HWND g_hWnd;
HDC g_FrontBufferDC;    // 앞면 DC
HDC g_BackBufferDC;    // 뒷면 DC
HBITMAP g_BackBufferBitmap;

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

// WIN32 API 에러 값에 대한 실제 메세지를 출력하는 함수
void PrintLastErrorMessage()
{
	DWORD errorCode = GetLastError();
	LPVOID lpMsgBuf;

	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 기본 언어
		(LPSTR)&lpMsgBuf,
		0,
		NULL);

	if (lpMsgBuf)
	{
		printf("오류 코드: %lu\n오류 메시지: %s\n", errorCode, (char*)lpMsgBuf);
		LocalFree(lpMsgBuf); // 할당된 버퍼 해제
	}
	else
	{
		printf("오류 코드: %lu (메시지를 찾을 수 없음)\n", errorCode);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	InitConsole();  // 콘솔 출력 초기화

	char szPath[MAX_PATH] = { 0, };
	::GetCurrentDirectoryA(MAX_PATH, szPath);
	printf("Current Directory: %s\n", szPath);

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = g_szClassName;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	// 기본 커서 모양
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// 기본 아이콘 모양
	RegisterClass(&wc);

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

	////////Renderer::Initialize
	g_FrontBufferDC = GetDC(hwnd); //윈도우 클라이언트 영역의 DeviceContext얻기
	g_BackBufferDC = CreateCompatibleDC(g_FrontBufferDC); // 호환되는 DeviceContext 생성
	g_BackBufferBitmap = CreateCompatibleBitmap(g_FrontBufferDC, g_width, g_height); // 메모리 영역생성
	SelectObject(g_BackBufferDC, g_BackBufferBitmap); // MemDC의 메모리영역 지정

	// DeviceContext 생성및 HBitmap 연결
	HDC hImageDC = CreateCompatibleDC(g_FrontBufferDC); // 호환되는 DeviceContext 생성
	HBITMAP hImageBitmap = (HBITMAP)LoadImage(
		NULL,                    // 인스턴스 핸들 (파일이므로 NULL)
		L"../Resource/elf24.bmp",          // BMP 파일 경로
		IMAGE_BITMAP,            // 비트맵 로드
		0, 0,                    // 크기 자동 (원본 크기)
		LR_LOADFROMFILE        // 외부 파일에서 로드
	);	
	BITMAP bmpInfo; // 내부에 크기 정보가 있다.
	GetObject(hImageBitmap, sizeof(BITMAP), &bmpInfo);


	if (!hImageBitmap)
		PrintLastErrorMessage();

	SelectObject(hImageDC, hImageBitmap); 


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

		// Renderer::BeginDraw()
		PatBlt(g_BackBufferDC, 0, 0, g_width, g_height, BLACKNESS);

		// Render()
		// 전체 이미지 그대로 복사하기
		BitBlt(g_BackBufferDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, hImageDC, 0, 0, SRCCOPY);

		//특정 컬러(컬러키)를 제외하고 그리기
		TransparentBlt(g_BackBufferDC, bmpInfo.bmWidth, 0, bmpInfo.bmWidth, bmpInfo.bmHeight,
			hImageDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, 
			RGB(255,255,255));	// Msimg32.lib 링크 필요

		// Renderer::EndDraw()
		BitBlt(g_FrontBufferDC, 0, 0, g_width, g_height, g_BackBufferDC, 0, 0, SRCCOPY);
	}

	DeleteObject(hImageBitmap);
	DeleteDC(hImageDC);

	// Renderer::Uninitialize
	DeleteObject(g_BackBufferBitmap);
	DeleteDC(g_BackBufferDC);
	ReleaseDC(hwnd, g_FrontBufferDC);
	//////////////////////////////////////////////////////////////////////////

	UninitConsole();  // 콘솔 출력 해제
	return (int)msg.wParam;
}
