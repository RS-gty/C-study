#include <stdio.h>
#include <stdlib.h>

// Basic type definition
typedef struct ex{
    char content[1000];
} Expression;

typedef struct po {
    int power[11];
} Polynomial;

// String utils

int get_length(const char* string) {
    int length = 0;
    while (string[length] != '\0') {
        length++;
    }
    return length;
}

int* sep_by(const char* string, char separator, int begin_index, int end_index) {
    int index1=begin_index-1, index2=0;
    int trigger = 0;
    for (int i = begin_index; i <= end_index; i++) {
        if (string[i] != separator || trigger == 1) {
            if (!trigger) {
                index1 = i;
            }
        } else {
            trigger = 1;
            index2 = i + 1;
        }
    }
    int* list = (int*)malloc(sizeof(int)*4);
    list[0] = begin_index;
    list[1] = index1;
    list[2] = index2;
    list[3] = get_length(string)-1;

    return list;
}

void replace(char* string, int begin_index, int end_index, const char* new_content) {
    int gap = end_index - begin_index + 1;
    int new_length = get_length(new_content);
    int move = new_length - gap;
    if (move > 0) {
        for (int i = get_length(string); i >= end_index; i--) {
            string[i + move] = string[i];
        }
    }
    if (move < 0) {
        for (int i = end_index; i <= get_length(string); i++) {
            string[i + move] = string[i];
        }
    }
    for (int i = 0; i < new_length; i++) {
        string[begin_index+i] = new_content[i];
    }
}

void remove_spaces(char* string) {
    int shift = 0;
    for (int i = 0; i < get_length(string); i++) {
        while (string[i + shift] == ' ') {shift++;}
        string[i] = string[i + shift];
    }
}

// Condition checks

int is_part_length_zero(const int* list) {
    return (list[1]-list[0] < 0 || list[3]-list[2] < 0);
}

int is_number(const char* string, int begin_index, int end_index) {
    for (int i = begin_index; i <= end_index; i++) {
        if (string[i] < '0' || string[i] > '9') {
            return 0;
        }
    }
    return 1;
}

int is_alpha(const char* string, int begin_index, int end_index) {
    if (end_index - begin_index != 0) {
        return 0;
    }
    if (string[begin_index] == 'a') {
        return 1;
    }
    return 0;
}

int is_pow_of_a(const char* string, int begin_index, int end_index) {
    int* list = sep_by(string, '^', begin_index, end_index);
    if (is_part_length_zero(list)) {
        return 0;
    }
    if (is_alpha(string, list[0], list[1]) && is_number(string, list[2], list[3])) {
        return 1;
    }
    return 0;
}

int is_num_times_a(const char* string, int begin_index, int end_index) {
    int* list = sep_by(string, '*', begin_index, end_index);
    if (is_part_length_zero(list)) {
        return 0;
    }
    if (is_number(string, list[0], list[1]) && is_pow_of_a(string, list[2], list[3])) {
        return 1;
    }
    if (is_pow_of_a(string, list[0], list[1]) && is_number(string, list[2], list[3])) {
        return 1;
    }
    return 0;
}


int main() {
    char string[1000] = "((a^1) ^ 2)^3";
    char new[10] = "u";
    remove_spaces(string);
    replace(string, 2, 3, new);
    printf("%d\n", is_num_times_a("1222323233*a^123", 0, 17));

    return 0;
}



