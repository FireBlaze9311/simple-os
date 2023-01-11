#ifndef SCREEN_H
#define SCREEN_H

#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

void print(char* message);
void print_at(char* message, int col, int row);
void clear_screen();
#endif