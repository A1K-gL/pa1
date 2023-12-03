#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct WORD{
    char* word_ptr;
    size_t size;
} word;

//compare function for qsort
int compare_words(const void* a, const void* b){
    word left = *(const word*) a;
    word right = *(const word*) b;
    size_t min = (left.size > right.size) ? right.size : left.size;
    size_t max = (left.size < right.size) ? right.size : left.size, i = 0;
    for(; i < min; i++){
        if(tolower(left.word_ptr[i]) != tolower(right.word_ptr[i])){
            if(tolower(left.word_ptr[i]) < tolower(right.word_ptr[i]))
                return 1;
            else
                return -1;
        }
    }
    word word_max = left;
    word word_min = right;
    int result = 1;
    if(right.word_ptr[i] != '\0') {
        word_max = right;
        word_min = left;
        result *= -1;
    }
    for(; i < (max+min); i++){
        if(tolower(word_max.word_ptr[i%max]) != tolower(word_min.word_ptr[i%min])){
            if(tolower(word_max.word_ptr[i%max]) < tolower(word_min.word_ptr[i%min]))
                return result;
            else
                return result * -1;
        }
    }
    if(left.size < right.size)
        return 1;
    else
        return -1;
}
//print output
void print_words(const word* a, size_t size){
    size_t print_chars = 0;
    for(size_t i = 0; i < size; i++){
        printf("%s", a[i].word_ptr);
        print_chars += a[i].size;
        if((i + 1 == size) || (print_chars + a[i+1].size >= 80)) {
            print_chars = 0;
            printf("\n");
        }
        else {
            printf(" ");
            print_chars++;
        }
    }
}
//de-allocation word array
void dealloc(word* a, size_t size){
    for(size_t i = 0; i < size; i++){
        free(a[i].word_ptr);
    }
    free(a);
}
//check if word has already appeared
bool find_word(const word* a, size_t size, char* new_word){
    for(size_t i = 0; i < size; i++){
        for(size_t j = 0; ; j++){
            if((a[i].word_ptr[j] == '\0') && (new_word[j] == '\0'))
                return true;
            if(tolower(a[i].word_ptr[j]) != tolower(new_word[j]))
                break;
        }
    }
    return false;
}
int main() {
    printf("Slova:\n");
    size_t capacity = 2, fill = 0, word_capacity = 5, word_fill = 0;
    int read_characters = 0;
    word* words = (word*)calloc(capacity, sizeof(word));
    char* tmp_word = (char*)calloc(word_capacity, sizeof(char));
    char tmp;
    while(scanf("%c", &tmp) == 1){
        if(feof(stdin))
            break;
        //new word or add character to current
        if((tmp == ' ') || (tmp == '\n') || (tmp == '\t')){
            if(read_characters > 0) {
                tmp_word[word_fill] = '\0';
                if (find_word(words, fill, tmp_word)) {
                    printf("Nespravny vstup.\n");
                    dealloc(words, fill);
                    free(tmp_word);
                    return 1;
                }
                words[fill].size = read_characters;
                words[fill].word_ptr = tmp_word;
                tmp_word = (char *) calloc(word_capacity, sizeof(char));
                fill++;
                //text array capacity extension
                if (fill + 1 == capacity) {
                    capacity *= 2;
                    words = (word *) realloc(words, capacity * sizeof(word));
                }
                word_capacity = 2;
                word_fill = 0;
                read_characters = 0;
            }
        }
        else {
            tmp_word[word_fill++] = tmp;
            read_characters++;
        }
        //word array extension
        if(word_fill + 2 >= word_capacity){
            word_capacity *= 2;
            tmp_word = (char*) realloc(tmp_word, word_capacity * sizeof(char));
        }
    }
    //empty input
    if(words[0].size == 0){
        free(tmp_word);
        dealloc(words, fill);
        printf("Nespravny vstup.\n");
        return 1;
    }
    qsort(words, fill, sizeof(word), compare_words);
    print_words(words, fill);
    dealloc(words, fill);
    free(tmp_word);
    return 0;
}
