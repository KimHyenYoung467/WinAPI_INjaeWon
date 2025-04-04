#include <windows.h>

// 앞의 const는 포인터가 문자열의 내용을 변경할수 없음을 의미한다. 주소값은 변경가능
const char* g_Text="멀티바이트 문자열(즉, 일반적인 ASCII 또는 UTF-8 인코딩 문자열)입니다.";
LPCSTR g_Text2 = g_Text;
const wchar_t* g_wText = L"유니코드 와이드 문자열 입니다";
LPCWSTR	g_wText2 = g_wText;
const TCHAR* g_TText = TEXT("TCHAR 문자열 입니다");
bool g_boolType = false; // 0은 false, 0아닌값은 true 로 변환
BOOL g_BoolType = -1;

// 윈도우 프로시저
// 윈도우 메시지를 처리하는 함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_DESTROY) 
	{
		PostQuitMessage(0); // WM_QUIT 메시지를 큐에 넣어줍니다.
		return 0;
	}
	//우리가 직접 처리하지 않은 메시지는 DefWindowProc에 넘겨줘야 Windows가 기본 처리를 해줍니다.
	return DefWindowProc(hWnd, message, wParam, lParam);
}

// WinMain 함수
// 프로그램의 진입점
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow) // 실행	옵션
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;	// 윈도우 프로시저
	wc.hInstance = hInstance;   // 가상메모리에서 EXE모듈의 시작주소
	wc.lpszClassName = TEXT("MyWindowClass");	// 클래스 이름
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	// 윈도우 배경색
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	// 기본 커서 모양
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// 기본 아이콘 모양

	RegisterClass(&wc);

	HWND hWnd = CreateWindow(
		TEXT("MyWindowClass"),      // 클래스 이름
		TEXT("My Basic Window"),    // 윈도우 타이틀
		WS_OVERLAPPEDWINDOW,   // 윈도우 스타일
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, // 각종 스타일을 포함한	크기
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow); // 윈도우 보이기
	UpdateWindow(hWnd);		 // 윈도우 업데이트

	MSG msg;	// 메시지 구조체와 루프 
	// GetMessage는 큐에 메시지가 없으면 대기(Block)함 ,WM_QUIT 메시지가 발생하면 0 을 반환
	while (GetMessage(&msg, NULL, 0, 0))  
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}