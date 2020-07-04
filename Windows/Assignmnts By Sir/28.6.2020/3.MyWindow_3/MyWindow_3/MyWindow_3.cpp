#include<windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//Variable declaration 

	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyApp");

	//code 

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = NULL;

	//Register class 

	RegisterClassEx(&wndclass);

	//Create Window

	hwnd = CreateWindow(szAppName, TEXT("Bhavesh Dilip Joshi!!!"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd,iCmdShow);
	UpdateWindow(hwnd);

	//Message Loop

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wparam, LPARAM lparam)
{
	switch (iMsg)
	{
	case WM_CREATE:
		MessageBox(hwnd, TEXT("WM_CREATE Received!!"), TEXT("My Message"), MB_TOPMOST |MB_ICONINFORMATION | MB_OK);
		break;

	case WM_LBUTTONDOWN:
		MessageBox(hwnd, TEXT("WM_LBUTTONDOWN Received!!"), TEXT("My Message"),MB_ICONINFORMATION | MB_OKCANCEL);
		break;

	case WM_RBUTTONDOWN:
		MessageBox(hwnd, TEXT("WM_RBUTTONDOWN Received!!"), TEXT("My Message"), MB_ICONINFORMATION | MB_YESNOCANCEL);
		break;


	case WM_DESTROY:
		MessageBox(hwnd, TEXT("WM_DESTROY Received!!"), TEXT("My Message"), MB_ICONINFORMATION | MB_OK);
		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wparam, lparam));
}