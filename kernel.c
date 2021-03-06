#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <lstdlib.h>
#include <asm.h>
#include <cursor.h>
#include <strings.h>
#include <keyboard.h>
 
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
 
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};
 
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}
 
size_t VGA_WIDTH;
size_t VGA_HEIGHT;

int SHIFT = 0;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    VGA_WIDTH = 80;
    VGA_HEIGHT = 25;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_scroll(void) {
    for (unsigned int i = 0; i < VGA_HEIGHT; i++) {
        for (unsigned int m = 0; m < VGA_WIDTH; m++) {
            terminal_buffer[i * VGA_WIDTH + m] = terminal_buffer[(i+1) * VGA_WIDTH + m];
        }
    }
}
 
void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}
 
void terminal_putchar(char c) {
    if (c == 0)
        return;
    if (c == '\n') {
        terminal_row++;
        terminal_column = 0;
        update_cursor(terminal_column, terminal_row, VGA_WIDTH);
        return;
    }
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column >= VGA_WIDTH) {
        terminal_row++;
        terminal_column = 0;
    }
    if (terminal_row == VGA_HEIGHT) {   
        terminal_scroll();
        terminal_row--;
    }
    update_cursor(terminal_column, terminal_row, VGA_WIDTH);
}

void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}
 
void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}

char kgetchar()
{
    int sc = get_scancode();
    char c = scancodes[sc+1];
    if (c == 0x7f) {
        do {
            update_cursor(--terminal_column, terminal_row, VGA_WIDTH);
            terminal_putentryat(0, terminal_color, terminal_column, terminal_row);
        } while (terminal_buffer[terminal_row * VGA_WIDTH + terminal_column-1] == 0);
        return 0;
    }
    if (sc == 0x2A)
        SHIFT = 0x2A;
    if ((sc == 0xAA) && (SHIFT == 0x2A))
        SHIFT = 0;
    if (sc == 0x36)
        SHIFT = 0x36;
    if ((sc == 0xB6) && (SHIFT == 0x36))
        SHIFT = 0;
    if (sc == 0x3A)
        SHIFT = !SHIFT;
    return SHIFT ? c : tolower(c);
}

char getchar()
{
    return scancodes[get_scancode()+1];
}

char* scan()
{
    char* ret = (char*) 0xF8000;
    char c;
    int i = 0;
    while ((c = getchar())) {
        if ((c == '\n') || (c == EOF))
            break;
        ret[i] = c;
        if (c == 0x7f)
            i--;
    }
    ret[i] = '\0';
    return ret;
} 

void kernel_main(void) {
    terminal_initialize();
    enable_cursor(0, VGA_HEIGHT);
 
    terminal_writestring("Welcome to ");
    terminal_setcolor(VGA_COLOR_LIGHT_BROWN);
    terminal_writestring("kazamaOS");
    terminal_setcolor(VGA_COLOR_LIGHT_GREY);
    terminal_writestring("!\n");

    char c;
    while (true) {
        while ((c = kgetchar())) {
            terminal_putchar(c);
            update_cursor(terminal_column, terminal_row, VGA_WIDTH);
        }
    }
}
