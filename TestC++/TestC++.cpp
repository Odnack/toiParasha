#include "GymConsole.h"
#include <windows.h>

int main() {
    setlocale(LC_ALL, "");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    GymConsole console;
    console.run();
    return 0;
}