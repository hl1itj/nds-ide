// Graphics converted using PAGfx by Mollusk.

#pragma once

#include <PA_BgStruct.h>

#ifdef __cplusplus
extern "C"{
#endif

// Sprites:
extern const unsigned char selected_Sprite[64] _GFX_ALIGN; // Palette: selected_Pal
extern const unsigned char menu_del_Sprite[1024] _GFX_ALIGN; // Palette: menu_del_Pal
extern const unsigned char menu_move_Sprite[1024] _GFX_ALIGN; // Palette: menu_move_Pal
extern const unsigned char menu_serial_Sprite[4096] _GFX_ALIGN; // Palette: menu_serial_Pal
extern const unsigned char menu_setting_Sprite[1024] _GFX_ALIGN; // Palette: menu_setting_Pal
extern const unsigned char menu_usb_Sprite[4096] _GFX_ALIGN; // Palette: menu_usb_Pal
extern const unsigned char menu_wifi_Sprite[4096] _GFX_ALIGN; // Palette: menu_wifi_Pal
extern const unsigned char move_create_Sprite[1024] _GFX_ALIGN; // Palette: move_create_Pal
extern const unsigned char sel_menu_serial_Sprite[4096] _GFX_ALIGN; // Palette: sel_menu_serial_Pal
extern const unsigned char sel_menu_usb_Sprite[4096] _GFX_ALIGN; // Palette: sel_menu_usb_Pal
extern const unsigned char sel_menu_wifi_Sprite[4096] _GFX_ALIGN; // Palette: sel_menu_wifi_Pal

// Backgrounds:
extern const PA_BgStruct bg_up;
extern const PA_BgStruct bg_create;
extern const PA_BgStruct bg_dialog;
extern const PA_BgStruct bg_down;
extern const PA_BgStruct bg_edit_dialog;

// Palettes:
extern const unsigned short selected_Pal[256] _GFX_ALIGN;
extern const unsigned short menu_del_Pal[256] _GFX_ALIGN;
extern const unsigned short menu_move_Pal[256] _GFX_ALIGN;
extern const unsigned short menu_serial_Pal[256] _GFX_ALIGN;
extern const unsigned short menu_setting_Pal[256] _GFX_ALIGN;
extern const unsigned short menu_usb_Pal[256] _GFX_ALIGN;
extern const unsigned short menu_wifi_Pal[256] _GFX_ALIGN;
extern const unsigned short move_create_Pal[256] _GFX_ALIGN;
extern const unsigned short sel_menu_serial_Pal[256] _GFX_ALIGN;
extern const unsigned short sel_menu_usb_Pal[256] _GFX_ALIGN;
extern const unsigned short sel_menu_wifi_Pal[256] _GFX_ALIGN;

#ifdef __cplusplus
}
#endif
