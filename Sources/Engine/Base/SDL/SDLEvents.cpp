/* Copyright (c) 2002-2012 Croteam Ltd. All rights reserved. */

#include <stdlib.h>

#include <Engine/Base/Types.h>
#include <Engine/Base/Assert.h>
#include <Engine/Base/SDL/SDLEvents.h>
#include "SDL.h"

SDL_EventType WM_SYSKEYDOWN;
SDL_EventType WM_LBUTTONDOWN;
SDL_EventType WM_LBUTTONUP;
SDL_EventType WM_RBUTTONDOWN;
SDL_EventType WM_RBUTTONUP;
SDL_EventType WM_PAINT;

// This keeps the input subsystem in sync with everything else, by
//  making sure all SDL events tunnel through one function.
extern int SE_SDL_InputEventPoll(SDL_Event *event);

#ifdef PLATFORM_SWITCH
#include <switch.h>

// this is only used in GUI stuff, so joystick emulates keyboard keys

static const int joy2key[] = {
  /*[KEY_A]      =*/ VK_RETURN,
  /*[KEY_B]      =*/ SDLK_BACKSPACE,
  /*[KEY_X]      =*/ VK_DELETE,
  /*[KEY_Y]      =*/ VK_SPACE,
                     -1,
                     -1,
  /*[KEY_L]      =*/ VK_PRIOR,
  /*[KEY_R]      =*/ VK_NEXT,
  /*[KEY_ZL]     =*/ VK_LSHIFT,
  /*[KEY_ZR]     =*/ VK_RSHIFT,
  /*[KEY_PLUS]   =*/ VK_ESCAPE,
  /*[KEY_MINUS]  =*/ SDLK_BACKQUOTE,
  /*[KEY_DLEFT]  =*/ VK_LEFT,
  /*[KEY_DUP]    =*/ VK_UP,
  /*[KEY_DRIGHT] =*/ VK_RIGHT,
  /*[KEY_DDOWN]  =*/ VK_DOWN,
};

#define NUMJOYBINDS (sizeof(joy2key) / sizeof(joy2key[0]))

static int joystate[NUMJOYBINDS] = { 0 };

static inline int SE_SDL_JoyToKey(int button)
{
    if (button >= 0 && button < NUMJOYBINDS)
        return joy2key[button];
    return -1;
}

static inline int SE_SDL_KeyToJoy(int key)
{
    for (int i = 0; i < NUMJOYBINDS; ++i)
        if (joy2key[i] == key)
            return i;
    return -1;
}

#endif

// !!! FIXME: maybe not try to emulate win32 here?
BOOL PeekMessage(MSG *msg, void *hwnd, UINT wMsgFilterMin,
                UINT wMsgFilterMax, UINT wRemoveMsg)
{
    ASSERT(msg != NULL);
    ASSERT(wRemoveMsg == PM_REMOVE);
    ASSERT(wMsgFilterMin == 0);
    ASSERT(wMsgFilterMax == 0);

    SDL_Event sdlevent;
    while (SE_SDL_InputEventPoll(&sdlevent))
    {
        SDL_zerop(msg);
        msg->message = sdlevent.type;

        switch (sdlevent.type)
        {
            case SDL_MOUSEMOTION:
                msg->lParam = (
                                ((sdlevent.motion.y << 16) & 0xFFFF0000) |
                                ((sdlevent.motion.x      ) & 0x0000FFFF)
                              );
                return TRUE;

            case SDL_KEYDOWN:
                if (sdlevent.key.keysym.mod & KMOD_ALT)
                    msg->message = WM_SYSKEYDOWN;
                // deliberate fall through...
            case SDL_KEYUP:
                if (sdlevent.key.keysym.sym == SDLK_BACKQUOTE)
                    msg->unicode = '~';  // !!! FIXME: this is all a hack.
                #ifdef PLATFORM_PANDORA
                if(sdlevent.key.keysym.sym == SDLK_RCTRL) {
                    msg->message = (sdlevent.type==SDL_KEYDOWN)?WM_RBUTTONDOWN:WM_RBUTTONUP;
                } else if(sdlevent.key.keysym.sym == SDLK_RSHIFT) {
                    msg->message = (sdlevent.type==SDL_KEYDOWN)?WM_LBUTTONDOWN:WM_LBUTTONUP;
                } else
                #endif
                msg->wParam = sdlevent.key.keysym.sym;
                return TRUE;

            case SDL_TEXTINPUT:
                msg->wParam = sdlevent.text.text[0];  // !!! FIXME: dropping characters!
                return TRUE;

            case SDL_MOUSEWHEEL:
                if (sdlevent.wheel.y > 0)  // wheel up
                    msg->wParam = 120 << 16;  // !!! FIXME: deal with actual y value.
                else if (sdlevent.wheel.y < 0)  // wheel down
                    msg->wParam = -(120 << 16);  // !!! FIXME: deal with actual y value.
                return TRUE;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                if (sdlevent.button.button == SDL_BUTTON_LEFT)
                    msg->message = (sdlevent.button.state == SDL_PRESSED) ? WM_LBUTTONDOWN : WM_LBUTTONUP;
                else if (sdlevent.button.button == SDL_BUTTON_RIGHT)
                    msg->message = (sdlevent.button.state == SDL_PRESSED) ? WM_RBUTTONDOWN : WM_RBUTTONUP;
                return TRUE;

#ifdef PLATFORM_SWITCH
            // map some joystick buttons to keys, since this whole thing is only used in UI
            case SDL_JOYBUTTONDOWN:
            case SDL_JOYBUTTONUP:
                msg->message = (sdlevent.type==SDL_JOYBUTTONDOWN)?WM_KEYDOWN:WM_KEYUP;
                msg->wParam = SE_SDL_JoyToKey(sdlevent.jbutton.button);
                if (sdlevent.jbutton.button < NUMJOYBINDS)
                    joystate[sdlevent.jbutton.button] = (sdlevent.type==SDL_JOYBUTTONDOWN);
                return TRUE;
#endif

            case SDL_WINDOWEVENT:
                if (sdlevent.window.event == SDL_WINDOWEVENT_EXPOSED)
                {
                    msg->message = WM_PAINT;
                    return TRUE;
                }
                break;

            // These all map to WM_* things without any drama.
            case SDL_QUIT:
                return TRUE;

            default: break;
        } // switch
    } // while

    return FALSE;
} // PeekMessage


void TranslateMessage(MSG *msg)
{
    // do nothing.
} // TranslateMessage


void DispatchMessage(MSG *msg)
{
    // do nothing.
} // DispatchMessage


SHORT GetKeyState(int vk)
{
    SHORT retval = 0;
#if defined(PLATFORM_PANDORA) || defined(PLATFORM_SWITCH)
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
#endif

    switch (vk)
    {
        case VK_LBUTTON:
            if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK)
                retval = 0x8000;
            #ifdef PLATFORM_PANDORA
            if(keystate[SDL_SCANCODE_RSHIFT])
                retval = 0x8000;
            #endif
            break;

        case VK_RBUTTON:
            if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_RMASK)
                retval = 0x8000;
            #ifdef PLATFORM_PANDORA
            if(keystate[SDL_SCANCODE_RCTRL])
                retval = 0x8000;
            #endif
            break;

        case VK_MBUTTON:
            if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_MMASK)
                retval = 0x8000;
            break;

        default:
            STUBBED("this can't possibly be right, yeah?");
            #if defined(PLATFORM_SWITCH)
            // check joystick "overlay" first
            vk = SE_SDL_KeyToJoy(vk);
            if (vk >= 0 && joystate[vk])
                retval = 0x8000;
            #endif
            #if defined(PLATFORM_PANDORA) || defined(PLATFORM_SWITCH)
            if (keystate[SDL_GetScancodeFromKey((SDL_Keycode)vk)])
            #else
            if (SDL_GetKeyboardState(NULL)[SDL_GetScancodeFromKey((SDL_Keycode)vk)])
            #endif
                retval = 0x8000;
            break;
    } // switch

    return(retval);
} // GetKeyState


SHORT GetAsyncKeyState(int vk)
{
    return(GetKeyState(vk));
} // GetAsyncKeyState


BOOL GetCursorPos(LPPOINT lpPoint)
{
    ASSERT(lpPoint != NULL);

    int x, y;
    SDL_GetMouseState(&x, &y);
    lpPoint->x = x;
    lpPoint->y = y;
    return(TRUE);
} // GetCursorPos


BOOL ScreenToClient(void *hWnd, LPPOINT lpPoint)
{
    // do nothing. SDL returns points in client coordinates already.
    return 1;  // success.  :)
} // ScreenToClient


int ShowCursor(BOOL yes)
{
    static int count = 0;
    count += (yes) ? 1 : -1;
    SDL_ShowCursor((count >= 0) ? SDL_TRUE : SDL_FALSE);
    return count;
} // ShowCursor

BOOL IsIconic(void *hwnd)
{
    return (hwnd != NULL) && ((SDL_GetWindowFlags((SDL_Window *) hwnd) & SDL_WINDOW_MINIMIZED) == SDL_WINDOW_MINIMIZED);
}

// end of SDLEvents.cpp ...


