// Module:		CIS023_S2020_HW17.cpp
// Author:		Miguel Antonio Logarta
// Date:		April 27, 2020
// Purpose:		Learn how to create and use two queues as a linked list.
//				Initialize, add, get, and delete vector - based queues.
//				Perform file comparisons.
//

#include "framework.h"
#include "CIS023_S2020_HW17.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND hWnd_InFile[2];							// handles to filename displays
HWND hWnd_btnFile[2];							// handles to File buttons
HWND hWnd_btnCompare;							// handle to Compare button

string strFileSpec[2];							// file paths

vector<char> vFile1;							// Stack for the 1st file
vector<char> vFile2;							// Stack for the 2nd file

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

bool GetFileName(int);						// standard file open dialog box
bool Push();								// load the two files into the vector stacks
bool CompareFiles();						// unload the vectors, and compare them

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CIS023S2020HW17, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CIS023S2020HW17));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CIS023S2020HW17));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+0);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CIS023S2020HW17);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 410, 200, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case BN_CLICKED:	// capture button click
			if (LOWORD(lParam) == (WORD)hWnd_btnFile[0])	// launch standard file open dialog
			{
				if (GetFileName(0))
					EnableWindow(hWnd_btnFile[1], true);
			}

			if (LOWORD(lParam) == (WORD)hWnd_btnFile[1])	// launch standard file open dialog
			{
				if (GetFileName(1))
					EnableWindow(hWnd_btnCompare, true);
			}

			else if (LOWORD(lParam) == (WORD)hWnd_btnCompare) // compare files
			{
				// Make sure the stack is empty first before comparing
				vFile1.clear();
				vFile2.clear();

				if (Push())									// push file contents onto stacks
					if (CompareFiles())
						MessageBox(NULL, TEXT("The files are identical."), TEXT("File Compare"), MB_ICONASTERISK);
					else
						MessageBox(NULL, TEXT("The files are NOT identical"), TEXT("File Compare"), MB_ICONSTOP);
			}
			break;

		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_CREATE:
	{

		// grey background
		CreateWindow(TEXT("STATIC"), TEXT(""),
			WS_VISIBLE | WS_CHILD, 0, 0, 400, 200, hWnd, NULL, NULL, NULL);

		CreateWindow(TEXT("STATIC"),
			TEXT("File 1:"),
			WS_VISIBLE | WS_CHILD,
			10, 10, 40, 20, hWnd, NULL, NULL, NULL);

		CreateWindow(TEXT("STATIC"),
			TEXT("File 2:"),
			WS_VISIBLE | WS_CHILD,
			10, 40, 40, 20, hWnd, NULL, NULL, NULL);

		hWnd_InFile[0] = CreateWindow(TEXT("STATIC"),
			TEXT(""),
			WS_VISIBLE | WS_CHILD, 100, 10, 250, 20, hWnd, NULL, NULL, NULL);

		hWnd_InFile[1] = CreateWindow(TEXT("STATIC"),
			TEXT(""),
			WS_VISIBLE | WS_CHILD, 100, 40, 250, 20, hWnd, NULL, NULL, NULL);

		hWnd_btnFile[0] = CreateWindow(TEXT("BUTTON"),
			TEXT("File 1"),
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			110, 100, 70, 30, hWnd, NULL, NULL, NULL);

		hWnd_btnFile[1] = CreateWindow(TEXT("BUTTON"),
			TEXT("File 2"),
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			200, 100, 70, 30, hWnd, NULL, NULL, NULL);

		hWnd_btnCompare = CreateWindow(TEXT("BUTTON"),
			TEXT("Compare"),
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			290, 100, 70, 30, hWnd, NULL, NULL, NULL);

		EnableWindow(hWnd_btnFile[1], false);		// buttons disabled, enabled in order
		EnableWindow(hWnd_btnCompare, false);

		SetWindowText(hWnd, _T("CIS 023, S2020, Homework 17, Miguel Antonio Logarta"));
	}
	break;

	case WM_DESTROY:
		// Clear the vectors
		vFile1.clear();
		vFile2.clear();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// get the file name from the common file open dialog box
bool GetFileName(int index)
{
	char szFileNameIN[MAX_PATH];
	TCHAR szFileSpec[MAX_PATH];

	// get the input file name
	OPENFILENAME ofn;
	ZeroMemory(&szFileSpec, sizeof(szFileSpec));
	ZeroMemory(&szFileNameIN, sizeof(szFileNameIN));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = LPWSTR("Any File\0*.*\0");
	ofn.lpstrFile = LPWSTR(szFileSpec);
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = LPWSTR(szFileNameIN);
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrTitle = LPWSTR("Select an input File");
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(LPOPENFILENAMEA(&ofn))) // user selected an input file
	{
		// display input file name on window
		size_t pReturnValue;
		wchar_t wcstr[MAX_LOADSTRING];
		mbstowcs_s(&pReturnValue, wcstr, MAX_LOADSTRING, szFileNameIN, MAX_LOADSTRING);
		SendMessage(hWnd_InFile[index], WM_SETTEXT, NULL, LPARAM(wcstr));

		TCHARtoString(wcstr, &strFileSpec[index]);		// store filename

		return true;
	}
	else
		return false;
}

// push the 2 files into the vector stacks
bool Push()
{
	ifstream inFile;					// Handle for the file
	char c;								// Character from file

	try
	{
		// Make sure the files will open before creating the stack

		// Load the first file
		inFile.open(strFileSpec[0]);
		if (inFile.bad())
			throw 1;

		// Load file into queue
		while (inFile.good())			// Read to end of file
		{
			inFile.get(c);				// Read a char from file
			vFile1.push_back(c);		// Append to queue
		}

		inFile.close();					// done with the input file

		inFile.clear();					// Clear flags because we're going to open another file

		// Load the second file
		inFile.open(strFileSpec[1]);
		if (inFile.bad())
			throw 1;

		// Load file into queue
		while (inFile.good())			// Read to end of file
		{
			inFile.get(c);				// Read a char from file
			vFile2.push_back(c);		// Append to queue
		}

		inFile.close();					// done with the input file

		return true;					// file reads did not fail
	}
	catch (int)
	{
		MessageBox(NULL,
			TEXT("Unable to open input file."),
			TEXT("File Open Error"),
			MB_ICONEXCLAMATION);

		return false;
	}					
}

// compare the two stacks
bool CompareFiles()
{
	// Check if the sizes are different
	if (vFile1.size() != vFile2.size())
		return false;

	// Loop through both stacks and see if all the chars match
	while (vFile1.size() > 0)
	{
		// Check if the chars match
		if (vFile1.back() != vFile2.back())
			return false;
		// Pop the stack
		vFile1.pop_back();
		vFile2.pop_back();
	}

	return true;								// didn't find a difference
}