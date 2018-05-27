#ifndef _DEBUGDISPLAY_H
#define _DEBUGDISPLAY_H
//****************************************************************************
//**
//**    DebugDisplay.h
//**    - Provides display capabilities for debugging. Because it is
//**	  specifically for debugging and not final release, we don't
//** 	  care for portability here
//**
//****************************************************************************

//============================================================================
//    INTERFACE REQUIRED HEADERS
//============================================================================

#include <stdint.h>
void DebugPutc (unsigned char c);
void DebugClrScr (const uint8_t c);
void DebugPuts (char* str);
int DebugPrintf (const char* str, ...);
unsigned DebugSetColor (const unsigned c);
void DebugGotoXY (unsigned x, unsigned y);

#endif
