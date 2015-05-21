#include <assert.h>
#include <stdio.h>

#include "helper.h"

int main(int argc, char *argv[]) {
    printf("Output: %c, Expected: %c\n", decorate_value(0), '.');
    assert(decorate_value(0) == '.');
    printf("Output: %c, Expected: %c\n", decorate_value(1), '1');
    assert(decorate_value(1) == '1');
    printf("Output: %c, Expected: %c\n", decorate_value(2), '1');
    assert(decorate_value(2) == '1');
    printf("Output: %c, Expected: %c\n", decorate_value(3), '2');
    assert(decorate_value(3) == '2');
    printf("Output: %c, Expected: %c\n", decorate_value(4), '2');
    assert(decorate_value(4) == '2');
    printf("Output: %c, Expected: %c\n", decorate_value(5), '2');
    assert(decorate_value(5) == '2');
    printf("Output: %c, Expected: %c\n", decorate_value(6), '2');
    assert(decorate_value(6) == '2');
    printf("Output: %c, Expected: %c\n", decorate_value(7), '3');
    assert(decorate_value(7) == '3');
    printf("Output: %c, Expected: %c\n", decorate_value(14), '3');
    assert(decorate_value(14) == '3');
    printf("Output: %c, Expected: %c\n", decorate_value(15), '4');
    assert(decorate_value(15) == '4');
    printf("Output: %c, Expected: %c\n", decorate_value(30), '4');
    assert(decorate_value(30) == '4');
    printf("Output: %c, Expected: %c\n", decorate_value(256), '8');
    assert(decorate_value(256) == '8');
    printf("Output: %c, Expected: %c\n", decorate_value(511), '9');
    assert(decorate_value(511) == '9');
    printf("Output: %c, Expected: %c\n", decorate_value(1022), '9');
    assert(decorate_value(1022) == '9');
    printf("Output: %c, Expected: %c\n", decorate_value(1023), 'a');
    assert(decorate_value(1023) == 'a');
    printf("Output: %c, Expected: %c\n", decorate_value(2046), 'a');
    assert(decorate_value(2046) == 'a');
    printf("Output: %c, Expected: %c\n", decorate_value(2047), 'b');
    assert(decorate_value(2047) == 'b');

    return 0;
}
