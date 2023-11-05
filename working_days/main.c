#ifndef __PROGTEST__
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
typedef struct
{
    int m_TotalDays;
    int m_WorkDays;
} TResult;
#endif /* __PROGTEST__ */

bool isHoliday(int m, int d){
    if(m == 1 && d == 1)
        return true;
    else if(m == 5 && d == 1)
        return true;
    else if(m == 5 && d == 8)
        return true;
    else if(m == 7 && d == 5)
        return true;
    else if(m == 7 && d == 6)
        return true;
    else if(m == 9 && d == 28)
        return true;
    else if(m == 10 && d == 28)
        return true;
    else if(m == 11 && d == 17)
        return true;
    else if(m == 12 && (d == 24 || d == 25 || d == 26))
        return true;
    return false;
}

bool isLeapYear(int y){
    if(y % 4 == 0){
        if(y % 100 == 0){
            if(y % 400 == 0){
                if(y % 4000 == 0)
                    return false;
                return true;
            }
            return false;
        }
        return true;
    }
    return false;
}

bool checkMonth(bool leapYear, int m, int d){
    int month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(leapYear)
        month[1] = 29;
    if(m > 12 || m < 1)
        return false;
    if(d > month[m-1] || d < 1)
        return false;
    return true;
}
//????
int total_days( int y1, int m1, int d1,
                int y2, int m2, int d2 )
{
    int days = 1;
    int month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(isLeapYear(y1))
        month[1] = 29;
    days += month[m1-1] - d1;
    for(int i = m1; i < 12; i++)
        days += month[i];
    y1++;
    days += (y2-y1)*365 + ((y2-y1)/4) - ((y2-y1)/100) + ((y2-y1)/400) - ((y2-y1)/4000);
    if(isLeapYear(y2))
        month[1] = 29;
    else
        month[1] = 28;
    for(int i = 0; i < m2-1; i++) {
        days += month[i];
    }
    days += d2;
    return days;
}

bool is_weekend(int day){
    return ((day % 7 == 1) || (day % 7 == 2));
}

bool isWorkDay ( int y, int m, int d )
{
    if(y < 2000 || !checkMonth(isLeapYear(y), m, d))
        return false;
    if(isHoliday(m,d))
        return false;
    int days = total_days(2000,1,1,y, m ,d);
    return !is_weekend(days);
}

int countWorkDays ( int y1, int y2, int total_days1, int total_days2)
{
    int t_days = (total_days2/364)*260 - (total_days1/364)*260;
    int rest_days = total_days2 - (total_days2%364 - total_days1%364);
    int holidays[11] = {1, 122, 129, 187, 188, 272, 302, 322, 359, 360, 361};
    for(int i = 2000; i <= y2; i++){
        for(int j = 0; j < 11; j++) {
            if (holidays[j] >= total_days1) {
                if(holidays[j] <= total_days2 && !is_weekend(holidays[j]))
                    t_days--;
            }
            holidays[j] += 365;
            if(j == 0 && isLeapYear(i))
                holidays[j]++;
            if(isLeapYear(i+1) && j != 0)
                holidays[j]++;
        }
    }
    for(int i = rest_days; i <= total_days2; i++){
        if(!is_weekend(i))
            t_days++;
    }
    return t_days;
}

TResult countDays ( int y1, int m1, int d1,
                    int y2, int m2, int d2 )
{
    TResult result;
    result.m_TotalDays = total_days(y1,m1,d1,y2,m2,d2);
    bool non_valid = false;
    if(result.m_TotalDays < 1)
        non_valid = true;
    else if(y1 < 2000 || y2 < 2000)
        non_valid = true;
    else if(!checkMonth(isLeapYear(y1), m1, d1))
        non_valid = true;
    else if(!checkMonth(isLeapYear(y2),m2,d2))
        non_valid = true;
    if(non_valid){
        result.m_TotalDays = -1;
        result.m_WorkDays = -1;
    } else{
        int total_days1 = total_days(2000,1,1,y1,m1,d1);
        int total_days2 = total_days(2000,1,1,y2,m2,d2);
        result.m_WorkDays = countWorkDays(y1,y2,total_days1,total_days2);
        result.m_TotalDays = total_days2 - total_days1 + 1;
    }
    return result;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
    TResult r;

    assert ( !isWorkDay ( 2000, 2, 31 ) );

    assert ( isWorkDay ( 2000, 1, 3 ) );

    assert ( isWorkDay ( 2023, 10, 10 ) );

    assert ( ! isWorkDay ( 2023, 11, 11 ) );

    assert ( ! isWorkDay ( 2023, 11, 17 ) );

    assert ( ! isWorkDay ( 2023, 11, 31 ) );

    assert ( ! isWorkDay ( 2023,  2, 29 ) );

    assert ( ! isWorkDay ( 2004,  2, 29 ) );

    assert ( isWorkDay ( 2008,  2, 29 ) );

    assert ( ! isWorkDay ( 2001,  2, 29 ) );

    assert ( ! isWorkDay ( 1996,  1,  2 ) );

    r = countDays(2000,1,3,
                  2000,1,7);
    assert(r.m_TotalDays == 5);
    assert(r.m_WorkDays == 5);
    r = countDays(2000,1,1,
                  2000,12,31);
    assert(r.m_TotalDays == 366);
    assert(r.m_WorkDays == 252);

    r = countDays ( 2023, 11,  1,
                    2023, 11, 30 );
    assert ( r . m_TotalDays == 30 );
    assert ( r . m_WorkDays == 21 );

    r = countDays ( 2023, 11,  1,
                    2023, 11, 17 );
    assert ( r . m_TotalDays == 17 );
    assert ( r . m_WorkDays == 12 );

    r = countDays ( 2023, 11,  1,
                    2023, 11,  1 );
    assert ( r . m_TotalDays == 1 );
    assert ( r . m_WorkDays == 1 );

    r = countDays ( 2023, 11, 17,
                    2023, 11, 17 );
    assert ( r . m_TotalDays == 1 );
    assert ( r . m_WorkDays == 0 );

    r = countDays ( 2023,  1,  1,
                    2023, 12, 31 );
    assert ( r . m_TotalDays == 365 );
    assert ( r . m_WorkDays == 252 );

    r = countDays ( 2024,  1,  1,
                    2024, 12, 31 );
    assert ( r . m_TotalDays == 366 );
    assert ( r . m_WorkDays == 254 );

    r = countDays ( 2000,  1,  1,
                    2023, 12, 31 );
    assert ( r . m_TotalDays == 8766 );
    assert ( r . m_WorkDays == 6072 );

    r = countDays ( 2001,  2,  3,
                    2023,  7, 18 );
    assert ( r . m_TotalDays == 8201 );
    assert ( r . m_WorkDays == 5682 );

    r = countDays ( 2021,  3, 31,
                    2023, 11, 12 );
    assert ( r . m_TotalDays == 957 );
    assert ( r . m_WorkDays == 666 );

    r = countDays ( 2001,  1,  1,
                    2000,  1,  1 );
    assert ( r . m_TotalDays == -1 );
    assert ( r . m_WorkDays == -1 );

    r = countDays ( 2001,  1,  1,
                    2023,  2, 29 );
    assert ( r . m_TotalDays == -1 );
    assert ( r . m_WorkDays == -1 );

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
