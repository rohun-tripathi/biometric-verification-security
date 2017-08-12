/*! 	\file pwrap2.h
*/

/*! \mainpage Pwrap2
 *
 * \section intro_sec Introduction
 *
 * Pwrap2 is a new widget library.
 *
 */

#ifndef   __PW__
#define   __PW__

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <pimglib.h>
#include <sys/time.h>
#include <time.h>
#include <acctrl.h>
#include <math.h>
#ifdef __cplusplus
extern "C" {
#endif


// Text listbox selection types.
/*!\def PW_TLISTBOX_SINGLE_SELECTION
 * \brief This value is used by pw_tlistbox_set_selection_type() function to allow only one selection from a
 * text listbox.
 */
#define PW_TLISTBOX_SINGLE_SELECTION	0
/*!\def PW_TLISTBOX_MULTIPLE_SELECTION
 * \brief This value is used by pw_tlistbox_set_selection_type() function to allow the text listbox to select multiple
 * items.
 */
#define PW_TLISTBOX_MULTIPLE_SELECTION	1

// Orientation type, used by pw_set_display_orientation() function.
/*!\def PW_ORIENT_TOP
 * \brief This value is used by the pw_set_display_orientation() funtion to set the display orietation to top portrait mode.
 */
#define PW_ORIENT_TOP				0
/*!\def PW_ORIENT_BOTTOM
 * \brief This value is used by the pw_set_display_orientation() funtion to set the display orietation to bottom portrait mode.
 */
#define PW_ORIENT_BOTTOM				1
/*!\def PW_ORIENT_LEFT
 * \brief This value is used by the pw_set_display_orientation() funtion to set the display orietation to left landscape mode.
 */
#define PW_ORIENT_LEFT				2
/*!\def PW_ORIENT_RIGHT
 * \brief This value is used by the pw_set_display_orientation() function to set the display orietation to right landscape mode.
 */
#define PW_ORIENT_RIGHT				3

/*! \def PW_SUPPORT_GENERIC
 * \brief To be sent to the pw_init() function for writing simple applications.
 */
#define PW_SUPPORT_GENERIC		0x00000000
/*! \def PW_SUPPORT_ALCHEMY
 * \brief To be sent to the pw_init() function for writing applications using the Alchemy framework.
 */
#define PW_SUPPORT_ALCHEMY		0x00000001
/*!\cond */
#define PW_SUPPORT_ROCKER		0x00000002
#define PW_SUPPORT_ACCELEROMETER	0x00000004
/*!\endcond */
/*! \def PW_SUPPORT_DRAFTMODE
 * \brief To be sent to the pw_init() function for displaying a grid in the widgets. The grid will be
 * useful in placing widgets. The "grid_mode" program in the examples, shows the usage of it.
 */
#define PW_SUPPORT_DRAFTMODE		0x00000008

// Slider text print orientation.
/*!\def PW_SLIDER_TOP
 * \brief This value is used by the pw_hscrollbar_set_sliderpos() function to position the text on top of the horizontal slider.
 */
#define PW_SLIDER_TOP				0
/*!\def PW_SLIDER_BOTTOM
 * \brief This value is used by the pw_hscrollbar_set_sliderpos() function to position the text at bottom of the horizontal slider.
 */
#define PW_SLIDER_BOTTOM				1
/*!\def PW_SLIDER_LEFT
 * \brief This value is used by the pw_vscrollbar_set_sliderpos() function to position the text on left of the vertical slider.
 */
#define PW_SLIDER_LEFT				2
/*!\def PW_SLIDER_RIGHT
 * \brief This value is used by the pw_vscrollbar_set_sliderpos() function to position the text on right of the vertical slider.
 */
#define PW_SLIDER_RIGHT				3

/*! \def PW_NORMAL_FONT
 * \brief Used by pw_set_font() function to set normal font.
 */
#define 	PW_NORMAL_FONT   "Sans 9"
/*! \def PW_BOLD_FONT
 * \brief Used by pw_set_font() function to set bold font.
 */
#define 	PW_BOLD_FONT   "Sans Bold 9"
/*! \def PW_SMALL_FONT
 * \brief Used by pw_set_font() function to set small font.
 */
#define 	PW_SMALL_FONT   "Sans 8"
/*! \def PW_TINY_FONT
 * \brief Used by pw_set_font() function to set tiny font.
 */
#define 	PW_TINY_FONT   "Sans 7"
/*! \def PW_LARGE_FONT
 * \brief Used by pw_set_font() function to set large font.
 */
#define 	PW_LARGE_FONT   "Sans 12"
/*! \def PW_FIXED_FONT
 * \brief Used by pw_set_font() function to set fixed font.
 */
#define 	PW_FIXED_FONT   "Sans 10"


/*!\cond */
	//alchemy
#define 	PW_WM_LAUNCH_APP   AC_CMD_LAUNCH_APP
#define 	PW_WM_SET_TITLE_FONT   AC_CMD_SET_TITLE_FONT
#define 	PW_WM_SET_TITLE_JUSTIFY   AC_CMD_SET_TITLE_JUSTIFY
#define 	PW_WM_SET_STATUS_MSG   AC_CMD_SET_STATUS_MSG
#define 	PW_WM_SET_STATUS_FONT   AC_CMD_SET_STATUS_FONT
#define 	PW_WM_SET_STATUS_JUSTIFY   AC_CMD_SET_STATUS_JUSTIFY
#define 	PW_WM_SET_PAGE_NUM   AC_CMD_SET_PAGE_NUM
#define 	PW_WM_SET_PAGE_NUM_FONT   AC_CMD_SET_PAGE_NUM_FONT
#define 	PW_WM_SET_PAGE_NUM_JUSTIFY   AC_CMD_SET_PAGE_NUM_JUSTIFY
#define 	PW_WM_SET_KEYBOARD_LAYOUT   AC_CMD_SET_KEYBOARD_LAYOUT
#define 	PW_WM_SHOW_INPUT   AC_CMD_SHOW_INPUT
#define 	PW_WM_HIDE_INPUT   AC_CMD_HIDE_INPUT
#define 	PW_WM_SHOW_PAGE_NAV   AC_CMD_SHOW_PAGE_NAV
#define 	PW_WM_HIDE_PAGE_NAV   AC_CMD_HIDE_PAGE_NAV
#define 	PW_WM_ENABLE_PAGE_NEXT   AC_CMD_ENABLE_PAGE_NEXT
#define 	PW_WM_DISABLE_PAGE_NEXT   AC_CMD_DISABLE_PAGE_NEXT
#define 	PW_WM_ENABLE_PAGE_PREV   AC_CMD_ENABLE_PAGE_PREV
#define 	PW_WM_DISABLE_PAGE_PREV   AC_CMD_DISABLE_PAGE_PREV
#define 	PW_WM_SHOW_RETRACTBAR   AC_CMD_SHOW_RETRACTBAR
#define 	PW_WM_HIDE_RETRACTBAR   AC_CMD_HIDE_RETRACTBAR
#define 	PW_WM_SHOW_KEYBOARD   AC_CMD_SHOW_KEYBOARD
#define 	PW_WM_HIDE_KEYBOARD   AC_CMD_HIDE_KEYBOARD
#define 	PW_WM_SHOW_STATUS   AC_CMD_SHOW_STATUS
#define 	PW_WM_HIDE_STATUS   AC_CMD_HIDE_STATUS
#define 	PW_WM_SWAP_CLOSE_UP   AC_CMD_SWAP_CLOSE_UP
#define 	PW_WM_SHOW_CLOSE   AC_CMD_SHOW_CLOSE
#define 	PW_WM_SHOW_UP   AC_CMD_SHOW_UP
#define 	PW_WM_SHOW_TICK   AC_CMD_SHOW_TICK

/* Pimglib image types */

#define PW_IMAGE_FORMAT_JPEG	FORMAT_JPEG
#define PW_IMAGE_FORMAT_JPG	FORMAT_JPG
#define PW_IMAGE_FORMAT_GIF	FORMAT_GIF
#define PW_IMAGE_FORMAT_BMP	FORMAT_BMP
#define PW_IMAGE_FORMAT_PNG	FORMAT_PNG
#define PW_IMAGE_FORMAT_XPM	FORMAT_XPM
#define PW_IMAGE_FORMAT_PNM	FORMAT_PNM
#define PW_IMAGE_FORMAT_PGM	FORMAT_PGM

	typedef enum _widget_type {
		PW_TOPLEVEL=1,
		PW_FRAME,
		PW_LABEL,
		PW_BUTTON,
		PW_ENTRY,
		PW_TEXTBOX,
		PW_VSCROLLBAR,
		PW_HSCROLLBAR,
		PW_CHECKBUTTON,
		PW_RADIOBUTTON,
		PW_WLISTBOX,
		PW_TLISTBOX,
		PW_COMBOBOX,	
		PW_PROGRESSBAR,
		PW_SKETCH,
		PW_TABS,
		PW_MESSAGEBOX,
		PW_SPINNER,
		PW_HSLIDER,
		PW_VSLIDER,
		PW_CALENDAR,
		PW_WIDGET_END
	} widget_type;

	typedef enum _widget_attr {
		PW_CONFIG_END=-1,
		PW_NA=-1,
		PW_TITLE=1,
		PW_TEXT,
		PW_FONT,
		PW_JUSTIFY,
		PW_WRAP,
		PW_BORDER,
		PW_BORDER_COLOR,
		PW_TAG,
		PW_FGCOLOR,
		PW_BGCOLOR,
		PW_BGTRANSPARENT,
		PW_BGIMAGE_FILE,
		PW_POSITION,
		PW_SIZE,
		PW_NORMAL_IMAGE,
		PW_ACTIVE_IMAGE,
		PW_DISABLED_IMAGE,
		PW_STATE,
		PW_CALLBACK,
		PW_CALLBACK_PARAM,
		PW_IMAGE,
		PW_COMBOBOX_TYPE,
		PW_TAB_BGCOLOR,
		PW_ATTR_END,
	} widget_attr;

	typedef enum _event_callback {
		PW_BUTTON_PRESS_EVENT,
		PW_BUTTON_RELEASE_EVENT,
		PW_MOTION_EVENT,
		PW_KEY_PRESS_EVENT,
		PW_KEY_RELEASE_EVENT,
		PW_CONFIGURE_EVENT,
		PW_CLIENT_MESSAGE_EVENT,
		PW_VISIBILITY_EVENT,
		PW_WINDOW_DELETE_EVENT,
		PW_RETRACT_BAR_REL_EVENT,
		PW_UP_RELEASE_EVENT,
		PW_TICK_RELEASE_EVENT,
		PW_NEXT_PAGE_EVENT,
		PW_NEXT_PAGE_RELEASE_EVENT,
		PW_PREV_PAGE_EVENT,
		PW_PREV_PAGE_RELEASE_EVENT,
		PW_FOCUS_IN_EVENT,
		PW_FOCUS_OUT_EVENT,
		PW_EVENT_END
	} event_callback;

	typedef enum _widget_state {
		PW_ENABLED,
		PW_DISABLED
	} widget_state;
	typedef enum _widget_image {
		PW_NORMAL_IMAGE_FILE,
		PW_ACTIVE_IMAGE_FILE,
		PW_DISABLED_IMAGE_FILE
	} widget_image;
	typedef enum _widget_border {
		PW_NO_BORDER, 
		PW_RECTANGLE_BORDER,
		PW_BEVELLED_BORDER,
		PW_ELLIPTIC_BORDER
	} widget_border;
	typedef enum _text_justify{
		PW_JUSTIFY_LEFT,
		PW_JUSTIFY_CENTER,
		PW_JUSTIFY_RIGHT
	} text_justify;

	typedef enum _text_wrap{
		PW_WORD_WRAP,
		PW_CHAR_WRAP,
		PW_WORD_CHAR_WRAP,
		PW_NO_WRAP
	} text_wrap;
	typedef enum _vscrollbar_states 
	{
		VSCROLL_NONE, VSCROLL_UP, VSCROLL_DOWN, VSCROLL_SLIDER, VSCROLL_TROUGH
	} vscrollbar_states;
	typedef enum _txtlb_types {
		TXTLB_SINGLE_SELECN,
		TXTLB_MULTI_SELECN
	} txtlb_types;

	typedef enum {
		MSGTYPE_YESNO, 
		MSGTYPE_YESNOCANCEL, 
		MSGTYPE_OK, 
		MSGTYPE_OKCANCEL, 
		MSGTYPE_RETRYCANCEL, 
		MSGTYPE_ABORTRETRYCANCEL,
		MSGTYPE_CUSTOM,
		MSGTYPE_NOTYPE
	} messagebox_type;

	typedef enum 
	{
		MSGBTN_YES, 
		MSGBTN_NO, 
		MSGBTN_CANCEL, 
		MSGBTN_OK, 
		MSGBTN_RETRY, 
		MSGBTN_ABORT, 
		MSGBTN_NOBUTTON
	} messagebox_button;

	typedef enum {
		MSGICON_ERROR, 
		MSGICON_INFO, 
		MSGICON_QUESTION, 
		MSGICON_WARNING,
		MSGICON_NOICON
	}messagebox_icon;

	typedef enum _vslider_calibration_side {
		VSLIDER_NONE,
		SLIDER_LEFT,
		SLIDER_RIGHT
	}vslider_calibration_side;

	typedef enum _combobox_type {
		PW_COMBOBOX_EDITABLE,
		PW_COMBOBOX_READONLY
	} combobox_type ;

	

	typedef enum _pw_fdtype {
		PW_FD_READ,
		PW_FD_WRITE
	} pw_fdtype;

	

	typedef struct {
		double r, g, b;
	} pw_color;

	struct _pw_timer {
		struct timeval tv;
		long duration;
		int repeat;
		Bool (*callback)(void *arg1, void *arg2);
		void *arg1;
		void *arg2;
	};

	

	struct _pw_fd {
		int fd;
		pw_fdtype type;
		void (*callback)(void *arg1, void *arg2);
		void *arg1;
		void *arg2;
	};

	struct _px_pixmap {

		/* Empty structure not used */

	};

	typedef struct _pw_widget* pw_widget;
	typedef struct _pw_event* pw_event;
	typedef struct _pw_timer* pw_timer;
	typedef struct _pw_fd* pw_fd;
	typedef struct _px_pixmap* pw_pixmap; /* Not used */ 

	struct _pw_widget {
		unsigned char type;
		int x, y, width, height;
		char *tag;
		widget_state state;
		widget_border border;
		Bool hidden;
		pw_color bgcolor, fgcolor,border_color;

		pw_widget parent;
		void (*event_callback[PW_EVENT_END])(pw_widget widget, pw_event event);
		void *callback_parameters[PW_EVENT_END];

		void *internal;
	};

	struct _pw_event {
		int x, y;
		unsigned char ch[6];
		void *private_data;
	};
/*!\endcond */

// MessageBox button, return and icon types.
/*!\def PW_MSGBOX_BUTTON_YESNO
 * \brief The second argument of the pw_messagebox() function takes this to display a message box with "Yes" and "No" buttons.
 */
#define PW_MSGBOX_BUTTON_YESNO	    		MSGTYPE_YESNO
/*!\def PW_MSGBOX_BUTTON_YESNOCANCEL
 * \brief The second argument of the pw_messagebox() function takes this to display a message box with "Yes", "No" and "Cancel" buttons.
 */
#define PW_MSGBOX_BUTTON_YESNOCANCEL		MSGTYPE_YESNOCANCEL
/*!\def PW_MSGBOX_BUTTON_OK
 * \brief The second argument of the pw_messagebox() function takes this to display a message box with "Ok" button.
 */
#define PW_MSGBOX_BUTTON_OK			MSGTYPE_OK
/*!\def PW_MSGBOX_BUTTON_OKCANCEL
 * \brief The second argument of the pw_messagebox() function takes this to display a message box with "Ok" and "Cancel" buttons.
 */
#define PW_MSGBOX_BUTTON_OKCANCEL		MSGTYPE_OKCANCEL
/*!\def PW_MSGBOX_BUTTON_RETRYCANCEL
 * \brief The second argument of the pw_messagebox() function takes this to display a message box with "Retry" and "Cancel" buttons.
 */
#define PW_MSGBOX_BUTTON_RETRYCANCEL		MSGTYPE_RETRYCANCEL
/*!\def PW_MSGBOX_BUTTON_ABORTRETRYCANCEL
 * \brief The second argument of the pw_messagebox() function takes this to display a message box with "Abort", "Retry" and "Cancel" buttons.
 */
#define PW_MSGBOX_BUTTON_ABORTRETRYCANCEL	MSGTYPE_ABORTRETRYCANCEL

/*!\def PW_MSGBOX_RETURN_YES
 * \brief This value is returned by the pw_messagebox() function on clicking the "Yes" button.
 */
#define PW_MSGBOX_RETURN_YES			MSGBTN_YES
/*!\def PW_MSGBOX_RETURN_NO
 * \brief This value is returned by the pw_messagebox() function on clicking the "No" button.
 */
#define PW_MSGBOX_RETURN_NO			MSGBTN_NO
/*!\def PW_MSGBOX_RETURN_CANCEL
 * \brief This value is returned by the pw_messagebox() function on clicking the "Cancel" button.
 */
#define PW_MSGBOX_RETURN_CANCEL		    	MSGBTN_CANCEL
/*!\def PW_MSGBOX_RETURN_OK
 * \brief This value is returned by the pw_messagebox() function on clicking the "Ok" button.
 */
#define PW_MSGBOX_RETURN_OK		    	MSGBTN_OK
/*!\def PW_MSGBOX_RETURN_RETRY
 * \brief This value is returned by the pw_messagebox() function on clicking the "Retry" button.
 */
#define PW_MSGBOX_RETURN_RETRY		    	MSGBTN_RETRY
/*!\def PW_MSGBOX_RETURN_ABORT
 * \brief This value is returned by the pw_messagebox() function on clicking the "Abort" button.
 */
#define PW_MSGBOX_RETURN_ABORT		    	MSGBTN_ABORT
/*!\cond */
#define PW_MSGBOX_RETURN_NOBUTTON		MSGBTN_NOBUTTON
/*!\endcond */

/*!\def PW_MSGBOX_ICON_ERROR
 * \brief The third argument of the pw_messagebox() function takes this to display a message box with Error icon.
 */
#define PW_MSGBOX_ICON_ERROR			MSGICON_ERROR
/*!\def PW_MSGBOX_ICON_INFO
 * \brief The third argument of the pw_messagebox() function takes this to display a message box with Information icon.
 */
#define PW_MSGBOX_ICON_INFO			MSGICON_INFO
/*!\def PW_MSGBOX_ICON_QUESTION
 * \brief The third argument of the pw_messagebox() function takes this to display a message box with Question icon.
 */
#define PW_MSGBOX_ICON_QUESTION		    	MSGICON_QUESTION
/*!\def PW_MSGBOX_ICON_WARNING
 * \brief The third argument of the pw_messagebox() function takes this to display a message box with Warning icon.
 */
#define PW_MSGBOX_ICON_WARNING		    	MSGICON_WARNING
/*!\def PW_MSGBOX_ICON_NOICON
 * \brief The third argument of the pw_messagebox() function takes this to display a message box without any icon.
 */
#define PW_MSGBOX_ICON_NOICON			MSGICON_NOICON

/*
 * KeyCode set in the second byte of pw_event_getchar_utf8 when keys such as arrow, pageup, pagedown,home,del are pressed 
 */

/*!\def PW_NUMKEY_START
 * \brief This is the value of the first byte of pw_event_getchar_utf8 when a numpad control key is pressed
 */
#define PW_NUMKEY_START 		16

/*!\def PW_NUMKEY_HOME
 * \brief The home keycode set in the second byte of pw_event_getchar_utf8 
 */
#define PW_NUMKEY_HOME	    		80	

/*!\def PW_NUMKEY_ARROWLEFT
 * \brief The Arrow left keycode set in the second byte of pw_event_getchar_utf8 
 */
#define PW_NUMKEY_ARROWLEFT	    	81	

/*!\def PW_NUMKEY_ARROWUP
 * \brief The Arrow up keycode set in the second byte of pw_event_getchar_utf8 
 */
#define PW_NUMKEY_ARROWUP	    	82	

/*!\def PW_NUMKEY_ARROWRIGHT
 * \brief The Arrow right keycode set in the second byte of pw_event_getchar_utf8 
 */
#define PW_NUMKEY_ARROWRIGHT	    	83	

/*!\def PW_NUMKEY_ARROWDOWN
 * \brief The Arrow down keycode set in the second byte of pw_event_getchar_utf8 
 */
#define PW_NUMKEY_ARROWDOWN	    	84	

/*!\def PW_NUMKEY_PAGEUP
 * \brief The pageup keycode set in the second byte of pw_event_getchar_utf8 
 */
#define PW_NUMKEY_PAGEUP    		85	

/*!\def PW_NUMKEY_PAGEDOWN
 * \brief The pagedown keycode set in the second byte of pw_event_getchar_utf8 
 */
#define PW_NUMKEY_PAGEDOWN    		86	

/*!\def PW_NUMKEY_END
 * \brief The end keycode set in the second byte of pw_event_getchar_utf8 
 */
#define PW_NUMKEY_END	    		87	

/*!\def PW_NUMKEY_INSERT
 * \brief The insert keycode set in the second byte of pw_event_getchar_utf8 
 */
#define PW_NUMKEY_INSERT    		99	

/*
 * Entry Allowed chars constants 
 */

/*!\def PW_ENTRY_CHAR_NUMBERS
 * \brief Numbers
 */
#define PW_ENTRY_CHAR_NUMBERS			"0123456789"

/*!\def PW_ENTRY_CHAR_ALPHABETS_ALL
 * \brief alphabets all
 */
#define PW_ENTRY_CHAR_ALPHABETS_ALL		"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

/*!\def PW_ENTRY_CHAR_ALPHABETS_SMALL
 * \brief alphabets small
 */
#define PW_ENTRY_CHAR_ALPHABETS_SMALL		"abcdefghijklmnopqrstuvwxyz"

/*!\def PW_ENTRY_CHAR_ALPHABETS_CAPS
 * \brief alphabets capital
 */
#define PW_ENTRY_CHAR_ALPHABETS_CAPS		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"

/*!\def PW_ENTRY_CHAR_ALPHANUM
 * \brief Alphabets and numbers
 */
#define PW_ENTRY_CHAR_ALPHANUM			"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

/*!\def PW_ENTRY_CHAR_ALPHANUM_SPECIAL
 * \brief Alphabets, numbers and special chars (except space)
 */
#define PW_ENTRY_CHAR_ALPHANUM_SPECIAL		"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789`~!@#$%^&*()-_=+[{]}\\|;;'\",<.>/?"

/* [BASIC] */

/*!\fn int pw_init(unsigned flags)
 * \brief The very first function to be called in a pwrap program.
 *
 * \param flags 0 or an OR of any of the above mentioned PW_SUPPORT_* defines. The "grid_mode" program in the
 * examples shows the usage of enabling different kinds of support.
 *
 * \return 0 on Succeess, 1 on Failure 
 */
int pw_init(unsigned flags);


/*!\fn void pw_eventloop()
 * \brief This function must be called after calling all the necessary functions in the main program. All the
 * events are processed in this function. The program will not come out of this function, until a pw_stop_eventloop() is called.
 *
 * \return None.
 */
void pw_eventloop();

/*!\fn void pw_exit()
 * \brief This function must be called before the program exits. It releases all the resources allocated by pwrap.
 *
 * \return None.
 */
void pw_exit();

/* [EVENT HANDLING] */

/*!\fn void pw_stop_eventloop()
 * \brief This function can be called from within a event or a timer callback, to make the program come out of the
 * pw_eventloop() function. This is generally called in the buttons, which does application quit.
 *
 * \return
 */
void pw_stop_eventloop();

/*!\fn void pw_handle_one_event()
 * \brief All the events are handled automatically in the pw_eventloop() function. So, if a callback function is called
 * the next event is processed only after it returns. There are some cases when an event in the event queue needs to
 * be handled in a callback. In such cases, this function can be used.
 *
 * \return None.
 */
void pw_handle_one_event();

/*!\fn void pw_flush_eventloop()
 * \brief There are some kind of events, which are generated in surplus. For example, there can be numerous MOTION events
 * generated for a single strike on the screen. In such a case we would need only a few events to handled and discard the others.
 * This function can be called to achieve this. The "drag_widget" program in the examples shows the use of it.
 *
 * \return None.
 */
void pw_flush_eventloop();

/*!\fn pw_timer pw_create_timer(unsigned long time, int repeat, Bool (*callback)(void *arg1, void *arg2), void *arg1, void *arg2)
 * \brief This function is used to execute a procedure in periodic intervals.
 *
 * \param time Time interval in microseconds.
 * \param repeat 0 to execute the callback only once or 1 to execute it in regular intervals.
 * \param callback Pointer to a function to be called when the timer expires.
 * \param arg1 Pointer to a argument passed to the callback function. This will be available as the first argument in the callback function.
 * \param arg2 Pointer to a argument passed to the callback function. This will be available as the second argument in the callback function.
 *
 * \return pw_timer type variable.
 */
pw_timer pw_create_timer(unsigned long time, int repeat, Bool (*callback)(void *arg1, void *arg2), void *arg1, void *arg2);

/*!\fn void pw_destroy_timer(pw_timer timer)
 * \brief This is used to destroy a timer.
 *
 * \param timer Timer variable.
 *
 * \return None.
 */
void pw_destroy_timer(pw_timer timer);

/*!\fn int pw_event_getx(pw_event event)
 * \brief This function is used to get the x co-ordinate of a mouse event within a callback.
 *
 * \param event The event variable present in the second argument of the callback.
 *
 * \return None.
 */
int pw_event_getx(pw_event event);

/*!\fn int pw_event_gety(pw_event event)
 * \brief This function is used to get the y co-ordinate of a mouse event within a callback.
 *
 * \param event The event variable present in the second argument of the callback.
 *
 * \return None.
 */
int pw_event_gety(pw_event event);

/*!\fn unsigned char pw_event_getchar(pw_event event)
 * \brief This function is used to get the character in a keyboard event within a callback.
 *
 * \param event The event variable present in the second argument of the callback.
 *
 * \return Character entered through the keyboard.
 */
unsigned char  pw_event_getchar(pw_event event);

/*!\fn unsigned char * pw_event_getchar_utf8(pw_event event)
 * \brief This function is used to get the character in a keyboard event within a callback.
 *
 * \param event The event variable present in the second argument of the callback.
 *
 * \return Character entered through the keyboard.
 */
unsigned char *  pw_event_getchar_utf8(pw_event event);

/*!\fn void *pw_event_callback_parameter(pw_event event)
 * \brief This function is used to get parameter to the callback.
 *
 * \param event The event variable present in the second argument of the callback.
 *
 * \return Pointer to the parameter set using the pw_set_callback_parameter() function.
 */
void *pw_event_callback_parameter(pw_event event);

/* [GENERIC] */

/*!\fn void pw_show(pw_widget widget)
 * \brief This function is used to display a hidden widget.
 *
 * \param widget pw_widget variable.
 *
 * \return None.
 */
void pw_show(pw_widget widget);

/*!\fn void pw_show_many(pw_widget widget, ...)
 * \brief This function is used to display more than one widget at a time. The widgets can be specified
 * as a comma separated list, with a NULL at the last. For example: pw_show_many(entry1, entry2, label1, NULL);
 *
 * \param widget List of widgets.
 *
 * \return None.
 */
void pw_show_many(pw_widget widget, ...);

/*!\fn void pw_hide(pw_widget widget)
 * \brief This function is used to hide a widget.
 *
 * \param widget pw_widget variable.
 *
 * \return None.
 */
void pw_hide(pw_widget widget);

/*!\fn void pw_hide_many(pw_widget widget, ...)
 * \brief This function is used to hide more than one widget at a time. The widgets can be specified
 * as a comma separated list, with a NULL at the last. For example: pw_hide_many(entry1, entry2, label1, NULL);
 *
 * \param widget List of widgets.
 *
 * \return None.
 */
void pw_hide_many(pw_widget widget, ...);

/*!\fn void pw_raise(pw_widget widget)
 * \brief This function is used to raise a widget to the top from a pile of overlapping widgets. The "drag_widget"
 * program in the examples shows the use of it.
 *
 * \param widget pw_widget variable.
 *
 * \return None.
 */
void pw_raise(pw_widget widget);

/*!\fn void pw_lower(pw_widget widget)
 * \brief This function is used to push a widget down the pile of widgets.
 *
 * \param widget pw_widget variable.
 *
 * \return None.
 */
void pw_lower(pw_widget widget);

/*!\fn void pw_enable(pw_widget widget)
 * \brief This function is used to enable a widget. A widget is functional only if it is enabled.
 *
 * \param widget pw_widget variable.
 *
 * \return None.
 */
void pw_enable(pw_widget widget);

/*!\fn void pw_enable_many(pw_widget widget, ...)
 * \brief This function is used to enable more than one widget at a time. The widgets can be specified
 * as a comma separated list, with a NULL at the last. For example: pw_enable_many(entry1, entry2, label1, NULL);
 *
 * \param widget List of widgets.
 *
 * \return None
 */
void pw_enable_many(pw_widget widget, ...);

/*!\fn void pw_disable(pw_widget widget)
 * \brief This function is used to disable a widget. A widget will not be functional if it is disabled.
 *
 * \param widget pw_widget variable.
 *
 * \return None.
 */
void pw_disable(pw_widget widget);

/*!\fn void pw_disable_many(pw_widget widget, ...)
 * \brief This function is used to disable more than one widget at a time. The widgets can be specified
 * as a comma separated list, with a NULL at the last. For example: pw_disable_many(entry1, entry2, label1, NULL);
 *
 * \param widget List of widgets.
 *
 * \return None.
 */
void pw_disable_many(pw_widget widget, ...);

/*!\fn void pw_destroy(pw_widget widget)
 * \brief This function is used to destroy any widget created using the pw_create_* functions.
 *
 * \param widget pw_widget variable.
 *
 * \return None.
 */
void pw_destroy(pw_widget widget);

/*!\fn void pw_destroy_many(pw_widget widget, ...)
 * \brief This function is used to destroy more than one widget at a time. The widgets can be specified
 *  as a comma separated list, with a NULL at the last. For example: pw_destroy_many(entry1, entry2, label1, NULL);
 *
 * \param widget List of widgets.
 *
 * \return None.
 */
void pw_destroy_many(pw_widget widget, ...);

/*!\fn void pw_focus(pw_widget widget)
 * \brief This function is used to set keyboard focus on a widget.
 *
 * \param widget pw_widget variable.
 *
 * \return None.
 */
void pw_focus(pw_widget widget);

/*!\fn void pw_grab(pw_widget widget)
 * \brief This function is used to grab all the events for a widget. On setting a grab on a widget, all the
 * events happening within or outside the widget region will be reported.
 *
 * \param widget pw_widget variable.
 *
 * \return None.
 */
void pw_grab(pw_widget widget);

/*!\fn void pw_ungrab()
 * \brief This function is used to release a grab set to a widget. A grab is always set to one widget, so this function
 * does not take any argument.
 *
 * \return None.
 */
void pw_ungrab();

/*!\fn void pw_configure(pw_widget widget, ...)
 * \brief This function is used to set attributes of a widget. This allows to set several attributes at a
 * time, there by avoiding calls to individual functions.<br> 
 * For example: pw_configure(label1, PW_TEXT, "Hello", PW_FGCOLOR, "red", PW_FONT, PW_BOLD_FONT, PW_NA);<br> will set the
 * widget label1 with "Hello" text, red foreground color and bold font. The same can also be achieved using the following 3 statements:<br>
 * pw_set_text(label1, "Hello");<br> pw_set_fgcolor(label1, "red");<br> pw_set_font(label1, PW_BOLD_FONT);<br>
 * The former is more efficient than the latter.
 *
 * \param widget pw_widget variable followed by attribute settings.
 *
 * \return None.
 */
void pw_configure(pw_widget widget, ...);

/*!\fn void pw_update(pw_widget widget, int show)
 * \brief A call to change the attribute of a widget within a callback is reflected only after its return. Incase a the widget needs to
 * be updated as and when an attribute is changed, this function must be called.
 *
 * \param widget pw_widget variable.
 * \param show Generally set to 1.
 *
 * \return None.
 */
void pw_update(pw_widget widget, int show);

/*!\fn void pw_redraw(pw_widget widget)
 * \brief A call to change the attribute of a widget within a callback is reflected only after its return. Incase a the widget needs to
 * be updated as and when an attribute is changed, this function must be called.
 *
 * \param widget pw_widget variable.
 * \param show Generally set to 1.
 *
 * \return None.
 */
void pw_redraw(pw_widget widget);

/*!\fn void pw_set_title(pw_widget widget, const char *title)
 * \brief This function is used to set the title of the toplevel window.
 *
 * \param widget pw_widget variable.
 * \param title Title text to be set.
 *
 * \return None.
 */
void pw_set_title(pw_widget widget, const char *title);

/*!\fn void pw_set_text(pw_widget widget, const char *text, ...)
 * \brief This function is used to set the text of a widget. The text can be a format string. For example,
 * pw_set_text(label1, "Item: %s Price: %d", "simputer", 10000) is possible.
 *
 * \param widget pw_widget variable.
 * \param text Formatted text string.
 *
 * \return None.
 */
void pw_set_text(pw_widget widget, const char *text, ...);

/*!\fn void pw_set_font(pw_widget widget, const char *font);
 * \brief This function is used to set the font type of a widget.
 *
 * \param widget pw_widget variable.
 * \param font Either of the above mentioned PW_*_FONT defines.
 *
 * \return None
 */
void pw_set_font(pw_widget widget, const char *font);

/*!\fn void pw_set_justify(pw_widget widget, unsigned justification)
 * \brief This function is used to set the text justification in a widget.
 *
 * \param widget pw_widget variable.
 * \param justification Either of the above PW_JUSTIFY_* defines.
 *
 * \return None.
 */
void pw_set_justify(pw_widget widget, unsigned justification);

/*!\fn void pw_set_wrap(pw_widget widget, unsigned wraptype)
 * \brief This function is used to set the text wrap type in a widget.
 *
 * \param widget pw_widget variable.
 * \param wraptype Either of the above PW_*_WRAP defines.
 *
 * \return None.
 */
void pw_set_wrap(pw_widget widget, unsigned wraptype);

/*!\fn void pw_set_border(pw_widget widget, unsigned bordertype)
 * \brief This function is used to set the border type of a widget.
 *
 * \param widget pw_widget variable.
 * \param bordertype Either of the above PW_*_BORDER defines.
 *
 * \return None.
 */
void pw_set_border(pw_widget widget, unsigned bordertype);

/*!\fn void pw_set_tag(pw_widget widget, const char *tag, ...)
 * \brief This function is used to set a tag name for a widget. A tag can be used to set an identifier for a widget.
 * The "display_info" example program shows the usage of a tag.
 *
 * \param widget pw_widget variable.
 * \param tag Tag name can be a format string. For example, pw_set_tag(label1, "name%d", 1) is possible.
 *
 * \return None.
 */
void pw_set_tag(pw_widget widget, const char *tag, ...);

/*!\fn void pw_set_fgcolor(pw_widget widget, const char *color)
 * \brief This function is used to set the foreground text color of the widget.
 *
 * \param widget pw_widget variable.
 * \param color Color specified as a name or in the "#rrggbb" format.
 *
 * \return None.
 */
void pw_set_fgcolor(pw_widget widget, const char *color);

/*!\fn void pw_set_bgcolor(pw_widget widget, const char *color)
 * \brief This function is used to set the background color of the widget.
 *
 * \param widget pw_widget variable.
 * \param color Color specified as a name or in the "#rrggbb" format.
 *
 * \return None.
 */
void pw_set_bgcolor(pw_widget widget, const char *color);

/*!\fn void pw_set_border_color(pw_widget widget, const char *color)
 * \brief This function is used to set the border color of the widget.
 *
 * \param widget pw_widget variable.
 * \param color Color specified as a name or in the "#rrggbb" format.
 *
 * \return None.
 */
void pw_set_border_color(pw_widget widget, const char *color);

/*!\fn void pw_set_bgtransparent(pw_widget widget)
 * \brief This function is used to set transperant background.
 *
 * \param widget pw_widget variable.
 *
 * \return None.
 */
void pw_set_bgtransparent(pw_widget widget);

/*!\fn void pw_set_bgimage_file(pw_widget widget, const char *file, ...)
 * \brief This function is used to set a background image from a file.
 *
 * \param widget pw_widget variable.
 * \param file	Image file path. The path can take a format string. For example, pw_create_pixmap_from_file("%s/%s", "/tmp", "img.jpg") is possible.
 *
 * \return None.
 */
void pw_set_bgimage_file(pw_widget widget, const char *file, ...);

/*!\fn void pw_set_position(pw_widget widget, int x, int y)
 * \brief This function is used to move a widget to a particular place on its parent widget.
 *
 * \param widget pw_widget variable.
 * \param x X-position of the widget's new location.
 * \param y Y-position of the widget's new location.
 *
 * \return None.
 */
void pw_set_position(pw_widget widget, int x, int y);

/*!\fn void pw_set_size(pw_widget widget, int width, int height)
 * \brief This function is used to set the dimension of a widget.
 *
 * \param widget pw_widget variable.
 * \param width Width of the widget.
 * \param height Height of the widget.
 *
 * \return None.
 */
void pw_set_size(pw_widget widget, int width, int height);

/*!\fn void pw_set_normal_image(pw_widget widget, char *filename)
 * \brief This function is used to specify the normal image of a widget.
 *
 * \param widget pw_widget variable.
 * \param filename Image file path.
 *
 * \return None.
 */
void pw_set_normal_image(pw_widget widget, char *filename);

/*!\fn void pw_set_active_image(pw_widget widget, char *filename)
 * \brief This function is used to specify the active image of a widget.
 *
 *  \param widget pw_widget variable.
 *  \param filename Image file path.
 *
 * \return None.
 */
void pw_set_active_image(pw_widget widget, char *filename);

/*!\fn void pw_set_disabled_image(pw_widget widget, char *filename)
 * \brief This function is used to specify the disabled image of a widget. This function is only for compatibility. 
 *  Active image is created using the normal image itself
 *
 * \param widget pw_widget variable.
 * \param filename Image file path.
 *
 * \return None.
 */
void pw_set_disabled_image(pw_widget widget, char *filename);

/*!\fn void pw_set_callback(pw_widget widget, unsigned eventtype, void (*callback)(pw_widget, pw_event))
 * \brief This function is used to set a callback function for a particular event. This function is only for compatibility
 *
 * \param widget pw_widget variable.
 * \param eventtype Either of the above PW_*_EVENT defines.
 * \param callback Pointer to the event callback function.
 *
 * \return None.
 */
void pw_set_callback(pw_widget widget, unsigned eventtype, void (*callback)(pw_widget, pw_event));

/*!\fn void pw_set_callback_parameter(pw_widget widget, unsigned eventtype, void *parameter)
 * \brief This function is used to set a parameter for event callback. The parameter can be retreived using
 * the pw_event_callback_parameter() function, within a callback.
 *
 * \param widget pw_widget variable.
 * \param eventtype Either of the above PW_*_EVENT defines.
 * \param parameter Pointer to parameter.
 *
 * \return None.
 */
void pw_set_callback_parameter(pw_widget widget, unsigned eventtype, void *parameter);

/*!\fn void pw_get_position(pw_widget widget, int *x, int *y)
 * \brief This function returns the position of a widget.
 *
 * \param widget pw_widget variable.
 * \param x The x co-ordinate will be stored in this variable.
 * \param y The y co-ordinate will be stored in this variable.
 *
 * \return None.
 */
void pw_get_position(pw_widget widget, int *x, int *y);

/*!\fn const char *pw_get_text(pw_widget widget)
 * \brief This function returns the text of a widget.
 *
 * \param widget pw_widget variable.
 *
 * \return Text of the widget.
 */
const char *pw_get_text(pw_widget widget);

/*!\fn const char *pw_get_tag(pw_widget widget)
 * \brief This function is used to get the tag name associated with a widget. This is generally used in
 * a common callback function to know the kind of the widget.
 *
 * \param widget pw_widget variable.
 *
 * \return Tag name of the widget.
 */
const char *pw_get_tag(pw_widget widget);

/*!\fn void pw_get_size(pw_widget widget, int *width, int *height)
 * \brief  This function returns the dimension of a widget.
 *
 * \param widget pw_widget variable.
 * \param width Width of the widget.
 * \param height Height of the widget.
 *
 * \return None.
 */
void pw_get_size(pw_widget widget, int *width, int *height);

/*!\fn Window pw_get_xwindow(pw_widget widget)
 * \brief This function returns the Window Id of the widget.
 *
 * \param widget pw_widget variable.
 *
 * \return Window Id of the widget.
 */
Window pw_get_xwindow(pw_widget widget);
	
/* [GRAPHICS] */

/*!\fn void pw_set_drawcolor(const char *color)
 * \brief This function is used to set the drawing color. All the graphics primitives drawn will be drawn with this
 * color.
 *
 * \param color Color name or a string in the form "#rrggbb".
 *
 * \return None.
 */
void pw_set_drawcolor(const char *color);

/*!\fn void pw_draw_point(pw_widget widget, int x, int y)
 * \brief This function is used to draw a point on a widget at the specified location.
 *
 * \param widget pw_widget variable.
 * \param x X co-ordinate of the point to be drawn.
 * \param y Y co-ordinate of the point to be drawn.
 *
 * \return None.
 */
void pw_draw_point(pw_widget widget, int x, int y);

/*!\fn void pw_draw_line(pw_widget widget, int x1, int y1, int x2, int y2)
 * \brief This function is used to draw a line on the widget at the specified location.
 *
 * \param widget pw_widget variable.
 * \param x1 X co-ordinate of the first point.
 * \param y1 Y co-ordinate of the first point.
 * \param x2 X co-ordinate of the second point.
 * \param y2 Y co-ordinate of the second point.
 *
 * \return None.
 */
void pw_draw_line(pw_widget widget, int x1, int y1, int x2, int y2);

/*!\fn void pw_draw_rectangle(pw_widget widget, int x, int y, int width, int height)
 * \brief This function is used to draw a rectangle on a widget at the specified location.
 *
 * \param widget pw_widget variable.
 * \param x X co-ordinate of the top left rectangle's corner.
 * \param y Y co-ordinate of the top left rectangle's corner.
 * \param width Width of the rectangle.
 * \param height Height of the rectangle.
 *
 * \return None.
 */
void pw_draw_rectangle(pw_widget widget, int x, int y, int width, int height);

/*!\fn void pw_fill_rectangle(pw_widget widget, int x, int y, int width, int height)
 * \brief This function draws a filled rectangle on a widget at the specified location.
 *
 * \param widget pw_widget variable.
 * \param x X co-ordinate of the top left rectangle's corner.
 * \param y Y co-ordinate of the top left rectangle's corner.
 * \param width Width of the rectangle.
 * \param height Height of the rectangle.
 *
 * \return None.
 */
void pw_fill_rectangle(pw_widget widget, int x, int y, int width, int height);

/*!\fn void pw_draw_circle(pw_widget widget, int x, int y, int radius)
 * \brief This function draws a circle on a widget at the specified location.
 *
 * \param widget pw_widget variable.
 * \param x X co-ordinate of the circle center.
 * \param y Y co-ordinate of the circle center.
 * \param radius Radius of the circle in pixels.
 *
 * \return
 */
void pw_draw_circle(pw_widget widget, int x, int y, int radius);

/*!\fn void pw_draw_arc(pw_widget widget, int x, int y, int a, int b, int start_angle, int end_angle)
 * \brief This function is used to draw an arc on a widget at the specified location.
 *
 * \param widget pw_widget variable.
 * \param x X co-ordinate of the arc center. 
 * \param y Y co-ordinate of the arc center. 
 * \param a Length of the major axis.
 * \param b Length of the minor axis.
 * \param start_angle Angle from which the arc drawing should start.
 * \param end_angle Angle at which the arc drawing should end.
 *
 * \return None.
 */
void pw_draw_arc(pw_widget widget, int x, int y, int a, int b, int start_angle, int end_angle);

/*!\fn void pw_fill_arc(pw_widget widget, int x, int y, int a, int b, int start_angle, int end_angle)
 * \brief This function is used to draw a filled arc on a widget at the specified location.
 *
 * \param widget pw_widget variable.
 * \param x X co-ordinate of the arc center. 
 * \param y Y co-ordinate of the arc center. 
 * \param a Length of the major axis.
 * \param b Length of the minor axis.
 * \param start_angle Angle from which the arc drawing should start.
 * \param end_angle Angle at which the arc drawing should end.
 *
 * \return None.
 */
void pw_fill_arc(pw_widget widget, int x, int y, int a, int b, int start_angle, int end_angle);

/*!\fn void pw_draw_text(pw_widget widget, int x, int y, char *text, char *font, char *color)
 * \brief This function is used to draw text of particular font and colour at a given co-ordinate on a toplevel or frame
 *
 * \param widget widget to draw on
 * \param x X co-ordinate of text
 * \param y Y co-ordinate of text
 * \param text The text to be drawn
 * \param font The font to be used
 * \param color The colour of the text
 *
 * \return None
 */
void pw_draw_text(pw_widget widget, int x , int y, char *text, char *font, char *color);

/* [EXTRAS] */

/*!\fn void pw_alchemy_command(pw_widget toplevel, int command, const char *data, ...)
 * \brief This function is used to send commands to Alchemy window manager.
 *
 * \param toplevel Toplevel window variable.
 * \param command Can be anyone of the above mentioned PW_WM_* defines.
 * \param data Takes a formatted string. For example, pw_alchemy_command(toplevel, PW_WM_SET_STATUS_MSG, "%d photos", 10); can be used.
 *
 * \return None.
 */
void pw_alchemy_command(pw_widget toplevel, int command, const char *data, ...);

/*!\fn messagebox_button pw_messagebox(	pw_widget toplevel, messagebox_button type, messagebox_icon icon, const char *title, const char *mesg)
 * \brief This function is used to display a message box. The type of message box can chosen be sending appropriate parameters.
 *
 * \param toplevel Toplevel window variable.
 * \param type Any one of the above mentioned PW_MSGBOX_BUTTON_* defines.
 * \param icon Any one of the above mentioned PW_MSGBOX_ICON_* defines.
 * \param title Title text of the message box.
 * \param mesg Message to be displayed in the message box.
 *
 * \return Any of the above mentioned PW_MSGBOX_RETURN_* defines.
 */
messagebox_button pw_messagebox(pw_widget toplevel,messagebox_button type, messagebox_icon icon,
						const char *title, const char *mesg );

/*!\fn messagebox_button pw_messagebox_custom(pw_widget toplevel, messagebox_button type, messagebox_icon icon, const char *title, const char *mesg, ...)
 * \brief This function is used to display a message box. The type of message box can chosen be sending appropriate parameters. 
 *
 * \param toplevel Toplevel window variable.
 * \param type Any one of the above mentioned PW_MSGBOX_BUTTON_* defines.
 * \param icon Any one of the above mentioned PW_MSGBOX_ICON_* defines.
 * \param title Title text of the message box.
 * \param mesg Message to be displayed in the message box.
 * \param <button_type> <customtext> button text to be replaced, example: pw_messagebox_custom(toplevel, ... , MSGBTN_OK, "Okay", MSGBTN_NOBUTTON) 
 *
 * \return Any of the above mentioned PW_MSGBOX_RETURN_* defines.
 */
messagebox_button pw_messagebox_custom(pw_widget toplevel,messagebox_button type, messagebox_icon icon,
						const char *title, const char *mesg, ... );

/*!\fn void pw_set_display_orientation(pw_widget window, unsigned orientation)
 * \brief This function is used to set the display orientation.
 *
 * \param window Toplevel window variable.
 * \param orientation Any one of the above mentioned PW_ORIENT_* defines.
 *
 * \return None.
 */
void pw_set_display_orientation(pw_widget window, unsigned orientation);

/*!\fn unsigned pw_get_display_width(pw_widget window)
 * \brief This function is used to the display width. Not implemented in pwrap2
 *
 * \param window Toplevel window variable.
 *
 * \return Display width.
 */
unsigned pw_get_display_width(pw_widget window);

/*!\fn unsigned pw_get_display_height(pw_widget window)
 * \brief This function is used to the display height. Not implemented in pwrap2
 *
 * \param window Toplevel window variable.
 *
 * \return Display height.
 */
unsigned pw_get_display_height(pw_widget window);

/*!\fn unsigned pw_get_display_orientation(pw_widget window)
 * \brief This function is used to get the current display orientation. Not implemented in pwrap2
 *
 * \param window Toplevel window variable.
 *
 * \return Any of the PW_ORIENT_* values.
 */
unsigned pw_get_display_orientation(pw_widget window);

/*!\fn void pw_toggle_fullscreen(pw_widget window)
 * \brief This function is used to toggle the application between fullscreen and normal screen. 
 *
 * \param window Toplevel window variable.
 *
 * \return None.
 */
void pw_toggle_fullscreen(pw_widget window);

/* [WIDGETS] */

/* [WINDOW] */

/*!\fn pw_widget pw_create_window(int width, int height, ...)
 * \brief This function is used to create a toplevel window.
 *
 * \param width Width of the toplevel window.
 * \param height Height of the toplevel window.
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return Toplevel widget variable.
 */
pw_widget pw_create_window(int width, int height, ...);

/*!\fn pw_widget pw_create_child_window(pw_widget parent, unsigned width, unsigned height, ...)
 * \brief This function is used to create a child window.
 *
 * \param parent Parent window variable.
 * \param width Width of the toplevel window.
 * \param height Height of the toplevel window.
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return
 */
pw_widget pw_create_child_window(pw_widget parent, unsigned width, unsigned height, ...);

/*!\fn int pw_compare_clientmessage(pw_event  event, const char *message)
 * \brief This function is used to compare a message received from the Alchemy window manager. Seldom used by
 * applications.
 *
 * \param event Event variable.
 * \param message Message sent by the window manager.
 *
 * \return 0 on mismatch OR 1 on match.
 */
int pw_compare_clientmessage(pw_event event, const char *message);

/*!\fn int pw_configure_event_width(pw_event  event)
 * \brief This funtion is used to get the changed window width in a configure event callback.
 *
 * \param event Event variable.
 *
 * \return Width in pixels.
 */
int pw_configure_event_width(pw_event  event);

/*!\fn int pw_configure_event_height(pw_event  event)
 * \brief This funtion is used to get the changed window height in a configure event callback.
 *
 * \param event Event variable.
 *
 * \return Height in pixels.
 */
int pw_configure_event_height(pw_event  event);

/* [DIALOG WINDOW] */

/*!\fn pw_widget pw_create_dialog(unsigned width, unsigned height, ...)
 * \brief This function is used to create a Dialog box window. The "dialog_box" program in the examples shows the
 * use of it.
 *
 * \param width Width of the dialog box window.
 * \param height Height of the dialog box window.
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return
 */
pw_widget pw_create_dialog(unsigned width, unsigned height, ...);

/*!\fn pw_widget pw_create_child_dialog(pw_widget parent, unsigned width, unsigned height, ...)
 * \brief This function is used to create a child dialog box window.
 *
 * \param parent Parent window variable.
 * \param width Width of the dialog box window.
 * \param height Height of the dialog box window.
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return
 */
pw_widget pw_create_child_dialog(pw_widget parent, unsigned width, unsigned height, ...);

/* [FRAME] */

/*!\fn pw_widget pw_create_frame(pw_widget parent, int x, int y, int width, int height, ...);
 * \brief This is used to create a frame widget. A frame is border less rectangular widget, on which other widgets are created.
 *
 * \param parent Parent widget variable.
 * \param x X co-ordinate, where the widget will be placed.
 * \param y Y co-ordinate, where the widget will be placed.
 * \param width Width of the widget.
 * \param height Height of the widget.
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return Frame widget variable.
 */
pw_widget pw_create_frame(pw_widget parent, int x, int y, int width, int height, ...);

/* [LABEL] */

/*!\fn pw_widget pw_create_label(pw_widget parent,
 * 						int x, int y, int width, int height, ...);
 * \brief This is used to create a label widget.
 *
 * \param parent Parent widget variable.
 * \param x X co-ordinate, where the widget will be placed.
 * \param y Y co-ordinate, where the widget will be placed.
 * \param width Width of the widget.
 * \param height Height of the widget.
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return Label widget variable.
 */
pw_widget pw_create_label(pw_widget parent, int x, int y, int width, int height, ...);

/* [BUTTON] */

/*!\fn pw_widget pw_create_button(pw_widget parent,int x, int y, int width, int height, ...);
 *
 * \brief This is used to create a button widget.
 *
 * \param parent Parent widget variable.
 * \param x X co-ordinate, where the widget will be placed.
 * \param y Y co-ordinate, where the widget will be placed.
 * \param width Width of the widget.
 * \param height Height of the widget.
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return Button widget variable.
 */
pw_widget pw_create_button(pw_widget parent, int x, int y, int width, int height, ...);

/* //[ENTRY] */

/*!\fn pw_widget pw_create_entry(pw_widget parent, int x, int y, int width, int height, ...);
 * \brief This is used to create a text entry widget.
 *
 * \param parent Parent widget variable.
 * \param x X co-ordinate, where the widget will be placed.
 * \param y Y co-ordinate, where the widget will be placed.
 * \param width Width of the widget.
 * \param height Height of the widget.
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return Entry widget variable.
 */
pw_widget pw_create_entry(pw_widget parent, int x, int y, int width, int height, ...);

/*!\fn void pw_entry_clear(pw_widget widget)
 * \brief This function is used to clear the text contents of a widget.
 *
 * \param widget Entry widget variable.
 *
 * \return None.
 */
void pw_entry_clear(pw_widget widget);

/*!\fn void pw_entry_passwdchar(pw_widget widget, char ch)
 * \brief This function is used to password character of an entry widget. On setting this, the text entered will only be
 * displayed in this character.
 *
 * \param widget Entry widget variable.
 * \param ch Password character.
 *
 * \return None.
 */
void pw_entry_passwdchar(pw_widget widget, char ch);

/*!\fn void pw_entry_engtxt_len(pw_widget widget, int length)
 * \brief This function is used to set maximum length of english text allowed in the widget 
 *
 * \param widget Entry widget variable.
 * \param length Length allowed
 *
 * \return None.
 */
void pw_entry_engtxt_len(pw_widget widget, int length);

/*!\fn void pw_entry_engtxt_chars(pw_widget widget, chars *allowed_chars)
 * \brief This function is used to set allowed english characters in the widget
 *
 * \param widget Entry widget variable.
 * \param allowed_chars Array of characters allowed 
 *
 * \return None.
 */
void pw_entry_engtxt_chars(pw_widget widget, char *allowed_chars);

/*!\fn int pw_get_entry_engtxt_len(pw_widget widget)
 * \brief This function returns the maximum length allowed.
 *
 * \param widget Entry widget variable.
 *
 * \return Integer of length allowed.
 */
int pw_get_entry_engtxt_len(pw_widget widget);

/*!\fn char pw_get_entry_passwdchar(pw_widget widget)
 * \brief This function returns the password character of the entry.
 *
 * \param widget pw_widget variable.
 *
 * \return password character of the widget.
 */
char pw_get_entry_passwdchar(pw_widget widget);
  
/*!\fn char *pw_get_entry_engtxt_chars(pw_widget widget)
 * \brief This function returns the allowed english characters for the entry.
 *
 * \param widget pw_widget variable.
 *
 * \return allowed english characters for the entry.
 */
char *pw_get_entry_engtxt_chars(pw_widget widget);

/*!\fn char *pw_get_font_type(pw_widget widget)
 * \brief This function returns the font type of the widget.
 *
 * \param widget pw_widget variable.
 *
 * \return font type of the widget.
 */
char *pw_get_font_type(pw_widget widget);


/* [RADIO BUTTON] */

/*!\fn pw_widget pw_create_radiobutton(	pw_widget parent, int x, int y, int width, int height, pw_widget group, ...);
 * \brief This is used to create a text entry widget. The "radiobutton_demo" program in the examples shows the use of it.
 *
 * \param parent Parent widget variable.
 * \param x X co-ordinate, where the widget will be placed.
 * \param y Y co-ordinate, where the widget will be placed.
 * \param width Width of the widget.
 * \param height Height of the widget.
 * \param group Radiobutton widget variable. 
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return Radio button widget variable.
 */
pw_widget pw_create_radiobutton(pw_widget parent, int x, int y, int width, int height, pw_widget group, ...);

/*!\fn void pw_radiobutton_select(pw_widget widget)
 * \brief This function is used to select a radiobutton.
 *
 * \param widget Radiobutton widget variable. 
 *
 * \return None.
 */
void pw_radiobutton_select(pw_widget widget);

/*!\fn unsigned pw_radiobutton_status(pw_widget widget)
 * \brief This function is used to know whether a radiobutton is selected or not.
 *
 * \param  widget Radiobutton widget variable. 
 *
 * \return 1 if selected, 0 if not.
 */
int pw_radiobutton_status(pw_widget widget);

/* [CHECK BUTTON] */

/*!\fn pw_widget pw_create_checkbutton(pw_widget parent, int x, int y, int width, int height, ...);
 * \brief This is used to create a checkbutton  widget.
 *
 * \param parent Parent widget variable.
 * \param x X co-ordinate, where the widget will be placed.
 * \param y Y co-ordinate, where the widget will be placed.
 * \param width Width of the widget.
 * \param height Height of the widget.
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return Checkbutton widget variable.
 */
pw_widget pw_create_checkbutton(pw_widget parent, int x, int y, int width, int height, ...);

/*!\fn void pw_checkbutton_select(pw_widget widget)
 * \brief This function is used to select a checkbutton.
 *
 * \param widget Checkbutton widget variable.
 *
 * \return None.
 */
void pw_checkbutton_select(pw_widget widget);

/*!\fn void pw_checkbutton_deselect(pw_widget widget)
 * \brief This function is used to remove selection on a checkbutton.
 *
 * \param widget Checkbutton widget variable.
 *
 * \return None.
 */
void pw_checkbutton_deselect(pw_widget widget);

/*!\fn int pw_checkbutton_status(pw_widget widget)
 * \brief This function is used to know the status if the checkbutton.
 *
 * \param widget Checkbutton widget variable.
 *
 * \return 1 if selected, 0 if not.
 */
int pw_checkbutton_status(pw_widget widget);

/* [WIDGET LISTBOX] */

/*!\fn pw_widget pw_create_wlistbox(pw_widget parent, int x, int y, int width, int height, ...);
 * \brief This is used to create a widget listbox. The "multipage_demo" program in the examples
 * shows the use of it.
 *
 * \param parent Parent widget variable.
 * \param x X co-ordinate, where the widget will be placed.
 * \param y Y co-ordinate, where the widget will be placed.
 * \param width Width of the widget.
 * \param height Height of the widget.
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return Widget listbox variable.
 */
pw_widget pw_create_wlistbox(pw_widget parent, int x, int y, int width, int height, ...);

/*!\fn void pw_wlistbox_insert(pw_widget widget, pw_widget element, unsigned index)
 * \brief This function is used to insert a widget element at a specified index in a widget listbox.
 *
 * \param widget Widget listbox widget variable.
 * \param element Widget to be inserted.
 * \param index Index at which the widget will be inserted.
 *
 * \return None.
 */
void pw_wlistbox_insert(pw_widget widget, pw_widget element, unsigned index);

/*!\fn void pw_wlistbox_insertend(pw_widget widget, pw_widget element)
 * \brief This function is used to insert a widget element into a widget listbox at the end.
 *
 * \param widget Widget listbox widget variable.
 * \param element Widget to be inserted.
 *
 * \return None.
 */
void pw_wlistbox_insertend(pw_widget widget, pw_widget element);

/*!\fn void pw_wlistbox_remove(pw_widget widget, unsigned rownum)
 * \brief This function is used to remove an element from a widget listbox.
 *
 * \param widget Widget listbox widget variable.
 * \param rownum Row number to be deleted.
 *
 * \return None.
 */
void pw_wlistbox_remove(pw_widget widget, unsigned rownum);

/*!\fn void pw_wlistbox_remove(pw_widget widget, unsigned rownum)
 * \brief This function is used to remove an element from a widget listbox. The widget is not destroyed.
 *
 * \param widget Widget listbox widget variable.
 * \param rownum Row number to be deleted.
 *
 * \return None.
 */
void pw_wlistbox_remove_no_destroy(pw_widget widget, unsigned rownum);

/*!\fn void pw_wlistbox_clear(pw_widget widget)
 * \brief This function is used to clear a widget listbox.
 *
 * \param widget Widget listbox widget variable.
 *
 * \return None.
 */
void pw_wlistbox_clear(pw_widget widget);

/*!\fn pw_widget pw_wlistbox_get_element(pw_widget widget, unsigned rownum)
 * \brief This function is used to get the widget element at the specified row.
 *
 * \param widget Widget listbox widget variable.
 * \param rownum Row number to be deleted.
 *
 * \return Widget varible.
 */
pw_widget pw_wlistbox_get_element(pw_widget widget, unsigned rownum);

/*!\fn int pw_wlistbox_get_selection(pw_widget widget)
 * \brief This function is used to the selected element.
 *
 * \param widget Widget listbox widget variable.
 *
 * \return Selected index number.
 */
int pw_wlistbox_get_selection(pw_widget widget);

/*!\fn void pw_wlistbox_see(pw_widget widget, unsigned rownum)
 * \brief This function is used to make a row visible, if it is scrolled out of the view.
 *
 * \param widget Widget listbox widget variable.
 * \param rownum Row number to be deleted.
 *
 * \return None.
 */
void pw_wlistbox_see(pw_widget widget, unsigned rownum);

/*!\fn int pw_wlistbox_get_numrows(pw_widget widget)
 * \brief This function is used to get the number of rows in a widget listbox.
 *
 * \param widget Widget listbox widget variable.
 *
 * \return Number of rows in a widget listbox.
 */
int pw_wlistbox_get_numrows(pw_widget widget);

/* [TEXT LISTBOX] */

/*!\fn pw_widget pw_create_tlistbox(pw_widget parent, int x, int y, int width, int height, ...);
 * \brief This is used to create a multicolumn text listbox widget. The "textlistbox_demo" program in the examples
 * shows the use of it.
 *
 * \param parent Parent widget variable.
 * \param x X co-ordinate, where the widget will be placed.
 * \param y Y co-ordinate, where the widget will be placed.
 * \param width Width of the widget.
 * \param height Height of the widget.
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return Text listbox variable.
 */
pw_widget pw_create_tlistbox(pw_widget parent, int x, int y, int width, int height, ...);

/*!\fn void pw_tlistbox_addcolumn(pw_widget widget, unsigned width)
 * \brief This function is used to add a column to the text listbox.
 *
 * \param widget Text listbox widget variable.
 * \param width Width of the column in pixels.
 *
 * \return None.
 */
void pw_tlistbox_addcolumn(pw_widget widget, unsigned width);

/*!\fn void pw_tlistbox_insert(pw_widget widget, char *text, unsigned colnum, unsigned rownum)
 * \brief This function is used to insert an element into the text listbox at the specified row and column.
 *
 * \param widget Text listbox widget variable.
 * \param text Text string to be inserted.
 * \param colnum Column number.
 * \param rownum Row number.
 *
 * \return None.
 */
void pw_tlistbox_insert(pw_widget widget, char *text, unsigned colnum, unsigned rownum);

/*!\fn void pw_tlistbox_insertend(pw_widget widget, char *text, unsigned colnum)
 * \brief This function is used to insert an element at the end of the text listbox.
 *
 * \param widget Text listbox widget variable.
 * \param text Text string.
 * \param colnum Column number.
 *
 * \return None.
 */
void pw_tlistbox_insertend(pw_widget widget, char *text, unsigned colnum);

/*!\fn void pw_tlistbox_remove(pw_widget widget, unsigned row, Bool refresh_fg)
 * \brief This function is used to remove an element from the text listbox.
 *
 * \param widget Text listbox widget variable.
 * \param row Row number to be deleted.
 * \param refresh_fg True if the textbox display needs to be updated or False otherwise. There are situations
 * where several entries needs to be deleted. In such a case it is ideal to set this flag to Fasle for all the
 * elements, except for the last one.
 *
 * \return None.
 */
void pw_tlistbox_remove(pw_widget widget, unsigned row, Bool refresh_fg);

/*!\fn void pw_tlistbox_clear(pw_widget widget)
 * \brief This function is used to remove all the elements from a text listbox.
 *
 * \param widget Text listbox widget variable.
 *
 * \return None.
 */
void pw_tlistbox_clear(pw_widget widget);

/*!\fn void pw_tlistbox_change(pw_widget widget, char *text, unsigned rownum, unsigned colnum)
 * \brief This function is used to change the contents of an existing row and column entry of a text listbox.
 *
 * \param widget Text listbox widget variable.
 * \param text Text string update.
 * \param rownum Row number.
 * \param colnum Column number.
 *
 * \return None.
 */
void pw_tlistbox_change(pw_widget widget, char *text, unsigned rownum, unsigned colnum);

/*!\fn int pw_tlistbox_get_selection(pw_widget widget)
 * \brief This function is used to get the selected element row number in a text listbox.
 *
 * \param widget Text listbox widget variable.
 *
 * \return Number of the selected row.
 */
int pw_tlistbox_get_selection(pw_widget widget);

/*!\fn int pw_tlistbox_get_multiple_selection(pw_widget widget, int **selection_index)
 * \brief This function is used to get the list of selected elements.
 *
 * \param widget Text listbox widget variable.
 * \param selection_index Address of pointer variable, in which the selection will be stored. The calling function
 * should free this variable after the use.
 *
 * \return Number of elements selected.
 */
int pw_tlistbox_get_multiple_selection(pw_widget widget, int **selection_index);

/*!\fn void pw_tlistbox_set_selection(pw_widget widget, unsigned rownum)
 * \brief This function is used to set a selection on an element.
 *
 * \param widget Text listbox widget variable.
 * \param rownum Row number to be selected.
 *
 * \return None.
 */
void pw_tlistbox_set_selection(pw_widget widget, unsigned rownum);

/*!\fn void pw_tlistbox_remove_selection(pw_widget widget, unsigned rownum)
 * \brief This function is used to remove a selection on an element.
 *
 * \param widget Text listbox widget variable.
 * \param rownum Row number to remove selection.
 *
 * \return None.
 */
void pw_tlistbox_remove_selection(pw_widget widget, unsigned rownum);

/*!\fn void pw_tlistbox_remove_selection_all(pw_widget widget)
 * \brief This function is used to remove all selections in a text listbox.
 *
 * \param widget Text listbox widget variable.
 *
 * \return None.
 */
void pw_tlistbox_remove_selection_all(pw_widget widget);

/*!\fn void pw_tlistbox_get_element(pw_widget widget, char **text, unsigned rownum, unsigned colnum)
 * \brief This function is used to get the element at the specified row and column of a text listbox.
 *
 * \param widget Text listbox widget variable.
 * \param text Address of a character pointer variable, where the element will be stored. The calling function should free this
 * after the use.
 * \param rownum Row number.
 * \param colnum Column number.
 *
 * \return None.
 */
void pw_tlistbox_get_element(pw_widget widget, char **text, unsigned rownum, unsigned colnum);

/*!\fn int pw_tlistbox_get_numrows(pw_widget widget)
 * \brief This function is used to get the number of rows in a text listbox.
 *
 * \param widget Text listbox widget variable.
 *
 * \return Number of text listbox elements.
 */
int pw_tlistbox_get_numrows(pw_widget widget);

/*!\fn void pw_tlistbox_see(pw_widget widget, unsigned rownum)
 * \brief This function is used to bring a specified row to view, if it is scrolled away.
 *
 * \param rownum Row number to be brought into view.
 *
 * \return None.
 */
void pw_tlistbox_see(pw_widget widget, unsigned rownum);

/*!\fn void pw_tlistbox_set_selection_type(pw_widget widget, unsigned selection_type)
 * \brief This function is used to make the text listbox make multiple selections or not.
 *
 * \param widget Text listbox widget variable.
 * \param selection_type Either of the above mentioned PW_TLISTBOX_* defines.
 *
 * \return None.
 */
void pw_tlistbox_set_selection_type(pw_widget widget, unsigned selection_type);

/*!\fn void pw_tlistbox_selection_intimate(pw_widget widget, void (*change_fn)(pw_widget widget, int rownum));
 * \brief This function is used to setup a callback function for a text listbox element selection.
 *
 * \param widget Text listbox widget variable.
 * \param change_fn Pointer to the callback function. This function will be called with the row number selected.
 *
 * \return None.
 */
void pw_tlistbox_selection_intimate(pw_widget widget, void (*change_fn)(pw_widget widget, int rownum));

/* [PROGRESS BAR] */

/*!\fn pw_widget pw_create_progressbar(	pw_widget parent, int x, int y, int width, int height, ...);
 * \brief This is used to create a progressbar widget. The "progress_bar" program in the examples
 * shows the use of it.
 *
 * \param parent Parent widget variable.
 * \param x X co-ordinate, where the widget will be placed.
 * \param y Y co-ordinate, where the widget will be placed.
 * \param width Width of the widget.
 * \param height Height of the widget.
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return Progress bar widget variable.
 */
pw_widget pw_create_progressbar(pw_widget parent, int x, int y, int width, int height, ...);

/*!\fn void pw_progressbar_setvalue(pw_widget widget, unsigned value)
 * \brief This function is used to set the amount of progress in the progress bar.
 *
 * \param widget Progress bar widget variable.
 * \param value A value between 1-100.
 *
 * \return None.
 */
void pw_progressbar_setvalue(pw_widget widget, unsigned value);

/*!\fn void pw_progressbar_setcolor(pw_widget widget, const char *color)
 * \brief This function is used to set the progress bar color.
 *
 * \param widget Progress bar widget variable.
 * \param color Color name or the string "#rrggbb".
 *
 * \return None.
 */
void pw_progressbar_setcolor(pw_widget widget, const char *color);

/*!\fn void pw_progressbar_showvalue(pw_widget widget)
 * \brief This function is used to enable the progress value display.
 *
 * \param widget Progress bar widget variable.
 *
 * \return None.
 */
void pw_progressbar_showvalue(pw_widget widget);

/*!\fn void pw_progressbar_hidevalue(pw_widget widget)
 * \brief This function is used to disable the progress value display.
 *
 * \param widget Progress bar widget variable.
 *
 * \return None.
 */
void pw_progressbar_hidevalue(pw_widget widget);

/* [COMBO BOX] */

/*!\fn pw_widget pw_create_combobox(pw_widget parent, int x, int y, int width, int height, ...);
 * \brief This is used to create a combobox widget. The "combobox_demo" program in the examples
 * shows the use of it.
 *
 * \param parent Parent widget variable.
 * \param x X co-ordinate, where the widget will be placed.
 * \param y Y co-ordinate, where the widget will be placed.
 * \param width Width of the widget.
 * \param height Height of the widget.
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return Combobox widget variable.
 */
pw_widget pw_create_combobox(pw_widget parent, int x, int y, int width, int height, ...);

/*!\fn int pw_combobox_insert(pw_widget widget, char *text, Bool refresh_fg)
 * \brief This function is used to insert an element into a combobox.
 *
 * \param widget Combobox widget variable.
 * \param text Text string to be inserted.
 * \param refresh_fg True if the combobox display needs to be updated or False otherwise. There are situations
 * where several entries needs to be added. In such a case it is ideal to set this flag to False for all the
 * elements, except for the last one.
 *
 * \return Value can be ignored.
 */
int pw_combobox_insert(pw_widget widget, char *text, Bool refresh_fg);

/*!\fn int pw_combobox_remove(pw_widget widget, unsigned index)
 * \brief This function is used to remove an element at the specified index.
 *
 * \param widget Combobox widget variable.
 * \param index Index of the element to be removed.
 *
 * \return Value can be ignored.
 */
int pw_combobox_remove(pw_widget widget, unsigned index);

/*!\fn int pw_combobox_clear(pw_widget widget)
 * \brief This function is used to remove all the elements of a combobox.
 *
 * \param widget Combobox widget variable.
 *
 * \return Value can be ignored.
 */
int pw_combobox_clear(pw_widget widget);

/*!\fn void pw_combobox_get_selection(pw_widget widget, char **text, unsigned *index)
 * \brief This function is used to get the current combobox selection.
 *
 * \param widget Combobox widget variable.
 * \param text Address of a character pointer, where the selected item will be filled. The calling function should free this after the use.
 * \param index Address of an integer variable, where the selection index will be stored.
 *
 * \return None.
 */
void pw_combobox_get_selection(pw_widget widget, char **text, unsigned *index);

/*!\fn int pw_combobox_get_index(pw_widget widget, char *text)
 * \brief This function is used to get the index of a combobox element.
 *
 * \param widget Combobox widget variable.
 * \param text the text for which the index has to be found.
 *
 * \return if the text is found returns the index of the text otherwise returns -1.
 */
int pw_combobox_get_index(pw_widget widget, char *text);

/*!\fn void pw_combobox_set_selection(pw_widget widget, unsigned index)
 * \brief This function is used to select a combobox element.
 *
 * \param widget Combobox widget variable.
 * \param index Index of the element to be selected.
 *
 * \return None.
 */
void pw_combobox_set_selection(pw_widget widget, unsigned index);

/*!\fn void pw_combobox_selection_intimate(pw_widget widget,
 * 								void (*change_fn)(pw_widget widget, int index));
 * \brief This function is used to set a callback function to intimate selection.
 *
 * \param widget Combobox widget variable.
 * \param change_fn Pointer to callback function. This function will be called with the index of the selected element.
 *
 * \return None.
 */
void pw_combobox_selection_intimate(pw_widget widget, void (*change_fn)(pw_widget widget, int index));

/* [TAB] */

/*!\fn pw_widget pw_create_tab(	pw_widget parent,
 *                                    int x, int y, int width, int height, ...);
 * \brief This is used to create a tab widget. The "tab_example" program in the examples
 * shows the use of it.
 *
 * \param parent Parent widget variable.
 * \param x X co-ordinate, where the widget will be placed.
 * \param y Y co-ordinate, where the widget will be placed.
 * \param width Width of the widget.
 * \param height Height of the widget.
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return Tab widget variable.
 */
pw_widget pw_create_tab(pw_widget parent, int x, int y, int width, int height, ...);

/*!\fn pw_widget pw_tab_add_page(pw_widget widget, const char *title, Bool(*tab_callback)(pw_widget, pw_event))
 * \brief This function is used to add a page to a tab.
 *
 * \param widget Tab widget variable.
 * \param title Tab title text.
 * \param tab_callback Callback function for tab selection.
 *
 * \return Returns a pw_widget pointer to the new page.
 */
pw_widget pw_tab_add_page(pw_widget widget, const char *title, Bool(*tab_callback)(pw_widget, pw_event));

/*!\fn void pw_tab_raise_page(pw_widget widget, unsigned tabnum)
 * \brief This function is used to raise a particular tab.
 *
 * \param widget Tab widget variable.
 * \param tabnum Number of the tab to be raised.
 *
 * \return None.
 */
void pw_tab_raise_page(pw_widget widget, unsigned tabnum);

/*!\fn pw_widget pw_tab_get_page(pw_widget widget, unsigned tabnum)
 * \brief This function is used to get at particular page in a tab.
 *
 * \param widget Tab widget variable.
 * \param tabnum Number of the tab, whose page is needed.
 *
 * \return Page widget.
 */
pw_widget pw_tab_get_page(pw_widget widget, unsigned tabnum);

/*!\fn pw_widget pw_tab_get_active_page(pw_widget widget)
 * \brief This function is used to get a pointer to the active page in a tab.
 *
 * \param widget Tab widget variable.
 *
 * \return Page widget.
 */
pw_widget pw_tab_get_active_page(pw_widget widget);

/* [TEXT BOX] */

/*!\fn pw_widget pw_create_textbox(pw_widget parent, int x, int y, int width, int height, ...);
 * \brief This is used to create a textbox widget. The "textbox_demo" program in the examples
 * shows the use of it.
 *
 * \param parent Parent widget variable.
 * \param x X co-ordinate, where the widget will be placed.
 * \param y Y co-ordinate, where the widget will be placed.
 * \param width Width of the widget.
 * \param height Height of the widget.
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return Textbox widget variable.
 */
pw_widget pw_create_textbox(pw_widget parent, int x, int y, int width, int height, ...);

/*!\fn void pw_textbox_loadfile(pw_widget widget, const char *filename)
 * \brief This function is used to load a text file into the textbox widget.
 *
 * \param widget Textbox widget variable.
 * \param filename Name of the text file to be loaded.
 *
 * \return None.
 */
void pw_textbox_loadfile(pw_widget widget, const char *filename);

/*!\fn void pw_textbox_savefile(pw_widget widget, const char *filename)
 * \brief This function is used to save the typed contents in textbox into a file.
 *
 * \param widget Textbox widget variable.
 * \param filename Name of the text file to be saved.
 *
 * \return None.
 */
void pw_textbox_savefile(pw_widget widget, const char *filename);

/*!\fn int pw_textbox_get_numlines(pw_widget widget)
 * \brief This function is used to get the number of lines entered into the textbox widget.
 *
 * \param widget Textbox widget variable.
 *
 * \return Number of lines entered.
 */
int pw_textbox_get_numlines(pw_widget widget);

/*!\fn const char *pw_textbox_get_line(pw_widget widget, unsigned linenum)
 * \brief This function is used to get a line at a particular line number.
 *
 * \param widget Textbox widget variable.
 * \param linenum Line number of the required line.
 *
 * \return Line text string.
 */
const char *pw_textbox_get_line(pw_widget widget, unsigned linenum);

/*!\fn void pw_textbox_clear(pw_widget widget)
 * \brief This function is used to clear the contents of a textbox.
 *
 * \param widget Textbox widget variable.
 *
 * \return None
 */
void pw_textbox_clear(pw_widget widget);


/* [SPINNER] */

/*!\fn pw_widget pw_create_spinner(pw_widget parent,
 *                                    int x, int y, int width, int height, ...);
 * \brief This is used to create a spinner widget. The "spinner_demo" program in the examples
 * shows the use of it.
 *
 * \param parent Parent widget variable.
 * \param x X co-ordinate, where the widget will be placed.
 * \param y Y co-ordinate, where the widget will be placed.
 * \param width Width of the widget.
 * \param height Height of the widget.
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return Spinner widget variable.
 */
pw_widget pw_create_spinner(pw_widget parent, int x, int y, int width, int height, ...);

/*!\fn void pw_spinner_set_limits(pw_widget widget, int min, int max)
 * \brief This function is used to set the spinner limit.
 *
 * \param widget Spinner widget variable.
 * \param min Minimum value.
 * \param max Maximum value.
 *
 * \return None.
 */
void pw_spinner_set_limits(pw_widget widget, int min, int max);

/*!\fn void pw_spinner_set_label(pw_widget widget, const char *label)
 * \brief This function is used to set the spinner label.
 *
 * \param widget Spinner widget variable.
 * \param label Text string.
 *
 * \return None.
 */
void pw_spinner_set_label(pw_widget widget, const char *label);

/*!\fn void pw_spinner_set_value(pw_widget widget, int value)
 * \brief This function is used to set the spinner value.
 *
 * \param widget Spinner widget variable.
 * \param value Interger value within the range, specified using pw_spinner_set_limits() function.
 *
 * \return None.
 */
void pw_spinner_set_value(pw_widget widget, int value);

/*!\fn int pw_spinner_get_value(pw_widget widget)
 * \brief This function is used to get the current spinner value.
 *
 * \param widget Spinner widget variable.
 *
 * \return Spinner value.
 */
int pw_spinner_get_value(pw_widget widget);

/* [SKETCH] */

/*!\fn pw_widget pw_create_sketch(pw_widget parent, int x, int y, int width, int height, ...);
 * \brief This is used to create a sketch widget. The "tab_example" program in the examples has
 * a page, which shows the use of the sketch widget.
 *
 * \param parent Parent widget variable.
 * \param x X co-ordinate, where the widget will be placed.
 * \param y Y co-ordinate, where the widget will be placed.
 * \param width Width of the widget.
 * \param height Height of the widget.
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return Sketch widget variable.
 */
pw_widget pw_create_sketch(pw_widget parent, int x, int y, int width, int height, ...);

/*!\fn void pw_sketch_loadfile(pw_widget widget, const char *filename)
 * \brief This function is used to load a sketch from a file.
 *
 * \param widget Sketch widget variable.
 * \param filename Name of the sketch file to be loaded.
 * 
 * \return None.
 */
void pw_sketch_loadfile(pw_widget widget, const char *filename);

/*!\fn void pw_sketch_savefile(pw_widget widget, const char *filename)
 * \brief This function is used to save the current sketch into a file.
 *
 * \param widget Sketch widget variable.
 * \param filename Name of the sketch file to be saved.
 *
 * \return None.
 */
void pw_sketch_savefile(pw_widget widget, const char *filename);

/*!\fn void pw_sketch_pencolor(pw_widget widget, const char *color)
 * \brief This function is used to set the sketch pen color.
 *
 * \param widget Sketch widget variable.
 * \param color Color name or the color string of "#rrggbb" format.
 *
 * \return None.
 */
void pw_sketch_pencolor(pw_widget widget, const char *color);

/*!\fn void pw_sketch_penwidth(pw_widget widget, unsigned width)
 * \brief This function is used to set the sketch pen width.
 *
 * \param widget Sketch widget variable.
 * \param width Width of the pen in pixels. Should be a value between 1-8.
 *
 * \return None.
 */
void pw_sketch_penwidth(pw_widget widget, unsigned width);

/*!\fn void pw_sketch_clear(pw_widget widget)
 * \brief This function is used to clear the sketch.
 *
 * \param widget Sketch widget variable.
 *
 * \return None.
 */
void pw_sketch_clear(pw_widget widget);

/*!\fn void pw_sketch_save_as_image(pw_widget widget, char *filename, int format)
 * \brief This function is used to the sketch area as image file.
 *
 * \param widget Sketch widget variable.
 * \param filename filename of the file to save
 * \param format of the image. See list of PW_IMAGE_FORMAT_* for possible types
 *
 * \return Less than zero if failed.
 */
int pw_sketch_save_as_image(pw_widget widget, char *filename, int format);

/* [VERTICAL SCROLLBAR] */

/*!\fn pw_widget pw_create_vscrollbar(pw_widget parent, int x, int y, int width, int height, ...);
 * \brief This is used to create a vertical scrollbar widget. The "scrollbar_demo" program in the examples
 * shows the use of it.
 *
 * \param parent Parent widget variable.
 * \param x X co-ordinate, where the widget will be placed.
 * \param y Y co-ordinate, where the widget will be placed.
 * \param width Width of the widget.
 * \param height Height of the widget.
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return Vertical scrollbar widget variable.
 */
pw_widget pw_create_vscrollbar(pw_widget parent, int x, int y, int width, int height, ...);

/*!\fn void pw_vscrollbar_set_sliderpos(pw_widget widget, double position)
 * \brief This function is used to set the vertical scrollbar's position.
 *
 * \param widget Vertical scrollbar widget variable.
 * \param position A value within the range 0.0 to 1.0.
 * 
 * \return None.
 */
void pw_vscrollbar_set_sliderpos(pw_widget widget, double position);

/*!\fn void pw_vscrollbar_set_sliderlen(pw_widget widget, double length)
 * \brief This function is used to set the length of the scrollbar slider. The length is equivalent to scroll amount.
 *
 * \param widget Vertical scrollbar widget variable.
 * \param length A value within the range 0.0 to 1.0.
 *
 * \return None.
 */
void pw_vscrollbar_set_sliderlen(pw_widget widget, double length);

/*!\fn void pw_vscrollbar_set_sliderincr(pw_widget widget, double increment)
 * \brief This function is used to set automatic scroll increment for the scrollbar. A scrollbar is incremented
 * automatically, when the up or down buttons are pressed.
 *
 * \param widget Vertical scrollbar widget variable.
 * \param increment A value within the range 0.0 to 1.0.
 *
 * \return None
 */
void pw_vscrollbar_set_sliderincr(pw_widget widget, double increment);

/*!\fn void pw_vscrollbar_intimate_change(pw_widget widget, void (*callback)(pw_widget widget, double position))
 * \brief This function is used to set a callback for scroll area change.
 *
 * \param widget Vertical scrollbar widget variable.
 * \param callback Callback function to be called on moving the scrollbar. The function is passed with the current
 * position of the scrollbar.
 *
 * \return None.
 */
void pw_vscrollbar_intimate_change(pw_widget widget, void (*callback)(pw_widget widget, double position));

/*!\fn double pw_vscrollbar_get_sliderpos(pw_widget widget)
 * \brief This function is used to get the current slider position of the scrollbar.
 *
 * \param widget Vertical scrollbar widget variable.
 *
 * \return Current postion of the scrollbar. A value within the range 0.0 to 1.0.
 */
double pw_vscrollbar_get_sliderpos(pw_widget widget);

/*!\fn double pw_vscrollbar_get_sliderincr(pw_widget widget)
 * \brief This function is used to get the slider increment value.
 *
 * \param widget Vertical scrollbar widget variable.
 *
 * \return Slider increment value. A value within the range 0.0 to 1.0.
 */
double pw_vscrollbar_get_sliderincr(pw_widget widget);

/* [HORIZONTAL SCROLLBAR] */

/*!\fn pw_widget pw_create_hscrollbar(pw_widget parent, int x, int y, int width, int height, ...);
 * \brief This is used to create a horizontal scrollbar widget. The "scrollbar_demo" program in the examples
 * shows the use of it.
 *
 * \param parent Parent widget variable.
 * \param x X co-ordinate, where the widget will be placed.
 * \param y Y co-ordinate, where the widget will be placed.
 * \param width Width of the widget.
 * \param height Height of the widget.
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return Horizontal scrollbar widget variable.
 */
pw_widget pw_create_hscrollbar(pw_widget parent, int x, int y, int width, int height, ...);

/*!\fn void pw_hscrollbar_set_sliderpos(pw_widget widget, double position)
 * \brief This function is used to set the horizontal scrollbar's position.
 *
 * \param widget Horizontal scrollbar widget variable.
 * \param position A value within the range 0.0 to 1.0.
 * 
 * \return None.
 */
void pw_hscrollbar_set_sliderpos(pw_widget widget, double position);

/*!\fn void pw_hscrollbar_set_sliderlen(pw_widget widget, double length)
 * \brief This function is used to set the length of the scrollbar slider. The length is equivalent to scroll amount.
 *
 * \param widget Horizontal scrollbar widget variable.
 * \param length A value within the range 0.0 to 1.0.
 *
 * \return None.
 */
void pw_hscrollbar_set_sliderlen(pw_widget widget, double length);

/*!\fn void pw_hscrollbar_set_sliderincr(pw_widget widget, double increment)
 * \brief This function is used to set automatic scroll increment for the scrollbar. A scrollbar is incremented
 * automatically, when the up or down buttons are pressed.
 *
 * \param widget Horizontal scrollbar widget variable.
 * \param increment A value within the range 0.0 to 1.0.
 *
 * \return None
 */
void pw_hscrollbar_set_sliderincr(pw_widget widget, double increment);

/*!\fn void pw_hscrollbar_intimate_change(pw_widget widget, void (*callback)(pw_widget widget, double position))
 * \brief This function is used to set a callback for scroll area change.
 *
 * \param widget Horizontal scrollbar widget variable.
 * \param callback Callback function to be called on moving the scrollbar. The function is passed with the current
 * position of the scrollbar.
 *
 * \return None.
 */
void pw_hscrollbar_intimate_change(pw_widget widget, void (*callback)(pw_widget widget, double position));

/*!\fn double pw_hscrollbar_get_sliderpos(pw_widget widget)
 * \brief This function is used to get the current slider position of the scrollbar.
 *
 * \param widget Horizontal scrollbar widget variable.
 *
 * \return Current postion of the scrollbar. A value within the range 0.0 to 1.0.
 */
double pw_hscrollbar_get_sliderpos(pw_widget widget);

/*!\fn double pw_hscrollbar_get_sliderincr(pw_widget widget)
 * \brief This function is used to get the slider increment value.
 *
 * \param widget Horizontal scrollbar widget variable.
 *
 * \return Slider increment value. A value within the range 0.0 to 1.0.
 */
double pw_hscrollbar_get_sliderincr(pw_widget widget);

/* [VERTICAL SLIDER] */

/*!\fn pw_widget pw_create_vslider(pw_widget parent, int x, int y, int width, int height, ...);
 * \brief This is used to create a vertical slider widget.
 *
 * \param parent Parent widget variable.
 * \param x X co-ordinate, where the widget will be placed.
 * \param y Y co-ordinate, where the widget will be placed.
 * \param width Width of the widget.
 * \param height Height of the widget.
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return Vertical slider widget variable.
 */
pw_widget pw_create_vslider(pw_widget parent, int x, int y, int width, int height, ...);

/*!\fn void pw_vslider_set_range(pw_widget widget, int min, int max)
 * \brief This function is used to set the range of the slider.
 *
 * \param widget Vertical slider widget variable.
 * \param min Minimum value of the slider.
 * \param max Maximum value of the slider.
 *
 * \return None.
 */
void pw_vslider_set_range(pw_widget widget, int min, int max);

/*!\fn void pw_vslider_set_position(pw_widget widget, int position)
 * \brief This function is used to set the slider position.
 *
 * \param widget Vertical slider widget variable.
 * \param position A value within the range set by the function pw_vslider_set_range().
 *
 * \return None.
 */
void pw_vslider_set_position(pw_widget widget, int position);

/*!\fn int pw_vslider_get_position(pw_widget widget, int position)
 * \brief This function is used to get the current slider position.
 *
 * \param widget Vertical slider widget variable.
 * \param position Can pass any value. This arguement is ignored.
 *
 * \return Current slider position.
 */
int pw_vslider_get_position(pw_widget widget, int position);

/*!\fn void pw_vslider_show_value(pw_widget widget, unsigned calibration_side)
 * \brief This function is used to display the slider position.
 *
 * \param widget Vertical slider widget variable.
 * \param calibration_side Location of the slider value to be displayed. Any one of the PW_SLIDER_* defines.
 *
 * \return None.
 */
void pw_vslider_show_value(pw_widget widget, unsigned calibration_side);

/*!\fn void pw_vslider_hide_value(pw_widget widget)
 * \brief This function is used to hide the slider value display.
 *
 * \param widget Vertical slider widget variable.
 *
 * \return None.
 */
void pw_vslider_hide_value(pw_widget widget);

/*!\fn void pw_vslider_intimate_release(pw_widget widget, void (*callback)(int position))
 * \brief This function is used to set a callback for slider release.
 *
 * \param widget Vertical slider widget variable.
 * \param callback Pointer to callback function, which will be called on slider release. The callback will be passed with
 * the slider poition during release.
 *
 * \return None.
 */
void pw_vslider_intimate_release(pw_widget widget, void (*callback)(int position));

/*!\fn void pw_vslider_intimate_motion(pw_widget widget, void (*callback)(int position))
 * \brief This function is used to set a callback for slider motion.
 *
 * \param widget Vertical slider widget variable.
 * \param callback Pointer to callback function, which will be called on slider motion. The callback will be passed with
 * the slider poition during motion.
 *
 * \return None.
 */
void pw_vslider_intimate_motion(pw_widget widget, void (*callback)(int position));

/* [HORIZONTAL SLIDER] */

/*!\fn pw_widget pw_create_hslider(pw_widget parent, int x, int y, int width, int height, ...);
 * \brief This is used to create a Horizontal slider widget.
 *
 * \param parent Parent widget variable.
 * \param x X co-ordinate, where the widget will be placed.
 * \param y Y co-ordinate, where the widget will be placed.
 * \param width Width of the widget.
 * \param height Height of the widget.
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return Horizontal slider widget variable.
 */
pw_widget pw_create_hslider(pw_widget parent, int x, int y, int width, int height, ...);

/*!\fn void pw_hslider_set_range(pw_widget widget, int min, int max)
 * \brief This function is used to set the range of the slider.
 *
 * \param widget Horizontal slider widget variable.
 * \param min Minimum value of the slider.
 * \param max Maximum value of the slider.
 *
 * \return None.
 */
void pw_hslider_set_range(pw_widget widget, int min, int max);

/*!\fn void pw_hslider_set_position(pw_widget widget, int position)
 * \brief This function is used to set the slider position.
 *
 * \param widget Horizontal slider widget variable.
 * \param position A value within the range set by the function pw_hslider_set_range().
 *
 * \return None.
 */
void pw_hslider_set_position(pw_widget widget, int position);

/*!\fn int pw_hslider_get_position(pw_widget widget, int position)
 * \brief This function is used to get the current slider position.
 *
 * \param widget Horizontal slider widget variable.
 * \param position Can pass any value. This arguement is ignored.
 *
 * \return Current slider position.
 */
int pw_hslider_get_position(pw_widget widget, int position);

/*!\fn void pw_hslider_show_value(pw_widget widget, unsigned calibration_side)
 * \brief This function is used to display the slider position.
 *
 * \param widget Horizontal slider widget variable.
 * \param calibration_side Location of the slider value to be displayed. Any one of the PW_SLIDER_* defines.
 *
 * \return None.
 */
void pw_hslider_show_value(pw_widget widget, unsigned calibration_side);

/*!\fn void pw_hslider_hide_value(pw_widget widget)
 * \brief This function is used to hide the slider value display.
 *
 * \param widget Horizontal slider widget variable.
 *
 * \return None.
 */
void pw_hslider_hide_value(pw_widget widget);

/*!\fn void pw_hslider_intimate_release(pw_widget widget, void (*callback)(int position))
 * \brief This function is used to set a callback for slider release.
 *
 * \param widget Horizontal slider widget variable.
 * \param callback Pointer to callback function, which will be called on slider release. The callback will be passed with
 * the slider poition during release.
 *
 * \return None.
 */
void pw_hslider_intimate_release(pw_widget widget, void (*callback)(int position));

/*!\fn void pw_hslider_intimate_motion(pw_widget widget, void (*callback)(int position))
 * \brief This function is used to set a callback for slider motion.
 *
 * \param widget Horizontal slider widget variable.
 * \param callback Pointer to callback function, which will be called on slider motion. The callback will be passed with
 * the slider poition during motion.
 *
 * \return None.
 */
void pw_hslider_intimate_motion(pw_widget widget, void (*callback)(int position));

/* [Monitor FD] */

/*!\fn pw_fd pw_add_monitor_fd(int fd, pw_fdtype type, void (*callback)(void *arg1, void *arg2), void *arg1, void *arg2)
 * \brief This function is used to add file descriptor to the list of monitored file descriptors in the eventloop
 *
 * \param fd  The file descriptor
 * \param type The type of monitoring to be done (PW_FD_READ or PW_FD_WRITE)
 * \param callback Pointer to a function to be called when the fd is ready for read/write
 * \param arg1 Pointer to an argument passed to the callback function. This will be available as the first argument in the callback function.
 * \param arg2 Pointer to an argument passed to the callback function. This will be available as the second argument in the callback function.
 *
 * \return pw_fd type variable.
 */
pw_fd pw_add_monitor_fd(int fd, pw_fdtype type, void (*callback)(void *arg1, void *arg2), void *arg1, void *arg2);

/*!\fn pw_remove_monitor_fd(pw_fd fd)
 * \brief This function is used to remove a pw_fd object from the list of monitored fds
 *
 * \param fd The pw_fd type variable returned by pw_add_monitor_fd 
 * 
 * \return None.
 */
void pw_remove_monitor_fd(pw_fd fd);

/* [CALENDAR] */

/*!\fn int pw_create_calendar(pw_widget parent, int x, int y, int *day, int *month, int *year, ... ); 
 *
 * \brief This is used to create a calendar widget.
 *
 * \param parent Parent widget variable.
 * \param x X co-ordinate, where the widget will be placed.
 * \param y Y co-ordinate, where the widget will be placed.
 * \param *day Pointer to integer holding day of the month, if 0 current date will be used
 * \param *month Pointer to integer holding month, if 0 current month will be used
 * \param *year Pointer to integer holding year, if 0 current year will be used
 * \param ... List of attributes ended by a PW_NA.
 *
 * \return Integer (0-6) - (Sunday to Saturday) indicating day of the week of selected date.
 */
int pw_create_calendar(pw_widget parent, int x, int y, int *day, int *month, int *year, ... );

/* [ FUNCTIONS IN PWRAP NOT PRESENT IN PWRAP2 ] */


/*!\fn pw_pixmap pw_create_pixmap_from_file(const char *file, ...)
 * \brief This function is used to create image pixmap from a file. Pixmaps can be used to display images. The
 * "pixmap_demo" program in the examples shows the use of it. Deprecated function not implemented in pwrap2
 *
 * \param file Image file path. The path can take a format string. For example, pw_create_pixmap_from_file("%s/%s", "/tmp", "img.jpg") is possible.
 *
 * \return pw_pixmap type variable.
 */
pw_pixmap pw_create_pixmap_from_file(const char *file, ...);

/*!\fn void pw_destroy_pixmap(pw_pixmap pixmap)
 * \brief This function is used to destroy a pixmap. Deprecated function not implemented in pwrap2
 *
 * \param pixmap Pixmap variable.
 *
 * \return None.
 */
void pw_destroy_pixmap(pw_pixmap pixmap);

/*!\fn int pw_get_pixmap_width(pw_pixmap pixmap)
 * \brief This function returns the width of a pixmap. Deperecated function not implemented in pwrap2
 *
 * \param pixmap Pixmap variable.
 *
 * \return Width of the pixmap.
 */

int pw_get_pixmap_width(pw_pixmap pixmap);

/*!\fn int pw_get_pixmap_height(pw_pixmap pixmap)
 * \brief This function returns the height of a pixmap. Deprecated function not implemented in pwrap2
 *
 * \param pixmap Pixmap variable.
 *
 * \return Height of the pixmap.
 */

int pw_get_pixmap_height(pw_pixmap pixmap);

/*!\fn void pw_draw_pixmap(pw_pixmap src_pixmap, pw_widget dest_widget, int src_x, int src_y, int width, int height, int dest_x, int dest_y)
 * \brief This function draws pixmap on a widget's surface. Deprecated function not implemented in pwrap2
 *
 * \param src_pixmap Pixmap to be displayed.
 * \param dest_widget Widget on which the pixmap has to be displayed.
 * \param src_x X position on the pixmap, from where it should be copied.
 * \param src_y Y position on the pixmap, from where it should be copied.
 * \param width Width of the pixmap region to be copied.
 * \param height Height of the pixmap region to be copied.
 * \param dest_x X position on the widget, where the pixmap will be copied.
 * \param dest_y Y position on the widget, where the pixmap will be copied.
 *
 * \return None.
 */

void pw_draw_pixmap(pw_pixmap src_pixmap, pw_widget dest_widget,  
				int src_x, int src_y, int width, int height, int dest_x, int dest_y);

#if 0
void pw_frame_set_raw_img(pw_widget widget, char **img_data, int width, int height);
void pw_frame_unset_ximg(pw_widget);
#endif 
#ifdef __cplusplus
}
#endif

#endif
