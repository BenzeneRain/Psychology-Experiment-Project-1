// experiment.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "experiment.h"
#include "ConfWnd.h"
#include "Screen.h"
#include "Trial.h"
#include "TestObject.h"
#include "CylinderObject.h"
#include "PostExperimentScene.h"

#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <fstream>

using namespace std;

//#define MAX_LOADSTRING 100
//
//// Global Variables:
//HINSTANCE hInst;								// current instance
//TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
//TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
//ATOM				MyRegisterClass(HINSTANCE hInstance);
//BOOL				InitInstance(HINSTANCE, int);
//LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
//
//int APIENTRY _tWinMain(HINSTANCE hInstance,
//                     HINSTANCE hPrevInstance,
//                     LPTSTR    lpCmdLine,
//                     int       nCmdShow)
//{
//UNREFERENCED_PARAMETER(hPrevInstance);
//UNREFERENCED_PARAMETER(lpCmdLine);


//	// TODO: Place code here.
//MSG msg;
//HACCEL hAccelTable;

//// Initialize global strings
//LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//LoadString(hInstance, IDC_EXPERIMENT, szWindowClass, MAX_LOADSTRING);
//MyRegisterClass(hInstance);

//ConfWnd confWnd;
//confWnd.displayConfWnd(hInstance);

//// Perform application initialization:
//if (!InitInstance (hInstance, nCmdShow))
//{
//	return FALSE;
//}

//hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EXPERIMENT));

//// Main message loop:
//while (GetMessage(&msg, NULL, 0, 0))
//{
//	if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
//	{
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//	}
//}

//return (int) msg.wParam;
//}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
//ATOM MyRegisterClass(HINSTANCE hInstance)
//{
//	WNDCLASSEX wcex;
//
//	wcex.cbSize = sizeof(WNDCLASSEX);
//
//	wcex.style			= CS_HREDRAW | CS_VREDRAW;
//	wcex.lpfnWndProc	= WndProc;
//	wcex.cbClsExtra		= 0;
//	wcex.cbWndExtra		= 0;
//	wcex.hInstance		= hInstance;
//	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EXPERIMENT));
//	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
//	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
//	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_EXPERIMENT);
//	wcex.lpszClassName	= szWindowClass;
//	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
//
//	return RegisterClassEx(&wcex);
//}

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
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//   HWND hWnd;
//
//   hInst = hInstance; // Store instance handle in our global variable
//
//   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
//
//   if (!hWnd)
//   {
//      return FALSE;
//   }
//
//   ShowWindow(hWnd, nCmdShow);
//   UpdateWindow(hWnd);
//
//   return TRUE;
//}
//
////
////  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
////
////  PURPOSE:  Processes messages for the main window.
////
////  WM_COMMAND	- process the application menu
////  WM_PAINT	- Paint the main window
////  WM_DESTROY	- post a quit message and return
////
////
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	int wmId, wmEvent;
//	PAINTSTRUCT ps;
//	HDC hdc;
//
//	switch (message)
//	{
//	case WM_COMMAND:
//		wmId    = LOWORD(wParam);
//		wmEvent = HIWORD(wParam);
//		// Parse the menu selections:
//		switch (wmId)
//		{
//		case IDM_ABOUT:
//			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
//			break;
//		case IDM_EXIT:
//			DestroyWindow(hWnd);
//			break;
//		default:
//			return DefWindowProc(hWnd, message, wParam, lParam);
//		}
//		break;
//	case WM_PAINT:
//		hdc = BeginPaint(hWnd, &ps);
//		// TODO: Add any drawing code here...
//		EndPaint(hWnd, &ps);
//		break;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	default:
//		return DefWindowProc(hWnd, message, wParam, lParam);
//	}
//	return 0;
//}
//
//// Message handler for about box.
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	UNREFERENCED_PARAMETER(lParam);
//	switch (message)
//	{
//	case WM_INITDIALOG:
//		return (INT_PTR)TRUE;
//
//	case WM_COMMAND:
//		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//		{
//			EndDialog(hDlg, LOWORD(wParam));
//			return (INT_PTR)TRUE;
//		}
//		break;
//	}
//	return (INT_PTR)FALSE;
//}

// This option is for debugging. Set it to 1 in order to enter debug mode
int const Experiment::debug = 0;

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
        LPSTR lpCmdLine, int nShowCmd)
{
    Experiment *pExperi = Experiment::getInstance(NULL);

    pExperi->startProgram();

    return 0;
}

Experiment::Experiment(HINSTANCE hInstance)
{
    this->hInst = hInstance;
    this->currSecNo = 0;
};

Experiment::~Experiment(void)
{

}

BOOL Experiment::startProgram()
{
    BOOL ret;

    // Open the Configuration Window for settings
    ConfWnd *pConfWnd = ConfWnd::getInstance();
    ret = pConfWnd->displayConfWnd(this->hInst);

    // If Configuration failed or Exit button is pressed, 
    // directly exit the program
    if(ret == FALSE)
    {
        return FALSE; 
    }

    // Initialize the system
    this->initSystem();


    // Initialize the glut
    for(vector<Screen *>::iterator it = this->screens.begin();
            it != this->screens.end(); it ++)
    {
        Screen *pScreen = (Screen *) *it;
        if(!Experiment::debug)
        {
            pScreen->initGlut(this->devMode,
                    GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH,
                    string("Experiment"));
        }
        else
        {
            pScreen->initGlut(this->devMode,
                    GLUT_RGB | GLUT_SINGLE,
                    string("Experiment"));
        }
    }  
    // Initialize the output file if in experiment mode
    if(this->experiMode == 0)
    {
        ret = this->initOutputFile();
        if(ret == FALSE)
            return FALSE;
    }

    // Write fixed configurations to the output file
    this->recordConfigurations();

    // main body
    this->proceedExperiment();

    // dispose

    if(this->experiMode == 0)
    {
        this->closeOutputFile();
    }
    return TRUE;
}

BOOL Experiment::initOutputFile()
{
    try
    {
        this->hFileOut.open(this->outFilename.c_str(), fstream::out);
        return TRUE;
    }
    catch(fstream::failure err)
    {
        MessageBox(NULL, "Fail to open the output file. The program will exit silently.", "Error", 0);
        return FALSE;
    }
}

BOOL Experiment::closeOutputFile()
{
    if(this->hFileOut.is_open())
    {
        this->hFileOut.flush();
        this->hFileOut.close();
    }

    return TRUE;
}

BOOL Experiment::initSystem()
{
    // get the configurations
    ConfWnd *pConfWnd = ConfWnd::getInstance();

    this->subjectID = pConfWnd->subjectID;
    this->maxSecNo = pConfWnd->maxSecNo;
    this->experiMode = pConfWnd->experiMode;
    this->outFilename = pConfWnd->outFilename;
    this->trialsInOneSec = pConfWnd->trialsInOneSec;
    this->devMode = pConfWnd->devMode;
    this->strDate = pConfWnd->strDate;
    this->strTime = pConfWnd->strTime;

    // Register Test Objects
    this->stubObjects.push_back(new CylinderObject());

    // Initialize random seed
    srand((unsigned int)time(NULL));

    // Initialize screen class
    static auto_ptr<Screen> apScreen;
    apScreen.reset(new Screen);

    Screen * pScreen = apScreen.get();
    this->screens.push_back(pScreen);

    return TRUE;
}

BOOL Experiment::proceedExperiment()
{
    Trial *pTrial = Trial::getInstance();

    // TODO: We can add Pre-Experiment Scene here
    // if needed

    //proceed trials
    while(this->currSecNo < this->maxSecNo)
    {
        pTrial->startTrial();
    }

    //show post-experiment scene
    static auto_ptr<Scene> apScene;
    Scene *pScene;

    apScene.reset(new PostExperimentScene());
    pScene = apScene.get();        

    pScene->startScene();

    return TRUE;
}

BOOL Experiment::isNewSection()
{
   Trial *pTrial = Trial::getInstance();
   if(pTrial->trialID == 0)
   {
       return TRUE;
   }
   else
   {
       return FALSE;
   }

}

BOOL Experiment::recordConfigurations()
{
    BOOL ret;
    
    ostringstream ossConf;

    ossConf << "Subject ID: " << this->subjectID << endl;
    ossConf << "Max section number: " << this->maxSecNo << endl;
    ossConf << "Number of trials in one section: " << this->trialsInOneSec << endl;

    ossConf << "Screen resolution: " << this->devMode.dmPelsWidth
            << " X " << this->devMode.dmPelsHeight << endl;

    // FIX: this is actually the max refresh rate, but sometimes it cannot be reached
    // due to heavy load, such as open the MultiSample option
    ossConf << "Screen refresh rate: " << this->devMode.dmDisplayFrequency << "Hz" << endl;
    ossConf << "Bit per pixel of the Screen: " << this->devMode.dmBitsPerPel << endl;

    ossConf << "Experiment start time: " << this->strDate << " " << this->strTime << endl;

    ossConf << "Test object information: " << endl;
    // Output Test Object List
    for(vector<TestObject *>::iterator it = this->stubObjects.begin();
        it != this->stubObjects.end(); it ++)
    {
        string strTestObjDesc = ((TestObject *)(*it))->genObjDesc();
        ossConf << strTestObjDesc << endl;
    }   


    ret = this->writeOutputs(ossConf.str());

    return ret;
}

BOOL Experiment::writeOutputs(string& strOutputs)
{
    this->hFileOut << strOutputs;

    return TRUE;
}

// Below are singalton implementations
auto_ptr<Experiment> Experiment::m_pInstance;

Experiment *Experiment::getInstance(HINSTANCE hInstance)
{
    if(m_pInstance.get() == 0)
    {
        m_pInstance.reset(new Experiment(hInstance));
    }

    return m_pInstance.get();
}
