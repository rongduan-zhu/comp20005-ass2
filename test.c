#include <assert.h>

#include "helper.h"

int main(int argc, char *argv[]) {
    assert(decorate_value(0) == '.');
    assert(decorate_value(1) == '1');
    assert(decorate_value(2) == '2');
    assert(decorate_value(3) == '2');
    assert(decorate_value(256) == '9');
    assert(decorate_value(511) == '9');
    assert(decorate_value(512) == 'a');

    return 0;
}
