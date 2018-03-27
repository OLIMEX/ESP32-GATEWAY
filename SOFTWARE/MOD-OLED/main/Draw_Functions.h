#ifndef	_DRAW_FUNCTIONS_H
#define	_DRAW_FUNCTIONS_H

#include "driver/i2c.h"
#include "SSD1306.h"

extern uint8_t oledBuffer[OLED_SIZE];

extern const uint8_t font_16x16[][32];
extern const unsigned char font_6x8 [][5];

void drawPixel(unsigned char x, unsigned char y);

void print_text(uint8_t row, char *text);

void print_smchar(unsigned char x, unsigned char y, unsigned char ch, bool scr);

void print_smtext(unsigned char row, const unsigned char *dataPtr, bool scr);

void drawLine (int x1, int y1, int x2, int y2);

void drawRectangle (int x1, int y1, int x2, int y2);

void drawSolidRectangle (int x1, int y1, int x2, int y2);

void Ellipse4Points (int CX, int CY, int X, int Y);

void drawEllipse (int CX, int CY, int XRadius, int YRadius);

void drawCircle (int x, int y, int r);

void drawTriangle (int x1, int y1, int x2, int y2, int x3, int y3);

#endif
