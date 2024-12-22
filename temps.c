#include "temps.h"

#ifdef _WIN32
#include <windows.h> // Pour Sleep
#else
#include <unistd.h>  // Pour sleep
#include <stdlib.h> // pr linux
#endif
// utiliser des bibilotheques si le multiplateforme est trop complexe
void clearecran(){
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif  
    return;
}
void attendre(float secondes) {
    #ifdef _WIN32
    Sleep(secondes * 1000.0); // Convertir en millisecondes
    #else
    usleep(secondes*1000000);
    #endif
}
