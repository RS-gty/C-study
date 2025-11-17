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

void reverse(char* string, int begin_index, int end_index) {
    for (int i = begin_index; i < (float)(begin_index+end_index)/2; i++) {
        char temp = string[i];
        string[i] = string[end_index-(i-begin_index)];
        string[end_index-(i-begin_index)] = temp;
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

int is_containing_char(const char* string, int begin_index, int end_index, char c) {
    for (int i = begin_index; i <= end_index; i++) {
        if (string[i] == c) {
            return 1;
        }
    }
    return 0;
}

int is_inbracket(const char* string, int begin_index, int end_index) {
    for (int i = begin_index; i <= end_index; i++) {
        if (string[i] == '(' || string[i] == ')') {
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

int* PowerCalculation(const char* string, int begin_index, int end_index) {
    int left = begin_index;
    int op;
    int result = 1;
    if (!is_containing_char(string, begin_index, end_index, 'a')) {
        int trigger = 0;
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
    if (is_containing_char(string, begin_index, end_index, 'a')) {
        list[0] = 1;
        list[1] = result;
    } else {
        list[0] = result;
        list[1] = 0;
    }
    return list;
}

int* SecondaryCalculation(const char* string, int begin_index, int end_index) {
    int left = begin_index;
    int right = begin_index;
    int pow = 0;
    int op2 = 1;
    int result = 1;
    int* temp = nullptr;
    while (right <= end_index) {
        if (string[right] == '*' || right == end_index) {
            if (is_containing_char(string, left, right, '^')) {
                temp = PowerCalculation(string, left, right);
                op2 = temp[0];
                pow += temp[1];
            } else {
                if (is_containing_char(string, left, right, 'a')) {
                    pow++;
                    op2 = 1;
                } else {
                    op2 = get_integer(string, left);
                }
            }
            result *= op2;
            left = right + 1;
        }
        right ++;
    }
    int* list = (int*)malloc(sizeof(int)*2);
    list[0] = result;
    list[1] = pow;

    free(temp);
    return list;
}

// Polynomial
Polynomial* StringToPolynomial(const char* string, int begin_index, int end_index) {
    // NO BRACKETS IN ZONE
    Polynomial* poly = malloc(sizeof(Polynomial));
    memset(poly, 0, sizeof(Polynomial));
    int left = begin_index;
    int right = begin_index;
    int coefficient, pow;
    int plus = 1;
    int negative_trigger = 0;
    int* temp = nullptr;
    while (right <= end_index) {
        if (string[right] == '+' || string[right] == '-' || right == end_index) {
            plus = (left-1>=begin_index&&string[left-1] == '+' || left == begin_index)?1:0;
            if (is_containing_char(string, left, right, '*')) {
                temp = SecondaryCalculation(string, left, right);
                if (plus) {
                    poly->power[temp[1]] += temp[0];
                } else {
                    poly->power[temp[1]] -= temp[0];
                }
            } else if (is_containing_char(string, left, right, '^')) {
                temp = PowerCalculation(string, left, right);
                if (plus) {
                    poly->power[temp[1]] += temp[0];
                } else {
                    poly->power[temp[1]] -= temp[0];
                }
            } else {
                if (is_containing_char(string, left, right, 'a')) {
                    if (plus) {
                        poly->power[1] += 1;
                    } else {
                        poly->power[1] -= 1;
                    }
                } else {
                    if (string[left] == '-') {
                        negative_trigger = 1;
                        left = right + 1;
                        right++;
                        continue;
                    } else {
                        if (left == begin_index) {
                            plus = 1;
                        }
                    }
                    if (negative_trigger && left == 1) {
                        poly->power[0] -= get_integer(string, left);
                        negative_trigger = 0;
                    } else {
                        if (plus) {
                            poly->power[0] += get_integer(string, left);
                        } else {
                            poly->power[0] -= get_integer(string, left);
                        }
                    }
                }
            }
            left = right + 1;
        }
        right++;
    }
    return poly;
}

char* PolynomialToString(Polynomial* poly) {
    char* string = (char*)malloc(sizeof(char)*1000);
    memset(string, 0, sizeof(char)*1000);
    int index = 0;
    int temp = 0;
    for (int i = 10; i >= 0; i--) {
        temp = poly->power[i];
        if (temp != 0) {

            if (temp < 0 && index == 0) {
                string[index] = '-';
                index++;
            } else if (index != 0){
                string[index] = temp < 0?'-':'+';
                index++;
            }
            temp = abs(temp);
            int numbegin = index;
            while (temp >= 1) {
                string[index] = '0' + temp % 10;
                index++;
                temp = temp / 10;
            }
            reverse(string, numbegin, index-1);
            if (i != 0) {
                string[index] = '*';
                string[index+1] = 'a';
                index += 2;
                if (i != 1) {
                    string[index] = '^';
                    index++;
                    if (i != 10) {
                        string[index] = '0' + i;
                        index ++;
                    } else {
                        string[index+1] = '1';
                        string[index+2] = '0';
                        index += 2;
                    }
                }
            }


        }
    }
    return string;
}

void PolynomialPlus(Polynomial* poly, Polynomial* other) {
    for (int i = 0; i < 11; i++) {
        poly->power[i] += other->power[i];
    }
}

void PolynomialMultiplyInt(Polynomial *poly, int mul) {
    for (int i = 10; i >= 0; i--) {
        poly->power[i] *= mul;
    }
}

void PolynomialMultiplyPolynomial(Polynomial *poly, Polynomial *other) {
    int temp = 0;
    int* arr = (int*)malloc(sizeof(int)*11);
    for (int i = 10; i >= 0; i--) {
        temp = 0;
        for (int j = 0; j <= i; j++) {
            temp += poly->power[j] * other->power[i-j];
        }
        arr[i] = temp;
    }
    for (int i = 0; i < 11; i++) {
        poly->power[i] = arr[i];
    }
    free(arr);
}

void PolynomialPower(Polynomial *poly, int power) {
    Polynomial *temp = (Polynomial*)malloc(sizeof(Polynomial));
    for (int i = 10; i >= 0; i--) {
        temp->power[i] = poly->power[i];
    }
    if (power == 0) {
        for (int i = 10; i >= 0; i--) {
            poly->power[i] = 0;
        }
        poly->power[0] = 1;
    } else if (power == 1){return;}
    for (int i = 1; i < power; i++) {
        PolynomialMultiplyPolynomial(poly, temp);
    }
    free(temp);
}

// Bracket executions

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

}
// TODO
Polynomial* Simplify(const char* string, int begin_index, int end_index) {
    Polynomial* op = (Polynomial*)malloc(sizeof(Polynomial));
    Polynomial* result = (Polynomial*)malloc(sizeof(Polynomial));
    memset(op, 0, sizeof(Polynomial));
    memset(result, 0, sizeof(Polynomial));
    int first_negative = string[begin_index] == '-' ? 1 : 0;
    int init = 1;
    int left = begin_index + first_negative, right = left;
    int open = 0, close = 0;
    int operation = 0;
    // Range Check
    if (begin_index > end_index){return result;}
    // Directly Calculate
    if (!is_inbracket(string, begin_index, end_index)) {
        return StringToPolynomial(string, begin_index, end_index);
    }
    // Remove Extra Brackets
    if (string[begin_index] == '(' && get_M_bracket(string, begin_index) == end_index) {
        return Simplify(string, begin_index+1, end_index-1);
    }

    // Split & Calculate
    int has_outer_PM = 0;
    // Do when plus / minus outside
    while (right <= end_index) {
        if (string[right] == '('){open++;}
        if (string[right] == ')'){close++;}
        if ((open == close && (string[right] == '+' || string[right] == '-')) || right == end_index) {
            if (string[right] == '+' || string[right] == '-') {
                has_outer_PM = 1;
            }
            if (has_outer_PM) {
                op = Simplify(string, left, (right == end_index) ? end_index : right-1);
                if (init) {
                    if (first_negative) {
                        PolynomialMultiplyInt(op, -1);
                        first_negative = 0;
                    }
                    PolynomialPlus(result, op);
                    init = 0;
                } else {
                    operation = string[left-1] == '+' ? 1 : -1;
                    PolynomialMultiplyInt(op, operation);
                    PolynomialPlus(result, op);
                }
            }
            left = right+1;
        }
        right++;
    }
    // else (only include "Expression" "Multiply" "Power"
    int has_outer_multiply = 0;
    if (!has_outer_PM) {
        open = 0, close = 0;
        init = 1;
        left = begin_index + first_negative, right = left;
        while (right <= end_index) {
            if (string[right] == '('){open++;}
            if (string[right] == ')'){close++;}
            if ((open == close && (string[right] == '*')) || right == end_index) {
                if (string[right] == '*') {
                    has_outer_multiply = 1;
                }
                if (has_outer_multiply) {
                    op = Simplify(string, left, (right == end_index) ? end_index : right-1);
                    if (init) {
                        if (first_negative) {
                            PolynomialMultiplyInt(op, -1);
                            first_negative = 0;
                        }
                        PolynomialPlus(result, op);
                        init = 0;
                    } else {
                        PolynomialMultiplyPolynomial(result, op);
                    }
                }
                left = right+1;
            }
            right++;
        }
    }

    //else (only include "Expression" "power")
    if (!has_outer_PM && !has_outer_multiply && is_containing_char(string, begin_index, end_index, '^')) {
        open = 0, close = 0;
        init = 1;
        left = begin_index + first_negative, right = left;
        while (right <= end_index) {
            if (string[right] == '('){open++;}
            if (string[right] == ')'){close++;}
            if ((open == close && (string[right] == '^')) || right == end_index) {
                op = Simplify(string, left, (right == end_index) ? end_index : right-1);
                if (init) {
                    if (first_negative) {
                        PolynomialMultiplyInt(op, -1);
                        first_negative = 0;
                    }
                    PolynomialPlus(result, op);
                    init = 0;
                } else {
                    PolynomialPower(result, op->power[0]);
                }
                left = right+1;
            }
            right ++;
        }
    }
    return result;


}

// Main

int main() {
    char string[1000] = "a^2 + 2 * a * 1 + 1^2 + 10 -10 +a -a"; // 5a+3
    char string2[1000] = "1";
    char string3[1000] = "1";
    char string4[1000] = "(a-1)^2+1+11*((a^1) ^ 2)+(a+(123*12)-(12))";
    remove_spaces(string);
    remove_spaces(string2);

    int length = get_length(string);
    printf("%d\n", get_M_bracket(string, 0));
    Polynomial* poly3 = Simplify(string, 0, get_length(string)-1);
    printf("%s\n", PolynomialToString(poly3));

    return 0;
}
