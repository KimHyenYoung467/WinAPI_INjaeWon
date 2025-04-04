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

// 메시지 ID를 문자열 이름으로 변환
const char* GetMessageName(UINT msg)
{
	switch (msg)
	{
	case WM_NULL: return "WM_NULL";
	case WM_CREATE: return "WM_CREATE";
	case WM_DESTROY: return "WM_DESTROY";
	case WM_MOVE: return "WM_MOVE";
	case WM_SIZE: return "WM_SIZE";
	case WM_ACTIVATE: return "WM_ACTIVATE";
	case WM_SETFOCUS: return "WM_SETFOCUS";
	case WM_KILLFOCUS: return "WM_KILLFOCUS";
	case WM_ENABLE: return "WM_ENABLE";
	case WM_SETREDRAW: return "WM_SETREDRAW";
	case WM_SETTEXT: return "WM_SETTEXT";
	case WM_GETTEXT: return "WM_GETTEXT";
	case WM_GETTEXTLENGTH: return "WM_GETTEXTLENGTH";
	case WM_PAINT: return "WM_PAINT";
	case WM_CLOSE: return "WM_CLOSE";
	case WM_QUERYENDSESSION: return "WM_QUERYENDSESSION";
	case WM_QUIT: return "WM_QUIT";
	case WM_QUERYOPEN: return "WM_QUERYOPEN";
	case WM_ERASEBKGND: return "WM_ERASEBKGND";
	case WM_SYSCOLORCHANGE: return "WM_SYSCOLORCHANGE";
	case WM_SHOWWINDOW: return "WM_SHOWWINDOW";
	case WM_WININICHANGE: return "WM_WININICHANGE";
	case WM_DEVMODECHANGE: return "WM_DEVMODECHANGE";
	case WM_ACTIVATEAPP: return "WM_ACTIVATEAPP";
	case WM_FONTCHANGE: return "WM_FONTCHANGE";
	case WM_TIMECHANGE: return "WM_TIMECHANGE";
	case WM_CANCELMODE: return "WM_CANCELMODE";
	case WM_SETCURSOR: return "WM_SETCURSOR";
	case WM_MOUSEACTIVATE: return "WM_MOUSEACTIVATE";
	case WM_CHILDACTIVATE: return "WM_CHILDACTIVATE";
	case WM_QUEUESYNC: return "WM_QUEUESYNC";
	case WM_GETMINMAXINFO: return "WM_GETMINMAXINFO";

		// 마우스 관련	
	case WM_LBUTTONDOWN: return "WM_LBUTTONDOWN";
	case WM_LBUTTONUP: return "WM_LBUTTONUP";
	case WM_LBUTTONDBLCLK: return "WM_LBUTTONDBLCLK";
	case WM_RBUTTONDOWN: return "WM_RBUTTONDOWN";
	case WM_RBUTTONUP: return "WM_RBUTTONUP";
	case WM_RBUTTONDBLCLK: return "WM_RBUTTONDBLCLK";
	case WM_MBUTTONDOWN: return "WM_MBUTTONDOWN";
	case WM_MBUTTONUP: return "WM_MBUTTONUP";
	case WM_MBUTTONDBLCLK: return "WM_MBUTTONDBLCLK";
	case WM_MOUSEWHEEL: return "WM_MOUSEWHEEL";
	case WM_MOUSEHWHEEL: return "WM_MOUSEHWHEEL";

		// 키보드 관련
	case WM_KEYDOWN: return "WM_KEYDOWN";
	case WM_KEYUP: return "WM_KEYUP";
	case WM_CHAR: return "WM_CHAR";
	case WM_DEADCHAR: return "WM_DEADCHAR";
	case WM_SYSKEYDOWN: return "WM_SYSKEYDOWN";
	case WM_SYSKEYUP: return "WM_SYSKEYUP";
	case WM_SYSCHAR: return "WM_SYSCHAR";
	case WM_SYSDEADCHAR: return "WM_SYSDEADCHAR";
	case WM_HOTKEY: return "WM_HOTKEY";

		// 비클라이언트 영역
	case WM_NCMOUSEMOVE: return "WM_NCMOUSEMOVE";
	case WM_NCLBUTTONDOWN: return "WM_NCLBUTTONDOWN";
	case WM_NCLBUTTONUP: return "WM_NCLBUTTONUP";
	case WM_NCLBUTTONDBLCLK: return "WM_NCLBUTTONDBLCLK";
	case WM_NCRBUTTONDOWN: return "WM_NCRBUTTONDOWN";
	case WM_NCRBUTTONUP: return "WM_NCRBUTTONUP";
	case WM_NCRBUTTONDBLCLK: return "WM_NCRBUTTONDBLCLK";
	case WM_NCMBUTTONDOWN: return "WM_NCMBUTTONDOWN";
	case WM_NCMBUTTONUP: return "WM_NCMBUTTONUP";
	case WM_NCMBUTTONDBLCLK: return "WM_NCMBUTTONDBLCLK";
	case WM_NCHITTEST: return "WM_NCHITTEST";
	case WM_NCPAINT: return "WM_NCPAINT";
	case WM_NCACTIVATE: return "WM_NCACTIVATE";

		// 시스템 명령
	case WM_SYSCOMMAND: return "WM_SYSCOMMAND";

		// 기타
	case WM_TIMER: return "WM_TIMER";
	case WM_COMMAND: return "WM_COMMAND";
	case WM_INITDIALOG: return "WM_INITDIALOG";

	default: return "Unknown Message";
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		printf("WM_CREATE: 윈도우가 생성되었습니다.\n");
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(hwnd, &ps);
		const char* text = "윈도우 메시지출력 중";
		TextOutA(hDC, 10, 10, text, (int)strlen(text));
					
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

		EndPaint(hwnd, &ps);
		printf("WM_PAINT: 화면 다시 그리기\n");
	}
	break;

	case WM_KEYDOWN:
		printf("WM_KEYDOWN: VK_CODE = %d\n", (int)wParam);
		break;

	case WM_CHAR:
		printf("WM_CHAR: 문자 입력 = '%c'\n", (char)wParam);
		break;

	case WM_LBUTTONDOWN:
		printf("WM_LBUTTONDOWN: 클릭 위치 x=%d y=%d\n", LOWORD(lParam), HIWORD(lParam));
		break;
	
	// 메세지가 너무 자주 나오는것은 출력하지 않음
	case WM_MOUSEMOVE:
	case WM_NCHITTEST:
	case WM_NCMOUSEMOVE:
	case WM_SETCURSOR:
		break;
	

	case WM_DESTROY:
		printf("WM_DESTROY: 프로그램 종료\n");
		PostQuitMessage(0);
		break;

	default:		
		printf("%s (0x%04X)\n", GetMessageName(msg), msg);
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	InitConsole();  // 콘솔 출력 초기화

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

	MSG msg;
	// 메시지 루프
	while (GetMessage(&msg, NULL, 0, 0)) // GetMessage는 큐에 메시지가 없으면 대기(Block)함	
	{
		TranslateMessage(&msg);	// 이부분을 없애면	WM_CHAR 이벤트없음
		DispatchMessage(&msg);  // 윈도우 프로시저로 메시지 전달
	}

	UninitConsole();  // 콘솔 출력 해제
	return (int)msg.wParam;
}
