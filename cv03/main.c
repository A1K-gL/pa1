#include <stdio.h>

int main() {
    int x,y,z;
    char c;
    printf("Zadejte barvu v RGB formatu:\n");
    int scan_out = scanf(" rgb ( %d , %d , %d %c ", &x, &y, &z, &c);
    if(scan_out < 4){
        printf("Nespravny vstup.\n");
        return 1;
    }
    if((x < 0) || (x > 255) || (y < 0) || (y > 255) || (z < 0) || (z > 255) || (c != ')')){
        printf("Nespravny vstup.\n");
        return 1;
    }
    printf("#%02X%02X%02X\n", x, y, z);
    return 0;
}
