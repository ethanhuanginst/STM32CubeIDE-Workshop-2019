#include "menuDLG.h"
#include "set_frequencyDLG.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "FRAMEWIN.h"
#include "string.h"

option_t *option_setting_hook = NULL;
WM_HWIN menu_hwin = 0;

typedef struct _menu_attributes {
    WM_HWIN hCategoryButton;
    WM_HWIN hButtons[6];
    option_t *button_options[6];
    int currentCategory;
} menu_attributes_t;

static menu_attributes_t menu_attr;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0    (GUI_ID_USER + 0x02)
#define ID_BUTTON_0    (GUI_ID_USER + 0x04)
#define ID_BUTTON_1    (GUI_ID_USER + 0x05)
#define ID_BUTTON_2    (GUI_ID_USER + 0x06)
#define ID_BUTTON_3    (GUI_ID_USER + 0x07)
#define ID_BUTTON_4    (GUI_ID_USER + 0x08)
#define ID_BUTTON_5    (GUI_ID_USER + 0x09)
#define ID_BUTTON_CATEGORY    (GUI_ID_USER + 0x0A)

#define N_MENU_BUTTONS 6


// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "menu", ID_FRAMEWIN_0, 390, -21, 110, 300, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TRACE", ID_BUTTON_CATEGORY, 0, 5, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Detector:\nMAX HOLD", ID_BUTTON_0, 0, 40, 80, 35, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "2", ID_BUTTON_1, 0, 80, 80, 35, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "3", ID_BUTTON_2, 0, 120, 80, 35, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "4", ID_BUTTON_3, 0, 160, 80, 35, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "5", ID_BUTTON_4, 0, 200, 80, 35, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "6", ID_BUTTON_5, 0, 240, 80, 35, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

void option_press( option_t *o ) {
    if( o->type == OPTION_TYPE_SELECTION ) {
        selection_option_t *so = o->data;
        for( int i = 0; i != so->nSelections; ++i ) {
            if(so->selections[i].id == so->value) {
                so->value = so->selections[(i+1)%so->nSelections].id;
                return;
            }
        }
    } else if( o->type == OPTION_TYPE_FREQUENCY ) {
        option_setting_hook = o;
        GUI_HWIN freq_dialog = Createset_frequency();
        WM_ShowWindow( freq_dialog );
        WM_SetFocus( freq_dialog );
    } else if( o->type == OPTION_TYPE_ACTION ) {
        void (*o_callback)(void) = (void (*)(void)) o->data;
        o_callback();
    } else {
        // Nothing, unknown option type
    }
}

void dtoa_nobug(char *dest, double n) {
    const int MAX_LEN = 16;

    int n_lhs = (int)n;

    // Subtract everything left of decimal point
    n -= (double)(int)n;
    n *= 1e4;

    int out = (int)n;
    out = out%10 >=5 ? out + 10 : out;

    if(out > 1e4) {
        ++n_lhs;
        n = 0;
    }

    // Print everything before the decimal point
    snprintf(dest, MAX_LEN, "%i", n_lhs);

    if( n >=1 ) {
        // Print everything after the decimal point (with RHS zero padding)
        snprintf(dest + strlen(dest), MAX_LEN-strlen(dest), ".%03i", out/10);

        // Erase trailing zeros
        int i = strlen(dest);
        for(; i != 0; --i) {
            if( dest[i-1] != '0' )
                break;
        }
        dest[i] = 0;
    }
}

void print_scientific( double f, char *units, char *dest ) {
    char *si_prefixes = "yzafpnum kMGTPEZY";
    int prefix_index = 0;

    for(; prefix_index != 17; ++prefix_index) {
        double lower = pow(1e3, prefix_index - 8);
        double upper = pow(1e3, prefix_index - 7);
        if( lower < f && f < upper ) {
            f /= lower;
            break;
        }
    }

    char value_str[16];
    dtoa_nobug(value_str, f);

    // Ignore prefix if there isn't one
    if( si_prefixes[prefix_index] == ' ' ) {
        snprintf(dest, 64, "%s %s", value_str, units);
    } else {
        snprintf(dest, 64, "%s %c%s", value_str, si_prefixes[prefix_index], units);
    }
}

void option_to_string( option_t *o, char *buf ) {
    char value[64] = "";
    if( o->type == OPTION_TYPE_SELECTION ) {
        selection_option_t *so = o->data;
        for( int i = 0; i != so->nSelections; ++i ) {
            if(so->selections[i].id == so->value) {
                strcpy(value, so->selections[i].name);
            }
        }
        snprintf(buf, 64, "%s:\n%s", o->name, value);
    } else if( o->type == OPTION_TYPE_FREQUENCY ) {
        double f = *(double*)o->data;
        print_scientific(f, "Hz", value);
        snprintf(buf, 64, "%s:\n%s", o->name, value);
    } else if( o->type == OPTION_TYPE_ACTION ) {
        snprintf(buf, 64, "DO\n%s", o->name);
    } else {
        // Nothing, unknown option type
    }
}

void refreshMenu(WM_HWIN hCategoryButton, WM_HWIN *hButtons, option_t **button_options, int currentCategory) {
    //DEBUG INSTRUMENT
    //asm volatile ("cpsid i" : : : "memory");

    options_refresh();

    category_t *this_category = &get_option_menu()->categories[currentCategory];

    BUTTON_SetText( hCategoryButton, this_category->name );

    // Clear all buttons first
    for(int i = 0; i != N_MENU_BUTTONS; ++i) {
        BUTTON_SetText(hButtons[i], "");
        button_options[i] = 0;
    }

    int currentButton = 0;
    char buf[64];
    for(int i = 0; i != this_category->nOptions; ++i) {
        option_t *this_option = &this_category->options[i];
        if(this_option->enable) {
            option_to_string(this_option, buf);
            BUTTON_SetText(hButtons[currentButton], buf);
            button_options[currentButton] = this_option;
            ++currentButton;
        }
    }

    //DEBUG INSTRUMENT
    //asm volatile ("cpsie i" : : : "memory");
}

void globalMenuRefresh() {
    refreshMenu(menu_attr.hCategoryButton,
            menu_attr.hButtons,
            menu_attr.button_options,
            menu_attr.currentCategory);
}

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
    WM_HWIN hItem;
    int     NCode;
    int     Id;

    // USER START (Optionally insert additional variables)
    // USER END
    //
    switch (pMsg->MsgId) {
        case WM_INIT_DIALOG:
            hItem = WM_GetDialogItem(pMsg->hWin, ID_FRAMEWIN_0);


            menu_attr.hCategoryButton = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CATEGORY);
            BUTTON_SetFont(menu_attr.hCategoryButton, GUI_FONT_20B_ASCII);

            menu_attr.hButtons[0] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
            menu_attr.hButtons[1] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
            menu_attr.hButtons[2] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
            menu_attr.hButtons[3] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
            menu_attr.hButtons[4] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
            menu_attr.hButtons[5] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5);

            for( int i = 0; i !=  N_MENU_BUTTONS; ++i ) {
                BUTTON_SetFont(menu_attr.hButtons[i], GUI_FONT_13B_ASCII);
            }

            refreshMenu(menu_attr.hCategoryButton,
                        menu_attr.hButtons,
                        menu_attr.button_options,
                        menu_attr.currentCategory);

            break;
        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
            printf("WM_NOTIFY: ID:%i, NCODE:%i\n", Id, NCode);
            switch(Id) {
                case ID_BUTTON_CATEGORY: // Notifications sent by 'Category'
                    if( NCode == WM_NOTIFICATION_RELEASED ) {
                        ++menu_attr.currentCategory;
                        menu_attr.currentCategory %= get_option_menu()->nCategories;
                    }
                    break;
                case ID_BUTTON_0: // Notifications sent by '1'
                    if( NCode == WM_NOTIFICATION_RELEASED ) {
                        if( menu_attr.button_options[0] ) {
                            option_press( menu_attr.button_options[0] );
                        }
                    }
                    break;
                case ID_BUTTON_1: // Notifications sent by '2'
                    if( NCode == WM_NOTIFICATION_RELEASED ) {
                        if( menu_attr.button_options[1] ) {
                            option_press( menu_attr.button_options[1] );
                        }
                    }
                    break;
                case ID_BUTTON_2: // Notifications sent by '3'
                    if( NCode == WM_NOTIFICATION_RELEASED ) {
                        if( menu_attr.button_options[2] ) {
                            option_press( menu_attr.button_options[2] );
                        }
                    }
                    break;
                case ID_BUTTON_3: // Notifications sent by '4'
                    if( NCode == WM_NOTIFICATION_RELEASED ) {
                        if( menu_attr.button_options[3] ) {
                            option_press( menu_attr.button_options[3] );
                        }
                    }
                    break;
                case ID_BUTTON_4: // Notifications sent by '5'
                    if( NCode == WM_NOTIFICATION_RELEASED ) {
                        if( menu_attr.button_options[4] ) {
                            option_press( menu_attr.button_options[4] );
                        }
                    }
                    break;
                case ID_BUTTON_5: // Notifications sent by '6'
                    if( NCode == WM_NOTIFICATION_RELEASED ) {
                        if( menu_attr.button_options[5] ) {
                            option_press( menu_attr.button_options[5] );
                        }
                    }
                    break;
                    // USER START (Optionally insert additional code for further Ids)
                    // USER END
            }

            refreshMenu(menu_attr.hCategoryButton,
                        menu_attr.hButtons,
                        menu_attr.button_options,
                        menu_attr.currentCategory);

            break;
            // USER START (Optionally insert additional message handling)
            // USER END
        default:
            WM_DefaultProc(pMsg);
            break;
    }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       Createset_frequency
*/
WM_HWIN Createmenu(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  menu_hwin = hWin;
  menu_attr.currentCategory = 0;
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
