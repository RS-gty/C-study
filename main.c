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

char** sep_by(const char* string, char separator) {
    int length = get_length(string);
    char part1[length], part2[length];
    int index1=0, index2=0;
    int trigger = 0;
    for (int i = 0; i < length; i++) {
        if (string[i] != separator || trigger == 1) {
            if (!trigger) {
                part1[index1] = string[i];
                index1 ++;
            } else {
                part1[index1] = '\0';
                part2[index2] = string[i];
                index2 ++;
            }
        } else {
            trigger = 1;
        }
    }
    part2[index2] = '\0';
    char **result = (char**)malloc(sizeof(char) * length * 2);
    result[0] = part1;
    result[1] = part2;
    return result;
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

int is_number(const char* string) {
    int length = get_length(string);
    for (int i = 0; i < length; i++) {
        if (string[i] < '0' || string[i] > '9') {
            return 0;
        }
    }
    return 1;
}

int is_pow_of_a(const char* string) {
    char **arr = sep_by(string, '^');
    if (!(get_length(arr[0])* get_length(arr[1]))) {
        free(arr);
        return 0;
    }
    if (arr[0] == "a" && is_number(arr[1])) {
        free(arr);
        return 1;
    }
    return 0;
}

int is_num_times_a(const char* string) {
    char **arr = sep_by(string, '*');
    if (!(get_length(arr[0])* get_length(arr[1]))) {
        free(arr);
        return 0;
    }
    if (is_number(arr[0]) && is_pow_of_a(arr[1])) {
        free(arr);
        return 1;
    }
    if (is_pow_of_a(arr[0]) && is_number(arr[1])) {
        free(arr);
        return 1;
    }
    return 0;
}


int main() {
    char string[1000] = "((a^1) ^ 2)^3";
    char new[10] = "u";
    remove_spaces(string);
    replace(string, 2, 3, new);

    return 0;
}



