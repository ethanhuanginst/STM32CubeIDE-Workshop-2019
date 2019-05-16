#include "set_frequencyDLG.h"
#include <stdlib.h>
#include <stdio.h>
#include "menuDLG.h"

#define ID_FRAMEWIN_0    (GUI_ID_USER + 0x02)
#define ID_EDIT_0    (GUI_ID_USER + 0x03)
#define ID_BUTTON_0    (GUI_ID_USER + 0x04)
#define ID_BUTTON_1    (GUI_ID_USER + 0x05)
#define ID_BUTTON_2    (GUI_ID_USER + 0x06)
#define ID_BUTTON_3    (GUI_ID_USER + 0x07)
#define ID_BUTTON_4    (GUI_ID_USER + 0x08)
#define ID_BUTTON_5    (GUI_ID_USER + 0x09)
#define ID_BUTTON_6    (GUI_ID_USER + 0x0A)
#define ID_BUTTON_7    (GUI_ID_USER + 0x0B)
#define ID_BUTTON_8    (GUI_ID_USER + 0x0C)
#define ID_BUTTON_9    (GUI_ID_USER + 0x0D)
#define ID_BUTTON_10    (GUI_ID_USER + 0x0E)
#define ID_BUTTON_11    (GUI_ID_USER + 0x0F)
#define ID_BUTTON_12    (GUI_ID_USER + 0x10)
#define ID_BUTTON_13    (GUI_ID_USER + 0x11)
#define ID_BUTTON_14    (GUI_ID_USER + 0x12)
#define ID_BUTTON_15    (GUI_ID_USER + 0x13)
#define ID_BUTTON_16    (GUI_ID_USER + 0x15)

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "set_frequency", ID_FRAMEWIN_0, 40, 0, 320, 272, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_0, 18, 10, 267, 40, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "0", ID_BUTTON_0, 230, 60, 60, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "1", ID_BUTTON_1, 20, 160, 60, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "2", ID_BUTTON_2, 90, 160, 60, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "3", ID_BUTTON_3, 160, 160, 60, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "4", ID_BUTTON_4, 20, 110, 60, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "5", ID_BUTTON_5, 90, 110, 60, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "6", ID_BUTTON_6, 160, 110, 60, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "7", ID_BUTTON_7, 20, 60, 60, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "8", ID_BUTTON_8, 90, 60, 60, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "9", ID_BUTTON_9, 160, 60, 60, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, ".", ID_BUTTON_10, 230, 110, 60, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "GHz", ID_BUTTON_11, 20, 210, 50, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "MHz", ID_BUTTON_12, 80, 210, 50, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "KHz", ID_BUTTON_13, 140, 210, 50, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Hz", ID_BUTTON_14, 200, 210, 50, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "X", ID_BUTTON_15, 260, 210, 30, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "C", ID_BUTTON_16, 230, 160, 60, 40, 0, 0x0, 0 },
};

void PressNumber(WM_HWIN hWin, int n ) {
    WM_HWIN hEdit = WM_GetDialogItem(hWin, ID_EDIT_0);

    char buf[32];
    EDIT_GetText( hEdit, buf, 32 );

    for( int i = 0; i != 31; ++i ) {
        if( buf[i] == 0 ) {
            buf[i] = '0'+n;
            buf[i+1] = 0;
            EDIT_SetText( hEdit, buf );
            return;
        }
    }
}

void PressDot(WM_HWIN hWin) {
    WM_HWIN hEdit = WM_GetDialogItem(hWin, ID_EDIT_0);

    char buf[32];
    EDIT_GetText( hEdit, buf, 32 );

    for( int i = 0; i != 31; ++i ) {
        // Don't do anything if there's already a decimal point
        if( buf[i] == '.' ) {
            return;
        }

        if( buf[i] == 0 ) {
            buf[i] = '.';
            buf[i+1] = 0;
            EDIT_SetText( hEdit, buf );
            return;
        }
    }
}

void CloseThis(WM_HWIN hWin) {
    WM_HideWindow(hWin);
    option_setting_hook = NULL;
    // TODO: Destroy this!
}

void SetValueAndCloseThis(WM_HWIN hWin, double multiplier) {
    // Not setting an option or something wrong? Not good!
    if(option_setting_hook == NULL || option_setting_hook->type != OPTION_TYPE_FREQUENCY) {
        printf("FAILED TO SET OPTION, NULL OR INVALID OPTION HOOK TYPE\n");
    } else {
        WM_HWIN editItem;
        editItem = WM_GetDialogItem(hWin, ID_EDIT_0);

        char myValue_str[32];
        EDIT_GetText(editItem, myValue_str, 32);
        double myValue = atof(myValue_str) * multiplier;

        *(double*)option_setting_hook->data = myValue;

        globalMenuRefresh();
    }

    CloseThis(hWin);
}

static void _cbDialog(WM_MESSAGE * pMsg) {

    WM_HWIN hItem;
    int     NCode;
    int     Id;

    switch (pMsg->MsgId) {
        case WM_INIT_DIALOG:
            hItem = WM_GetDialogItem(pMsg->hWin, ID_FRAMEWIN_0);

            // Initialization of 'Edit'
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
            EDIT_SetText(hItem, "");
            EDIT_SetFont(hItem, GUI_FONT_32B_ASCII);
            EDIT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);

            // Button fonts
            for( int i = ID_BUTTON_0; i <= ID_BUTTON_16; ++i ) {
                hItem = WM_GetDialogItem(pMsg->hWin, i);
                BUTTON_SetFont(hItem, GUI_FONT_24B_ASCII);
            }

            break;
        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;

            // Button presses
            if( ID_BUTTON_0 <= Id && Id <= ID_BUTTON_9 &&
                NCode == WM_NOTIFICATION_RELEASED) {
                PressNumber(pMsg->hWin, Id-ID_BUTTON_0);
            }

            switch(Id) {
                case ID_EDIT_0: // Notifications sent by 'Edit'
                    break;
                case ID_BUTTON_10: // Notifications sent by '.'
                    if(NCode ==  WM_NOTIFICATION_RELEASED)
                        PressDot(pMsg->hWin);
                    break;
                case ID_BUTTON_11: // Notifications sent by 'GHz'
                    if(NCode ==  WM_NOTIFICATION_RELEASED)
                        SetValueAndCloseThis(pMsg->hWin, 1e9);
                    break;
                case ID_BUTTON_12: // Notifications sent by 'MHz'
                    if(NCode ==  WM_NOTIFICATION_RELEASED)
                        SetValueAndCloseThis(pMsg->hWin, 1e6);
                    break;
                case ID_BUTTON_13: // Notifications sent by 'kHz'
                    if(NCode ==  WM_NOTIFICATION_RELEASED)
                        SetValueAndCloseThis(pMsg->hWin, 1e3);
                    break;
                case ID_BUTTON_14: // Notifications sent by 'Hz'
                    if(NCode ==  WM_NOTIFICATION_RELEASED)
                        SetValueAndCloseThis(pMsg->hWin, 1);
                    break;
                case ID_BUTTON_15: // Notifications sent by 'X'
                    if(NCode ==  WM_NOTIFICATION_RELEASED)
                        CloseThis(pMsg->hWin);
                    break;
                case ID_BUTTON_16: // Notifications sent by 'C'
                    if(NCode ==  WM_NOTIFICATION_RELEASED) {
                        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
                        EDIT_SetText(hItem, "");
                    }
                    break;
            }
            break;
        default:
            WM_DefaultProc(pMsg);
            break;
    }
}

WM_HWIN Createset_frequency(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}
