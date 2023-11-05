#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int convert(char ch){
    if((ch < 97 || ch > 122) && ch != '|')
        return 0;
    else if(ch == '|')
        return -1;
    else
        return (int)pow(2, ch - 97);
}

int read_string(long long int* length_prev, long long int* length_msg, char c){
    int stage = 0, convert_ = -2;
    *length_msg = 0;
    *length_prev = 0;
    do{
        convert_ = convert(c);
        if(c == '\n')
            break;
        if(convert_ == 0)
            return 1;
        else if(convert_ == -1)
            stage++;
        if(stage == 0){
            *length_prev += convert_;
        }else if(stage == 1){
            if(convert_ > 0)
                *length_msg += convert_;
        } else
            return 1;
    }while(scanf("%c",&c) == 1);
    if(stage != 1 || ((*length_prev) == 0 && (*length_msg) == 0))
        return 1;
    return 0;
}

void reallocation(long long int** a, long long int** b, size_t* size){
    *size *= 2;
    (*a) = (long long int*)realloc(*a, (*size) * sizeof (long long int));
    (*b) = (long long int*)realloc(*b, (*size) * sizeof (long long int));
}

int check_remains(long long int** remains, long long int* elements, size_t size){
    for(size_t i = 0; i < size; i++){
        long long int rem;
        if(i == 0)
            rem = elements[0] - elements[size-1];
        else
            rem = elements[i] - elements[i-1];
        if(rem < 0)
            rem *= -1;
        if(remains[i][rem] < 4)
            return 0;
    }
    return 1;
}

long long int lcm(long long int* msg, long long int* elements, size_t size){
    long long int ** remains = (long long int**)calloc(size, sizeof(long long int*));
    long long int size_of_remains = msg[0], result = -1, new_result = 0, operation = 1;
    for(size_t i = 1; i < size; i++)
        if(msg[i] > size_of_remains)
            size_of_remains = msg[i];
    for(size_t i = 0; i < size; i++)
        remains[i] = (long long int*)calloc(size_of_remains + 1, sizeof(long long int));
    while(operation){
        result = new_result;
        operation = 0;
        for(size_t i = 0; i < size; i++){
            if(msg[i] == 0)
                elements[i] = result;
            if(result < elements[i])
                if(result < elements[i]) {
                    operation = 1;
                    new_result = elements[i];
                }
            if(result > elements[i]) {
                operation = 1;
                elements[i] += msg[i];
            }
        }
        if(result != new_result && result != 0) {
            for (size_t i = 0; i < size; i++) {
                long long int rem;
                if (i == 0)
                    rem = elements[0] - elements[size - 1];
                else
                    rem = elements[i] - elements[i - 1];
                if (rem < 0)
                    rem *= -1;
                remains[i][rem]++;
                if (remains[i][rem] > 1) {
                    if (check_remains(remains, elements, size)) {
                        result = -1;
                        operation = 0;
                        break;
                    }
                }
            }
        }
    }
    for(size_t i = 0; i < size; i++)
        free(remains[i]);
    free(remains);
    return result;
}

void freear(long long int** a, long long int** b){
    free(*a);
    free(*b);
}

int main() {
    size_t i = 0, size = 2;
    long long int* arr_msg = (long long int*)calloc(size, sizeof (long long int));
    long long int* elements = (long long int*)calloc(size, sizeof(long long int));
    char c;
    printf("Zpravy:\n");
    while((c = (char)getc(stdin)) != EOF) {
        long long int prev;
        if( i == size)
            reallocation(&arr_msg, &elements, &size);
        prev = 0;
        arr_msg[i] = 0;
        if (read_string(&prev, &arr_msg[i], c) ) {
            freear(&arr_msg, &elements);
            printf("Nespravny vstup.\n");
            return 1;
        }
        if(arr_msg[i] != 0)
            arr_msg[i] += prev;
        if(prev == 0)
            arr_msg[i] = 0;
        elements[i] = (prev + arr_msg[i]);
        i++;
    }
    long long int result = lcm(arr_msg, elements, i);
    if(result == -1)
        printf("Nelze dosahnout.\n");
    else
        printf("Synchronizace za: %lld\n", result);
    freear( &arr_msg, &elements);
    return 0;
}
