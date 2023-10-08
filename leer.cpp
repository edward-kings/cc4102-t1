#include<iostream>
#include "disk_manipulation_functions/binaryFileFunctions.hpp"

using namespace std;

int main() {
    FILE* rect;
    rect = binOpen("rectangulos.bin",false);
    Rect buffer[rectanglesPerBlock];
    cout << "Cantidad de rectangulos: " << rectanglesPerBlock << endl;
    binRectPageRead(rect,buffer);
    for (unsigned int i =0; i < rectanglesPerBlock ; i++) {
        cout << "Rect: (" << buffer[i].x1 << "," << buffer[i].y1 << "," << buffer[i].x2 << "," << buffer[i].y2 << ")" << endl;
        cout << "Largo: " << buffer[i].x2 - buffer[i].x1 << endl;
        cout << "Ancho: " << buffer[i].y2 - buffer[i].y1 << endl;
    }
    fclose(rect);
    return 0;
}