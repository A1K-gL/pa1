#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <stdbool.h>

const int MAP_MAX = 200;
#endif /* __PROGTEST__ */

void extension(int altitude[][MAP_MAX], int size, int ref, int i, int j, int extension[MAP_MAX][2]){
    extension[i][0] = 0;
    extension[i][1] = 0;
    for(int jj = j + 1; jj < size; jj++, extension[i][0]++)
        if(altitude[i][jj] >= ref)
            break;
    for (int jj = j - 1; jj >= 0 ; jj--, extension[i][1]++)
        if(altitude[i][jj] >= ref)
            break;
}

void castleArea ( int altitude[][MAP_MAX], int size, int area[][MAP_MAX] )
{
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            int min_i = i, max_i = i;
            int max_extension[MAP_MAX][2];
            for(int ii = i + 1; ii < size; ii++){
                if(altitude[ii][j] >= altitude[i][j])
                    break;
                max_i = ii;
            }
            for(int ii = i - 1; ii >= 0; ii--){
                if(altitude[ii][j] >= altitude[i][j])
                    break;
                min_i = ii;
            }
            for(int ii = min_i; ii <= max_i; ii++)
                extension(altitude, size, altitude[i][j], ii, j, max_extension);
            int _area = 0;
            for(int i_min = i; i_min >= min_i; i_min--){
                for(int i_max = i; i_max <= max_i; i_max++){
                    int min_extension[2] = {MAP_MAX, MAP_MAX};
                    for(int ii = i_min; ii <= i_max; ii++){
                        if(max_extension[ii][0] < min_extension[0])
                            min_extension[0] = max_extension[ii][0];
                        if(max_extension[ii][1] < min_extension[1])
                            min_extension[1] = max_extension[ii][1];
                    }
                    int tmp = (min_extension[0] + min_extension[1] + 1) * (i_max - i_min + 1);
                    if(tmp > _area)
                        _area = tmp;
                }
            }
            area[i][j] = _area;
        }
    }
}

#ifndef __PROGTEST__
bool identicalMap ( const int a[][MAP_MAX], const int b[][MAP_MAX], int n )
{
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(a[i][j] != b[i][j]) {
                return false;
            }
        }
    }
    return true;
}
int main ( int argc, char * argv [] )
{
    static int result[MAP_MAX][MAP_MAX];

    static int alt0[MAP_MAX][MAP_MAX] =
            {
                    { 1, 2 },
                    { 3, 4 }
            };
    static int area0[MAP_MAX][MAP_MAX] =
            {
                    { 1, 2 },
                    { 2, 4 }
            };
    castleArea ( alt0, 2, result );
    assert ( identicalMap ( result, area0, 2 ) );
    static int alt1[MAP_MAX][MAP_MAX] =
            {
                    { 2, 7, 1, 9 },
                    { 3, 5, 0, 2 },
                    { 1, 6, 3, 5 },
                    { 1, 2, 2, 8 }
            };
    static int area1[MAP_MAX][MAP_MAX] =
            {
                    { 1, 12, 2, 16 },
                    { 4, 4, 1, 2 },
                    { 1, 9, 4, 4 },
                    { 1, 2, 1, 12 }
            };
    castleArea ( alt1, 4, result );
    assert ( identicalMap ( result, area1, 4 ) );
    static int alt2[MAP_MAX][MAP_MAX] =
            {
                    { 1, 2, 3, 4 },
                    { 2, 3, 4, 5 },
                    { 3, 4, 5, 6 },
                    { 4, 5, 6, 7 }
            };
    static int area2[MAP_MAX][MAP_MAX] =
            {
                    { 1, 2, 3, 4 },
                    { 2, 4, 6, 8 },
                    { 3, 6, 9, 12 },
                    { 4, 8, 12, 16 }
            };
    castleArea ( alt2, 4, result );
    assert ( identicalMap ( result, area2, 4 ) );
    static int alt3[MAP_MAX][MAP_MAX] =
            {
                    { 7, 6, 5, 4 },
                    { 6, 5, 4, 5 },
                    { 5, 4, 5, 6 },
                    { 4, 5, 6, 7 }
            };
    static int area3[MAP_MAX][MAP_MAX] =
            {
                    { 12, 6, 2, 1 },
                    { 6, 2, 1, 2 },
                    { 2, 1, 2, 6 },
                    { 1, 2, 6, 12 }
            };
    castleArea ( alt3, 4, result );
    assert ( identicalMap ( result, area3, 4 ) );
    static int alt4[MAP_MAX][MAP_MAX] =
            {
                    { 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1 },
                    { 1, 1, 2, 1, 1 },
                    { 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1 }
            };
    static int area4[MAP_MAX][MAP_MAX] =
            {
                    { 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1 },
                    { 1, 1, 25, 1, 1 },
                    { 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1 }
            };
    castleArea ( alt4, 5, result );
    assert ( identicalMap ( result, area4, 5 ) );
    static int alt5[MAP_MAX][MAP_MAX] =
            {
                    { 2, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 2 },
                    { 1, 1, 2, 1, 1 },
                    { 1, 1, 1, 1, 1 },
                    { 1, 3, 1, 1, 1 }
            };
    static int area5[MAP_MAX][MAP_MAX] =
            {
                    { 8, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 10 },
                    { 1, 1, 12, 1, 1 },
                    { 1, 1, 1, 1, 1 },
                    { 1, 25, 1, 1, 1 }
            };
    castleArea ( alt5, 5, result );
    assert ( identicalMap ( result, area5, 5 ) );
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
