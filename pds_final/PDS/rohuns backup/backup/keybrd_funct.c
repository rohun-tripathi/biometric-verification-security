#include<stdio.h>
#include<stdlib.h>
#include<pwrap2.h>

//#include "funct2.c"
#include"features.h"


pw_widget toplevel;


Bool cb_entry_num(pw_widget w, pw_event e)
{
        pw_alchemy_command(toplevel, PW_WM_SET_KEYBOARD_LAYOUT, "123");
        pw_alchemy_command(toplevel, PW_WM_SHOW_KEYBOARD, "123");
        pw_alchemy_command(toplevel, PW_WM_SHOW_INPUT, "123");
        return False;
}

Bool cb_entry_alpha(pw_widget w, pw_event e)
{
        pw_alchemy_command(toplevel, PW_WM_SET_KEYBOARD_LAYOUT, "qwerty");
        pw_alchemy_command(toplevel, PW_WM_SHOW_KEYBOARD, "qwerty");
        pw_alchemy_command(toplevel, PW_WM_SHOW_INPUT, "");
        return False;
}

