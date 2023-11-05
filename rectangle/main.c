#include <stdio.h>
#include <math.h>
#include <float.h>

#define EPSILON 1e-8;

int read(double points[4][2]){
    char ch, ch1, ch2;
    for(int i = 0; i < 4; i++){
        printf("Bod #%d:\n", i+1);
        if(scanf(" %c %lf %c %lf %c ", &ch1, &points[i][0], &ch, &points[i][1], &ch2) != 5)
            return 1;
        if(ch != ',' || ch1 != '(' || ch2 != ')')
            return 1;
    }
    return 0;
}

double line(double x0, double x1, double y0, double y1){
    return sqrt(fabs((x0 * x0 - x1 * x1) + (y0 * y0 - y1 * y1)));
}

int double_equal(double a, double b){
    return fabs(a - b) <= fmax(a,b) * EPSILON;
}

double pi(double a, double b){
    return a*a + b*b;
}

int main() {
    double points[4][2];
    double lines[4];
    if(read(points)){
        printf("Nespravny vstup.\n");
        return 1;
    }
    for(int i = 0; i < 4; i++){
        if(i < 3)
            lines[i] = line(points[i][0], points[i+1][0],points[i][1], points[i+1][1]);
        else
            lines[i] = line(points[i][0], points[0][0],points[i][1], points[0][1]);
    }
    if(!double_equal(pi(lines[0], lines[1]), pi(lines[2], lines[3]))) {
        printf("Body netvori obdelnik.\n");
        return 1;
    }
    if(!double_equal(pi(lines[1], lines[2]), pi(lines[0], lines[3]))) {
        printf("Body netvori obdelnik.\n");
        return 1;
    }
    printf("Body tvori obdelnik.\n");
    return 0;
}
