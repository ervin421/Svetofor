/*
Pri zapuske zagoraetsya krasnyy pryamo i zelyonyy sboku.
Tsykl:
     5 sekund
     Vezde vklyuchayetsya zholtyy
     2 sekundy.
     Yesli pryamo gorit krasnyy, to
        zagoraetsya zelyonyy pryamo i krasnyy sboku
     inache
        zagoraetsya krasnyy pryamo i zelyonyy sboku
*/
#include <stdio.h>
#ifdef _WIN32
#include <windows.h> // Dlya funktsii Sleep() i nastroiki kodirovki
#else
#include <unistd.h> // Dlya funktsii Sleep() na POSIX-sistemakh
#endif
#include <locale.h> // Dlya nastroiki lokali
// Opredelenie bitovykh masok
#define RED_MAIN (1 << 0) // Krasnyy svet dlya glavnoj dorogi (bit 1)
#define RED_SIDE (1 << 3) // Krasnyy svet dlya bokovoj dorogi (bit 4)
#define YELLOW ((1 << 1) | (1 << 4)) // Zholtyy svet (bity 2 i 5)
#define GREEN_MAIN (1 << 2) // Zelyonyy svet dlya glavnoj dorogi (bit 3)
#define GREEN_SIDE (1 << 5) // Zelyonyy svet dlya bokovoj dorogi (bit 6)
// Funktsiya dlya otobrazheniya sostoyaniya bitovogo konteynera
void displayContainer(unsigned char container) {
    printf("Tekushchee sostoyanie konteynera: 0b");
    for (int i = 5; i >= 0; i--) {
        printf("%d", (container >> i) & 1);
    }
    printf("\n");
    printf("Sostoyanie svetofora:\n");
    if (container & RED_MAIN) {
        printf(" - Krasnyy svet na glavnoj doroge\n");
    }
    if (container & YELLOW) {
        printf(" - Zholtyy svet\n");
    }
    if (container & GREEN_MAIN) {
        printf(" - Zelyonyy svet na glavnoj doroge\n");
    }
    if (container & GREEN_SIDE) {
        printf(" - Zelyonyy svet na bokovoj doroge\n");
    }
    if (container & RED_SIDE) {
        printf(" - Krasnyy svet na bokovoj doroge\n");
    }
    printf("------------------------------------\n");
}

// Kross-platformennaya funktsiya dlya pauzy
void crossPlatformSleep(int seconds) {
#ifdef _WIN32
    Sleep(seconds * 1000); // V Windows Sleep prinimayet millisekundy
#else
    sleep(seconds); // V POSIX sleep prinimayet sekundy
#endif
}
int main() {
#ifdef _WIN32
    // Ustanavlivaem kodirovku UTF-8 dlya Windows Console
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
    // Ustanavlivaem lokal dlya korrektnogo otobrazheniya teksta
    if (!setlocale(LC_ALL, "ru_RU.UTF-8")) {
        fprintf(stderr, "Ne udalos' ustanovit' lokal'. Proverte nastroiki sistemy. \n");
        return 1;
    }
    unsigned char container = 0; // Iznachal'no vse bity sbrosheny
    int repeat = 1;
    int count = 0;
    while (repeat) {

        // 1. Krasnyy na glavnoj doroge, zelyonyy na bokovoj doroge
        container = RED_MAIN | GREEN_SIDE;
        displayContainer(container);
        crossPlatformSleep(5);

        // 2. Zholtyy svet na obeikh dorogakh (perekhod)
        container = YELLOW;
        displayContainer(container);
        crossPlatformSleep(2);

        // 3. Zelyonyy na glavnoj doroge, krasnyy na bokovoj doroge
        container = GREEN_MAIN | RED_SIDE;
        displayContainer(container);
        crossPlatformSleep(5);

        // 4. Zholtyy svet na obeikh dorogakh (perekhod)
        container = YELLOW;
        displayContainer(container);
        crossPlatformSleep(2);
        count++;
        if (count > 5) repeat = 0;
    }

    return 0;
}
