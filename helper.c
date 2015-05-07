#include <assert.h>
#include <stdio.h>

#include "helper.h"

double _get_by_func(csv_t *csv, int column_number, double (*get)(double , double)) {
    int column_index = column_number - 1;
    double selected = csv->vals[0][column_index];
    int i;

    for (i = 1; i < csv->nrows; ++i) {
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

void _init_bucket_t(csv_t *csv, bucket_t *bucket, int column_number) {
    int i;
    double min = _get_by_func(csv, column_number, double_min);
    double max = _get_by_func(csv, column_number, double_max);
    double step = (max + EPSILON - (min - EPSILON)) / GRAPHROWS;

    bucket->start = min;
    bucket->step = step;

    for (i = 0; i < GRAPHROWS; ++i) {
        bucket->buckets[i] = 0;
    }
}

int _calculate_bucket_index(double value, double min, double step) {
    return (int) ((value - min) / step);
}

void _populate_bucket(csv_t *csv, bucket_t *bucket, int column_number) {
    int i;
    int bucket_index;
    int column_index = column_number - 1;

    for (i = 0; i < csv->nrows; ++i) {
        bucket_index = _calculate_bucket_index(csv->vals[i][column_index],
            bucket->start, bucket->step);
        ++(bucket->buckets[bucket_index]);
    }
}

void _print_bucket_graph(csv_t *csv, bucket_t *bucket, int column_number) {
    int i, j;
    int scale = 1;
    string header = csv->labs[column_number - 1];

    printf("graph of %s, scaled by a factor of %d\n", header, scale);
    for (i = GRAPHROWS - 1; i >= 0; --i) {
        printf("%6.2f--%6.2f [%3d]:", bucket->start + i * bucket->step,
            bucket->start + (i + 1) * bucket->step, bucket->buckets[i]);

        for (j = 0; j < bucket->buckets[i]; ++j) {
            printf("*");
        }
        printf("\n");
    }
}
