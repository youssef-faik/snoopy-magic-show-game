#include "../include/game_logic.h"

int main() {
    // Set console to use UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    runGameApp();

    return 0;
}