#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../libc/string.h"
#include "../libc/mem.h"

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

void execute_command(char *input) {
    if (strcmp(input, "EXIT") == 0) {
        print("Stopping. Bye!\n");
        asm volatile("hlt");
    }
    else if(strcmp(input, "CLEAR") == 0){
        clear_screen();
    }
    else if(strcmp(input, "HELP") == 0){
        print("Supported Commands: \n EXIT - Exit OS\n CLEAR - Clear Screen");
    }
    else{
        print("Unknown Command: ");
        print(input);
    }
    print("\n> ");
}