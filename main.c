#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Basic type definition
typedef struct spliter {
    int parts;
    int *index[1000];
} Spliter;

typedef struct ex{
    char content[1000];
    Spliter *spliter;
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

int replace(char* string, int begin_index, int end_index, const char* new_content) {
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
    return move;
}

void remove_spaces(char* string) {
    int shift = 0;
    for (int i = 0; i < get_length(string); i++) {
        while (string[i + shift] == ' ') {shift++;}
        string[i] = string[i + shift];
    }
}

// Condition checks

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '^';
}

int is_valid_string(const char* string) {
    for (int i = 0; i < get_length(string); i++) {
        if (!(is_operator(string[i])||string[i]=='a'|| string[i]=='('|| string[i]==')')){
            return 0;
        }
        if (i == 0 && string[i] == ')') {
            return 0;
        }
    }
    return 1;
}

int is_part_length_zero(const int* list) {
    return list[1]-list[0] < 0 || list[3]-list[2] < 0;
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
    if (is_alpha(string, begin_index, end_index)) {
        return 1;
    }
    if (is_alpha(string, list[0], list[1]) && is_number(string, list[2], list[3])) {
        return 1;
    }
    free(list);
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
        free(list);
        return 1;
    }
    free(list);
    return 0;
}

int is_containing_a(const char* string, int begin_index, int end_index) {
    for (int i = begin_index; i <= end_index; i++) {
        if (string[i] == 'a') {
            return 1;
        }
    }
    return 0;
}
// Calculation

int get_numberindex(const char* string, int begin_index) {
    int i = begin_index;
    if (string[i] == '0'){return -1;}
    while (string[i] >= '0' && string[i] <= '9') {
        i++;
    }
    return i;
}

int get_integer(const char* string, int begin_index) {
    int i = begin_index;
    int number = 0;
    if (string[i] == '0'){return -1;}
    while (string[i] >= '0' && string[i] <= '9') {
        number = number * 10 + string[i] - '0';
        i++;
    }
    return number;
}

int IntegerCalculation(char operator, int n1, int n2) {
    if (operator == '+') {
        return n1 + n2;
    }
    if (operator == '-') {
        return n1 - n2;
    }
    if (operator == '*') {
        return n1 * n2;
    }
    else {
        return (int)pow(n1, n2);
    }
}

int* SecondaryCalculation(const char* string, int begin_index, int end_index) {
    int left = begin_index;
    int op1 = 1;
    int op2 = 1;
    while (left < end_index) {
        op1 = get_integer(string, left);
        left = get_numberindex(string, left);
        if (string[left+1] == '*') {}
    }


    int* list = (int*)malloc(sizeof(int)*2);

}

int* PowerCalculation(const char* string, int begin_index, int end_index) {
    int left = begin_index;
    int op = 1;
    int result = 1;
    int trigger = 0;
    if (!is_containing_a(string, begin_index, end_index)) {
        while (left <= end_index) {
            if (!trigger) {
                op = get_integer(string, left);
                result = op;
                trigger = 1;
            } else {
                op = get_integer(string, left);
                result = (int)pow(result, op);
            }
            left = get_numberindex(string, left)+1;
        }
    } else {
        left += 2;
        while (left <= end_index) {
            op = get_integer(string, left);
            result *= op;
            left = get_numberindex(string, left)+1;
        }
    }
    int* list = (int*)malloc(sizeof(int)*2);
    if (is_containing_a(string, begin_index, end_index)) {
        list[0] = 1;
        list[1] = result;
    } else {
        list[0] = result;
        list[1] = 0;
    }
    return list;
}


// Polynomial
Polynomial* StringToPolynomial(const char* string, int begin_index, int end_index) {
    Polynomial* poly = malloc(sizeof(Polynomial));
    memset(poly, 0, sizeof(Polynomial));
    for (int i = begin_index; i <= end_index; i++) {

    }
}

// Executions

int get_M_bracket(const char* string, int begin_index) {
    int i = begin_index + 1;
    int open = 1, close = 0;
    while (open != close && i <= get_length(string)) {
        if (string[i] == '(') {
            open++;
        }
        if (string[i] == ')') {
            close++;
        }
        i++;
    }
    if (open == close){return i - 1;}
    return -1;
}
/*
int* AttachBracket(const char* string, int begin_index, int end_index) {
    int left = begin_index, right = end_index;
    int open = 0, close = 0;

    if (begin_index - 2 >= 0 && string[begin_index - 2] == ')' ) {
        close = 1;
    }

    while (1) {
        if ((string[left] == '+' || string[left] == '-') && (open == close)) {
            break;
        }
        if (string[left] == '(') {
            open++;
        }
        if (string[left] == ')') {
            close++;
        }
        if (left != 0) {
            left--;
        } else {
            break;
        }
    }
    open = 0, close = 0;
    if (end_index + 2 <= get_length(string) - 1 && string[end_index + 2] == '(') {
        open = 1;
    }
    while (1) {
        if ((string[right] == '+' || string[right] == '-') && (open == close)) {
            break;
        }
        if (string[right] == '(') {
            open++;
        }
        if (string[right] == ')') {
            close++;
        }
        if (right != get_length(string)-1) {
            right++;
        } else {
            break;
        }
    }
    int* list = (int*)malloc(sizeof(int)*2);
    list[0] = left; list[1] = right;
    return list;
}
*/

int* AttachBracket2Number(const char* string, int begin_index, int end_index) {
    int left = begin_index, right = end_index;
    int symbol_count = 0;

    if (begin_index - 2 >= 0 && string[begin_index - 2] != ')') {
        while (symbol_count <= 1) {
            if ((string[left] == '+' || string[left] == '-')) {
                break;
            }
            if (is_operator(string[left])) {
                symbol_count++;
            }
            if (left != 0 && symbol_count <= 1) {
                left--;
            } else {
                if (symbol_count == 2) {
                    left++;
                }
                break;
            }
        }
    }
    symbol_count = 0;
    if (end_index + 2 <= get_length(string) - 1 && string[end_index + 2] != '(') {
        while (symbol_count <= 1) {
            if ((string[right] == '+' || string[right] == '-')) {
                break;
            }
            if (is_operator(string[right])) {
                symbol_count++;
            }
            if (right != get_length(string)-1 && symbol_count <= 1) {
                right++;
            } else {
                if (symbol_count == 2) {
                    right--;
                }
                break;
            }
        }
    }

    int* list = (int*)malloc(sizeof(int)*2);
    list[0] = left; list[1] = right;
    return list;
}

void ExpandBracket(const char* string, int begin_index, int end_index) {
    int expandable = !(string[end_index+1] == '^');
    if (expandable) {

    }
}
// TODO
void Simplify(const char* string) {

}

// Main

int main() {
    char string[1000] = "11*11*((a^1) ^ 2)*(a+(123*12)-(12))";
    char string2[1000] = "(1 + 2*a-6*a^2-a+12-2*a^2)";
    char string3[1000] = "2^3*4*5^6*7"; // 350000
    char string4[1000] = "a^3^4";
    remove_spaces(string);
    remove_spaces(string2);

    int length = get_length(string);
    printf("%d\n", get_M_bracket(string, 0));
    int * arr = AttachBracket2Number(string, 6, 14);
    int * arr2 = PowerCalculation(string4, 0, 4);

    return 0;
}
