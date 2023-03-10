#include "ports.h"
#include "screen.h"
#include "../libc/mem.h"

void set_cursor(int offset);
int get_screen_offset();
int get_cursor();
int get_offset_row(int offset);
int get_offset_col(int offset);

void print_char(char character, int col, int row, char attribute_byte);

int handle_scrolling(int cursor_offset)
{
    if (cursor_offset < MAX_ROWS * MAX_COLS * 2)
    {
        return cursor_offset;
    }

    for (int i = 0; i < MAX_ROWS; i++)
    {
        memory_copy(get_screen_offset(0, i) + VIDEO_ADDRESS,
                    get_screen_offset(0, i - 1) + VIDEO_ADDRESS, MAX_COLS * 2);
    }

    char* last_line = get_screen_offset(0, MAX_ROWS-1) + VIDEO_ADDRESS;
    for(int i=0; i<MAX_COLS*2; i++){
        last_line[i] = 0;
    }

   cursor_offset -= 2*MAX_COLS;

   return cursor_offset; 
}

void print_char(char character, int col, int row, char attribute_byte)
{
    unsigned char *vidmem = (unsigned char *)VIDEO_ADDRESS;

    if (!attribute_byte)
    {
        attribute_byte = WHITE_ON_BLACK;
    }

    int offset;
    if (col >= 0 && row >= 0)
    {
        offset = get_screen_offset(col, row);
    }
    else
    {
        offset = get_cursor();
    }

    if (character == '\n')
    {
        int rows = offset / (2 * MAX_COLS);
        offset = get_screen_offset(79, rows);
    }
    else
    {
        vidmem[offset] = character;
        vidmem[offset + 1] = attribute_byte;
    }

    offset += 2;
    offset = handle_scrolling(offset);
    set_cursor(offset);
}

int get_screen_offset(int column, int row)
{
    int offset = row * MAX_COLS + column;
    return offset * 2;
}

int get_cursor()
{
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);

    return offset * 2;
}

void set_cursor(int offset)
{
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset));
}

void print_at(char *message, int col, int row)
{
    if (col >= 0 && row >= 0)
    {
        set_cursor(get_screen_offset(col, row));
    }
    int i = 0;
    while (message[i] != 0)
    {
        print_char(message[i++], col++, row, WHITE_ON_BLACK);
    }
}

void print(char *message)
{
    print_at(message, -1, -1);
}

void handle_backspace() {
    int offset = get_cursor()-2;
    int row = get_offset_row(offset);
    int col = get_offset_col(offset);

    //only allow deletion to beginning of line 
    if(col >= 1){
        print_char(' ', col, row, WHITE_ON_BLACK);
        set_cursor(offset);
    }
}

void clear_screen()
{
    int row = 0;
    int col = 0;

    for (int row = 0; row < MAX_ROWS; row++)
    {
        for (int col = 0; col < MAX_COLS; col++)
        {
            print_char(' ', col, row, WHITE_ON_BLACK);
        }
    }

    set_cursor(get_screen_offset(0, 0));
}

int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; }