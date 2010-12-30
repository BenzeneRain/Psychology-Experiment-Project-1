#include "stdafx.h"
#include "resource.h"
#include "ConfWnd.h"

auto_ptr<ConfWnd> ConfWnd::m_pInstance;

ConfWnd::ConfWnd(void)
{

}

ConfWnd::~ConfWnd(void)
{

}

ConfWnd* ConfWnd::getInstance()
{
    if(m_pInstance.get() == 0)
    {
        m_pInstance.reset(new ConfWnd);
    }

    return m_pInstance.get();
}

INT_PTR ConfWnd::displayConfWnd(HINSTANCE ghInstance)
{
    INT_PTR ret;

    ret = DialogBox(ghInstance, MAKEINTRESOURCE(IDD_CONFWND), GetDesktopWindow(), confWndProc);

    return ret;
}

INT_PTR CALLBACK ConfWnd::confWndProc(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
    switch(message)
    {
        case WM_INITDIALOG:
            {
                return TRUE;
            }
        case WM_COMMAND:
            {
                if(LOWORD(wParam) == IDOK)
                {
                    EndDialog(hDlg, TRUE);
                }
                break;
            }
        case WM_CLOSE:
            {
                EndDialog(hDlg, FALSE);
                break;
            }
    }

    return FALSE;
}
