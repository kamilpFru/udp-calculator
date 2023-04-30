#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include "math.h"
#include "limits.h"

int overflow_check(int x, int y, char operation) {
    if (operation == '+') {
        if (x > 0 && y > INT_MAX - x) {
            return 0;
        }
        if (x < 0 && y < INT_MIN - x) {
            return 0;
        }
    } else {
        if (x < 0 && y > INT_MAX + x) {
            return 0;
        }
        if (x > 0 && y < INT_MIN + x) {
            return 0;
        }
    }
    return 1;
}

void calculate(char *expr, char *x) {

    size_t size = strlen(expr);
    if (size == 0) {
        perror("No arguments");
        strcpy(x, "ERROR");
        return;
    }
    // printf("%zu", size);

    if ((int)expr[0] < 48 || (int)expr[0] > 57 || (int)expr[size - 1] < 48 || (int)expr[size - 1] > 57) {
        perror("Wrong arguments");
        strcpy(x, "ERROR");
        return;
    }

    int j, l;

    for (l = 0; l < size; l++) {
        if (expr[l] == '+') {
            if (expr[l + 1] == '+' || expr[l + 1] == '-') {
                perror("Wrong operator");
                strcpy(x, "ERROR");
                return;
            }
            continue;
        }

        if (expr[l] == '-') {
            if (expr[l + 1] == '+' || expr[l + 1] == '-') {
                perror("Wrong operator");
                strcpy(x, "ERROR");
                return;
            }
            continue;
        }

        if ((int)expr[l] < 48 || (int)expr[l] > 57) {
            perror("Not a number");
            strcpy(x, "ERROR");
            return;
        }
    }

    char buf[256];
    
    memcpy(buf, expr, size);

    char operations[19];
    int operations_index = 0;

    int number_index = 0;

    for (j = 0; j < size; j++) {
        if (buf[j] == '+') {
            operations[operations_index] = '+';
            operations_index += 1;
            continue;
        }

        if (buf[j] == '-') {
            operations[operations_index] = '-';
            operations_index += 1;
            continue;
        }
    }

    char delimeters1[] = "+-";
    char * token1;
    int numbers[20];
    token1 = strtok(buf, delimeters1);

    while(token1 != NULL) {
        numbers[number_index] = atoi(token1);
        if (atof(token1) > INT_MAX || atof(token1) < INT_MIN) {
            perror("Overflow");
            strcpy(x, "ERROR");
            return;
        }
        number_index += 1;
        token1 = strtok(NULL, delimeters1);
    }

    int result = numbers[0];
    for (int l = 1; l < number_index; l++) {
        if (operations[l - 1] == '+') {
            if (overflow_check(result, numbers[l], '+') == 0) {
                perror("Overflow");
                strcpy(x, "ERROR");
                return;
            }
            result += numbers[l];
        } else {
            if (overflow_check(result, numbers[l], '-') == 0) {
                perror("Overflow");
                strcpy(x, "ERROR");
                return;
            }
            result -= numbers[l];
        }
    }

    sprintf(x, "%d", result);

    // FILE *fp = fopen("file.txt", "ab+");
    // fprintf(fp, "%d", result);
    // fclose(fp);

}
