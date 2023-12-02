#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void extension(size_t * size, char*** a, char*** b, size_t** c, size_t** d){
    *size *= 2;
    (*a) = (char**) realloc((*a), *size * sizeof(char*));
    (*b) = (char**) realloc((*b), *size * sizeof(char*));
    (*c) = (size_t*) realloc((*c), *size * sizeof(size_t));
    (*d) = (size_t*) realloc((*d), *size * sizeof(size_t));
}

void del(size_t size, char** a, char** b, size_t* c, size_t* d){
    for(size_t i = 0; i < size; i++){
        free(a[i]);
        free(b[i]);
    }
    free(a);
    free(b);
    free(c);
    free(d);
}

bool control_phone(char a){
    return (a >= 48) && (a <= 57);
}

bool control_name(char a){
    return ((a >= 65) && (a <= 90)) || ((a >= 97) && (a <= 122)) || (a == ' ');
}

bool get_phone(const char* line, size_t* index, size_t size_of_line, char** phone_num, size_t* size_of_phone, bool flag){
    if((size_of_line < 1) || ((*index) + 1 > size_of_line))
        return false;
    bool phone = false;
    (*phone_num) = (char*) calloc((*size_of_phone), sizeof(char));
    if(line[(*index)++] == ' ')
        phone = true;
    while(phone){
        if((*index) >= size_of_line){
            free(*phone_num);
            *phone_num = NULL;
            return false;
        }
        if(line[*index] == ' ' || line[*index] == '\n') {
            if(*index == 1) {
                free(*phone_num);
                *phone_num = NULL;
                return false;
            }
            break;
        }
        if((*index) + 1 > (*size_of_phone)) {
            if(flag) {
                free(*phone_num);
                *phone_num = NULL;
                return false;
            }
            *size_of_phone *= 2;
            (*phone_num) = (char*) realloc(*phone_num, (*size_of_phone) * sizeof(char));
        }
        if(!control_phone(line[*index])) {
            free(*phone_num);
            *phone_num = NULL;
            return false;
        }
        (*phone_num)[(*index) - 1] = line[*index];
        (*index)++;
    }
    if(!phone || (!flag && line[(*index)] != '\n')){
        free(*phone_num);
        *phone_num = NULL;
        return false;
    }
    (*phone_num)[(*index) - 1] = '\0';
    return phone;
}

bool parse_line(const char* line, ssize_t size_of_line, char** phones,
                char** names, size_t* phone_length, size_t* name_length){
    if(size_of_line < 1)
        return false;
    if(line[size_of_line - 1] != '\n')
        return false;
    size_t index = 0;
    char* phone_num = NULL;
    size_t size_of_phone = 21;
    bool phone = get_phone(line, &index, size_of_line, &phone_num, &size_of_phone, true);
    if(!phone)
        return false;
    bool name = true;
    *phone_length = index - 1;
    index++;
    if(line[index] == ' '){
        free(phone_num);
        return false;
    }
    size_t string_length = 2, string_current = 0;
    char* string = (char*)calloc(string_length, sizeof(char));
    while(name){
        if(line[index] == '\n') {
            if(string_current == 0) {
                free(string);
                free(phone_num);
                return false;
            }
            break;
        }
        if(!control_name(line[index])) {
            free(string);
            free(phone_num);
            return false;
        }
        if(string_current + 1 == string_length) {
            string_length *= 2;
            string = (char*) realloc(string, string_length * sizeof(char));
        }
        string[string_current++] = line[index++];
    }
    if(string[string_current - 1] == ' '){
        free(string);
        free(phone_num);
        return false;
    }
    string[string_current] = '\0';
    *names = string;
    *phones = phone_num;
    *name_length = string_current;
    return name && phone;
}

bool exist(char** names, char** phones, size_t size, char* name_l, char* phone_l){
    for(size_t i = 0; i < size; i++){
        if((strcmp(names[i], name_l) == 0) && (strcmp(phones[i], phone_l) == 0))
            return true;
    }
    return false;
}

bool equal_phone_chars(char a, char b){
    return a == b;
}

bool equal_name_chars(char a, char b){
    switch (a) {
        case '1':
            return b == 32;
        case '2':
            return ((b >= 65) && (b <= 67)) || ((b >= 97) && (b <= 99));
        case '3':
            return ((b >= 68) && (b <= 70)) || ((b >= 100) && (b <= 102));
        case '4':
            return ((b >= 71) && (b <= 73)) || ((b >= 103) && (b <= 105));
        case '5':
            return ((b >= 74) && (b <= 76)) || ((b >= 106) && (b <= 108));
        case '6':
            return ((b >= 77) && (b <= 79)) || ((b >= 109) && (b <= 111));
        case '7':
            return ((b >= 80) && (b <= 83)) || ((b >= 112) && (b <= 115));
        case '8':
            return ((b >= 84) && (b <= 86)) || ((b >= 116) && (b <= 118));
        case '9':
            return ((b >= 87) && (b <= 90)) || ((b >= 119) && (b <= 122));;
        default:
            return false;
    }
    return false;
}

size_t find(char** names, size_t* names_size, char** phones, size_t* phone_size,
            size_t size, size_t** val, char* line, size_t line_size){
    size_t result = 0;
    for(size_t i = 0; i < size; i++){
        bool flag = true;
        if(line_size <= phone_size[i]) {
            for (size_t j = 0; j < line_size; j++) {
                if(!equal_phone_chars(phones[i][j], line[j])){
                    flag = false;
                    break;
                }
            }
        }else
            flag = false;
        if((!flag) && (line_size <= names_size[i])) {
            flag = true;
            for (size_t j = 0; j < line_size; j++) {
                if(!equal_name_chars(line[j], names[i][j])){
                    flag = false;
                    break;
                }
            }
        }
        if(flag){
            result++;
            if(result < 11)
                (*val)[result-1] = i;
        }
    }
    return result;
}

void print_result(char ** names, char ** phones, size_t* val, size_t size){
    if(size < 11){
        for(size_t i = 0; i < size; i++){
            printf("%s %s\n", phones[(val[i])], names[(val[i])]);
        }
    }
    printf("Celkem: %lu\n", size);
}

int main() {
    size_t size = 2, index = 0;
    char** phones = (char**) calloc(size, sizeof(char*));
    char** names = (char**) calloc(size, sizeof(char*));
    size_t* size_of_phones = (size_t*) calloc(size, sizeof(size_t));
    size_t* size_of_names = (size_t*) calloc(size, sizeof(size_t));
    char control;
    while(scanf("%c", &control) == 1){
        if(control == EOF){
            return 0;
        }
        if(index + 1 == size)
            extension(&size, &phones, &names, &size_of_names, &size_of_phones);
        char* line = NULL;
        size_t line_length = 0;
        ssize_t read_l;
        read_l = getline(&line, &line_length, stdin);
        if(read_l == -1) {
            free(line);
            del(size, phones, names, size_of_phones, size_of_names);
            printf("Nespravny vstup.\n");
            return 1;
        }
        switch (control) {
            case '?': {
                size_t size_of_phone = 2, looking_index = 0;
                char* looking_phone = NULL;
                size_t* print_arr = (size_t*)calloc(10, sizeof(size_t));
                if(get_phone(line, &looking_index, read_l, &looking_phone, &size_of_phone, false)) {
                    looking_phone[--looking_index] = '\0';
                    size_t result = find(names, size_of_names, phones, size_of_phones, index, &print_arr, looking_phone,
                                         looking_index);
                    print_result(names, phones, print_arr, result);
                }else {
                    printf("Nespravny vstup.\n");
                }
                if(looking_phone != NULL)
                    free(looking_phone);
                free(print_arr);
                break;
            }
            case '+': {
                char* name_l = NULL,* phone_l = NULL;
                size_t size_name, size_phone;
                if(parse_line(line, read_l, &phone_l, &name_l, &size_phone, &size_name)) {
                    if (!exist(names, phones, index, name_l, phone_l)) {
                        size_of_names[index] = size_name;
                        size_of_phones[index] = size_phone;
                        names[index] = name_l;
                        phones[index++] = phone_l;
                        printf("OK\n");
                    } else {
                        free(name_l);
                        free(phone_l);
                        printf("Kontakt jiz existuje.\n");
                    }
                }else{
                    printf("Nespravny vstup.\n");
                }
                break;
            }
            default:
                printf("Nespravny vstup.\n");
                break;
        }
        free(line);
    }
    del(index, phones, names, size_of_phones, size_of_names);
    return 0;
}
