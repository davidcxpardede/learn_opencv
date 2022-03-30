// Star Centroiding Method
// Center of Gravity

#include <iostream>
using namespace std;

int main() {



    // CENTER OF GRAVITY EQUATION
    
    // Defining Variables

    int nI = 5;         // Number of Horizontal Pixels
    int nJ = 5;         // Number of Vertical Pixels

    int i;          // Horizontal Pixel Coordinate
    int j;          // Vertical Pixel Coordinate
    int G[5][5] = {};     // Gray Value

    float xc;       // Horizontal Center of Gravity
    float yc;       // Vertical Center of Gravity

    int sigmaIG;     // Sum of i and G product
    int iG;
    int sigmaJG;     // Sum of j and G product
    int jG;
    int sigmaG;      // Sum of G

    // Defining Gray Value (Randomly Generated)

    for (i = 0; i < nI; i++) {
        for (j = 0; j < nJ; j++) {
           G[i][j] = rand() % 255 + 1;
        }
    }

    // Defining Sigmas

        for (i = 0; i < nI; i++) {
            for (j = 0; j < nJ; j++) {
                iG = i * G[i][j];
                jG = j * G[i][j];
                sigmaIG += iG;
                sigmaJG += jG;
                sigmaG += G[i][j];
            }
        }
    
    // Defining Equations

    xc = sigmaIG/sigmaG;
    yc = sigmaJG/sigmaG;

    return 0;

    cout << ":v";
}