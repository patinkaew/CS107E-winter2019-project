#include "shell.h"
#include "shell_commands.h"
#include "uart.h"
#include "malloc.h"
#include "strings.h"
#include "gpio.h"
#include "waterlevel.h"
#include "keyboard.h"
#include "gl.h"
#include "timer.h"
#include "infographic.h"
#include <stdint.h>
#include <stdbool.h>

#define LINE_LEN 80

static int (*shell_printf)(const char * format, ...);
static int tokenize(const char *line, char *array[]);
static unsigned int lookup(char* name);
static int isspace(char ch);
static char *strndup(const char *src, size_t n);

int cmd_burst(int argc, const char *argv[]);

int cmd_checkMoisture(int argc, const char *argv[]);

int cmd_checkWater(int argc, const char *argv[]);

int cmd_startWatering(int argc, const char *argv[]);

int cmd_stopWatering(int argc, const char *argv[]);

int cmd_waterGraph(int argc, const char *argv[]);

static const command_t commands[] = {
    {"help",   "<cmd> prints a list of commands or description of cmd", cmd_help},
    {"echo",   "<cmd> echos the user input to the screen", cmd_echo},
    {"reboot",   "<cmd> resets the pi and reboots the bootloader", cmd_reboot},
    {"peek",   "<cmd> displays the value at a memory address", cmd_peek},
    {"poke",   "<cmd> modifies the value at a memory address", cmd_poke},
    {"graph",   "<cmd> displays a real-time graph of water levels remaining", cmd_waterGraph},
    {"stop",   "<cmd> stop autowatering the plant", cmd_stopWatering},
    {"start",   "<cmd> starts auto watering the plant", cmd_startWatering},
    {"water",   "<cmd> checks the current water levels remaining", cmd_checkWater},
    {"moisture",   "<cmd> checks the current moisture of the plant", cmd_checkMoisture},
    {"burst",   "<cmd> releases a burst of water", cmd_burst}

};

//Reboot Command - "resets the pi and reboots the bootloader"
int cmd_reboot(int argc, const char *argv[])
{
    uart_putchar('\04'); //Exits the program
    pi_reboot(); //Reboots the pi
    return 0;
}

//Peek Command - "displays the value at a memory address"
int cmd_peek(int argc, const char* argv[]) {
    if (argc == 1) { //Case where not enough args were given
        shell_printf("peek requires 1 argument [address]");
        return 1;
    }

    unsigned int *address = (unsigned int) strtonum(argv[1], NULL);

    char* firstChar = argv[1];


    //Case where invalid input was given
    if (address == 0 && firstChar[0] != '0') {
        shell_printf("peek cannot convert \'%s\'\n", argv[1]);
        return 1;
    }

    uintptr_t tempptr = (uintptr_t)tempptr;

    //Case where address not aligned
    if ((tempptr % 4) != 0) {
        shell_printf("error: peek address must be 4-byte aligned\n");
        return 1;
    }

    shell_printf("\n0x%08x: %x\n", address, *address);
    return 0;

}

//Poke Command - "modifies the value at a memory address"
int cmd_poke(int argc, const char* argv[]) {
    //Case where not enough args
    if (argc < 3) {
        shell_printf("poke expects 2 arguments [address] [value]");
        return 1;
    }

    unsigned int *address = (unsigned int)strtonum(argv[1], NULL);
    unsigned int value = strtonum(argv[2], NULL);

    char* firstArg = argv[1];
    char* secondArg = argv[2];


    //Case where invalid inout was given
    if (address == 0 && firstArg[0] != '0') {
        shell_printf("poke cannot convert \'%s\'\n", argv[1]);
        return 1;
    }

    if (address == 0 && secondArg[0] != '0')
    {
        shell_printf("poke cannot convert \'%s\'\n", argv[2]);
        return 1;
    }

    uintptr_t tempptr = (uintptr_t)tempptr;

    if ((tempptr % 4) != 0) {
        shell_printf("error: peek address must be 4-byte aligned\n");
        return 1;
    }

    *address = value;
    return 0;

}

//Echo command - "echos the user input to the screen"
int cmd_echo(int argc, const char *argv[])
{
    for (int i = 1; i < argc; ++i)
        shell_printf("\n%s ", argv[i]);
    shell_printf("\n");
    return 0;
}

//Help Command - prints a list of commands or description of cmd
int cmd_help(int argc, const char *argv[])
{   shell_printf("\n");

    //Case where no arguments were given and all commands need to be printed out
    if (argc == 1) {
        int n = sizeof(commands) / sizeof(commands[0]);
        int counter = 0;

        while (counter < n) {
            shell_printf("%s: %s\n", commands[counter].name, commands[counter].description);
            counter++;

        }
    //Case where only one help function needs to be printed
    } else {
        int result;

        result = lookup(argv[1]);

        if (result >= 0) {
            shell_printf("%s: %s\n", commands[result].name, commands[result].description);
        } else {
            shell_printf("no such command \'%s\'\n", argv[1]);
        }

    }
    return 0;
}

//Matches first word types to a function in the lookup directory of functions
static unsigned int lookup(char* name) {
    int counter = 0;
    while (counter < sizeof(commands)) {
        if (strcmp(commands[counter].name, name) == 0) {
            return counter;
        }
        counter++;
    }

    return -1;
}

void shell_init(formatted_fn_t print_fn)
{
    shell_printf = print_fn;
}

void shell_bell(void)
{
    uart_putchar('\a');
}

void shell_readline(char buf[], int bufsize)
{
    int counter = 0;
    char currChar = keyboard_read_next();
    if(currChar == 0){
        buf[0] = '\0';
        return;
    }

    //Keeps running until user hits enter or limit is reached
    while (currChar != '\n' && counter <= bufsize - 2) {


        if (currChar == 0x08)
        {
            //Runs shell bell if backspaces when at the beginning
            if (counter == 0) {
                shell_bell();
            } else {
                //Backspaces by backspacing, inserting a space, and backspacing again
                shell_printf("%c", '\b');
                shell_printf("%c", ' ');
                shell_printf("%c", '\b');
                counter--;

            }
        } else {
            shell_printf("%c", currChar);
            buf[counter] = currChar;
            counter++;
        }

        currChar = keyboard_read_next();
    }

    buf[counter] = '\0';

}

int shell_evaluate(const char *line)
{
    char *args[100];
    int counter = 0;

    int ntokens = tokenize(line, args);

    char *array[ntokens];

    while (counter < ntokens) {
        array[counter] = args[counter];
        counter++;
    }

    //Means nothing was entered or all spaces were entered
    if (ntokens == 0) {
        return -1;
    }

    int result = lookup(array[0]);

    if (result == -1) {
        shell_printf("\n%s is an invalid command\n", array[0]);
        return 0;
    }

    commands[result].fn(ntokens, array);

    return 0;


}

bool keyboard_has_char(void);

void shell_run(void)
{   shell_printf("\n");
    shell_printf("Welcome to the CS107E shell. Remember to type on your PS/2 keyboard!\n");
    shell_printf("Pi> ");
    while (1)
    {
        char line[LINE_LEN];

        if(keyboard_has_char()){
            shell_printf("Pi> ");
            shell_readline(line, sizeof(line));
            shell_evaluate(line);
        }
        infographic_update_screen();
        gl_swap_buffer();
    }
}

//Allocates space and copies string to new allocation - Adapted from Lab 4
static char *strndup(const char *src, size_t n)
{
    char *address = malloc(n + 1);
    memcpy(address, src, n);
    return address;
}

//Credit: Lab4 - heapclient/strings_main.c
//Modified to suit shell program
static int isspace(char ch)
{
    return ch == ' ' || ch == '\t' || ch == '\n';
}

//Credit: Lab4 - heapclient/strings_main.c
//Modified to suit shell program
static int tokenize(const char *line, char *array[])
{
    int ntokens = 0;

    while (*line != '\0') {
        while (isspace(*line)) line++;  // skip past spaces
        if (*line == '\0') break; // no more non-white chars
        const char *start = line;
        while (*line != '\0' && !isspace(*line)) line++; // advance to next space/null
        int nchars = line - start;
        array[ntokens++] = strndup(start, nchars);   // make heap-copy, add to array
    }
    return ntokens;
}

int cmd_waterGraph(int argc, const char *argv[])
{
    infographic_update_screen();
    gl_swap_buffer();
    return 0;
}

int cmd_stopWatering(int argc, const char *argv[])
{
    //
    return 0;
}

int cmd_startWatering(int argc, const char *argv[])
{
    //
    return 0;
}

int cmd_checkWater(int argc, const char *argv[])
{
    shell_printf("The Current Water Height is: %d\n", waterlevel_get_percent());
    return 0;
}

int cmd_checkMoisture(int argc, const char *argv[])
{
    //
    return 0;
}

int cmd_burst(int argc, const char *argv[])
{
    gpio_write(GPIO_PIN17, 1);
    timer_delay(1);
    gpio_write(GPIO_PIN17, 0);
    return 0;
}
