#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct DATE{
    int m_year;
    int m_month;
    int m_day;
};

bool equal_dates(struct DATE a, struct DATE b){
    if(a.m_year != b.m_year)
        return false;
    if(a.m_month != b.m_month)
        return false;
    if(a.m_day != b.m_day)
        return false;
    return true;
}

bool more_than(struct DATE a, struct DATE b){
    if(a.m_year > b.m_year)
        return false;
    else if(a.m_year == b.m_year){
        if(a.m_month > b.m_month)
            return false;
        else if(a.m_month == b.m_month){
            if(a.m_day > b.m_day)
                return false;
        }
    }
    return true;
}

void array_extension(struct DATE** a, int** b, char*** c, size_t* size){
    (*size) = 2 * (*size);
    (*a) = (struct DATE*)realloc((*a), (*size) * sizeof (struct DATE));
    (*b) = (int*)realloc((*b), (*size) * sizeof (int));
    (*c) = (char**)realloc((*c), (*size) * sizeof (char*));
}

bool read_date(struct DATE* result){
    char del1, del2;
    if(scanf(" %d %c %d %c %d ", &result->m_year, &del1, &result->m_month, &del2, &result->m_day) != 5)
        return false;
    if((del1 != del2) && (del1 != '-'))
        return false;
    return true;
}

bool leap_year(int year){
    if(year % 4 == 0){
        if(year % 100 == 0){
            if(year % 400 == 0) {
                if (year % 4000 == 0)
                    return false;
                return true;
            }
            return false;
        }
        return true;
    }
    return false;
}

bool check_date_valid(struct DATE a){
    int months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(leap_year(a.m_year))
        months[1] = 29;
    if(a.m_year < 1)
        return false;
    if((a.m_month < 1) || (a.m_month > 12))
        return false;
    if((a.m_day > months[a.m_month - 1]) || (a.m_day < 0))
        return false;
    return true;
}

void print_date(struct DATE d){
    printf("%04d-%02d-%02d", d.m_year, d.m_month, d.m_day);
}

void print_interval(struct DATE* a, int* b, char** c, size_t to, size_t from, int result, bool full){
    print_date(a[from]);
    printf(" - ");
    print_date(a[to]);
    printf(": %d\n", result);
    if(full){
        for(size_t i = from; i <= to; i++){
            printf("  %d: %s\n", b[i], c[i]);
        }
    }
}

int find_interval(struct DATE* dates, int* num, size_t size, const size_t* from, size_t* to, int ref){
    int result = 0;
    bool break_flag = false;
    for(size_t i = (*from); i < size; i++){
        if(i == 0){
            result += num[i];
        }else{
            struct DATE current_date = dates[i-1];
            while(equal_dates(current_date, dates[i]) && i < size){
                result += num[i++];
            }
            if((i < size) && (result != 0) && (result + num[i] > ref)){
                break_flag = true;
                if(abs(ref - result) < abs(ref - (result + num[i]))){
                    *to = i;
                    break;
                }
            }
            result += num[i];
        }
    }
    if(!break_flag)
        (*to) = size - 1;
    return result;
}

bool read(struct DATE* a, int* b, char** c, size_t* index){
    struct DATE new_date;
    if(!read_date(&new_date))
        return false;
    if(!check_date_valid(new_date))
        return false;
    if(((*index) > 0) && !more_than(a[(*index) - 1], new_date))
        return false;
    int num;
    if(scanf(" %d ", &num) != 1)
        return false;
    if(num < 1)
        return false;

    size_t buffer_size = 4096;
    char* buffer = (char*)malloc(sizeof(char) * buffer_size);
    if(scanf("%4095s", buffer) != 1)
        return false;
    a[(*index)] = new_date;
    b[(*index)] = num;
    c[(*index)++] = buffer;
    return true;
}

void delete_d(struct DATE* a, int* b, char** c, size_t size){
    for(size_t i = 0; i < size; i++)
        free(c[i]);
    free(a);
    free(b);
    free(c);
}

bool interval(struct DATE* date_array, int* num_array, char** text_array, size_t size, bool flag){
    size_t to = 0, from = 0;
    int res = 0, ref;
    if(scanf(" %d ", &ref) != 1){
        delete_d(date_array, num_array, text_array, size);
        printf("Nespravny vstup.\n");
        return false;
    }
    res = find_interval(date_array, num_array, size, &from, &to, ref);
    for(size_t i = 1; i < size; i++){
        struct DATE current_date = date_array[i-1];
        if(!equal_dates(current_date, date_array[i])){
            size_t tmp_to = i, tmp_from = i;
            int tmp = find_interval(date_array, num_array, size, &tmp_from, &tmp_to, ref);
            if(abs(ref - tmp) < abs(ref-res) ){
                res = tmp;
                to = tmp_to;
                from = tmp_from;
            }else if(abs(ref - tmp) == abs(ref - res)){
                if(to < tmp_to){
                    res = tmp;
                    to = tmp_to;
                    from = tmp_from;
                }else if(to == tmp_to){
                    if(tmp > res){
                        res = tmp;
                        from = tmp_from;
                    }
                }
            }
        }
    }
    print_interval(date_array, num_array, text_array, to, from, res, flag);
    return true;
}

int main() {
    size_t size = 2, index = 0;
    struct DATE* date_array = (struct DATE*)calloc(size, sizeof (struct DATE));
    int* num_array = (int*)calloc(size, sizeof(int));
    char** text_array = (char**)calloc(size, sizeof(char*));
    char action;
    printf("Recenze:\n");
    while(scanf(" %c ", &action) == 1){
        if(action == EOF)
            break;
        switch (action) {
            case '+':
                if(index + 1 == size)
                    array_extension(&date_array, &num_array, &text_array, &size);
                if(!read(date_array, num_array, text_array, &index)) {
                    delete_d(date_array, num_array, text_array, index);
                    printf("Nespravny vstup.\n");
                    return 1;
                }
                break;
            case '?': {
                if(!interval(date_array, num_array, text_array, index, true))
                    return 1;
                break;
            }
            case '#':
                if(!interval(date_array, num_array, text_array, index, false))
                    return 1;
                break;
            default:
                delete_d(date_array, num_array, text_array, index);
                return 1;
        }
    }
    delete_d(date_array, num_array, text_array, index);
    return 0;
}
