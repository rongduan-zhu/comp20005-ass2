#include <assert.h>
#include <stdio.h>

#include "helper.h"

double _get_by_func(csv_t *csv, int column_number, double (*get)(double , double)) {
    int column_index = column_number - 1;
    double selected = csv->vals[0][column_index];
    int i;

    for (i = 1; i < csv->ncols; ++i) {
        selected = (*get)(selected, csv->vals[i][column_index]);
    }

    return selected;
}

double double_max(double a, double b) {
    return a > b ? a : b;
}

double double_min(double a, double b) {
    return a > b ? b : a;
}

double _average_csv_t(csv_t *csv, int column_number) {
    double sum = 0.0;
    int i;

    for (i = 0; i < csv->nrows; ++i) {
        sum += csv->vals[i][column_number - 1];
    }

    return sum / csv->nrows;
}

void _print_average_csv_t(csv_t *csv, int column_number, double average) {
    string header = csv->labs[column_number - 1];
    printf("average %s is %.2f (over %d values)\n",
        header, average, csv->nrows);
}
