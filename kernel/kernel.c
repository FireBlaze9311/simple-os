#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../libc/string.h"

void main(){
    isr_install();
    irq_install();

    // Set Interrupt Flag
    asm volatile("sti");
    //init_timer(50);

    init_keyboard();

    print("\n\nWillkommen! \n");

    print("> ");

}

void user_input(char *input) {
    if (strcmp(input, "EXIT") == 0) {
        print("Stopping. Bye!\n");
        asm volatile("hlt");
    }
    if(strcmp(input, "CLEAR") == 0){
        clear_screen();
    }
    print(input);
    print("\n> ");
}