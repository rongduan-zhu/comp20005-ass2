/*
    Author: Rongduan Zhu (584 169)
*/

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "helper.h"

double get_by_func(csv_t *csv, int column_number,
        double (*get)(double , double)) {
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

int int_max(int a, int b) {
    return a > b ? a : b;
}

double average_csv_t(csv_t *csv, int column_number) {
    double sum = 0.0;
    int i;

    for (i = 0; i < csv->nrows; ++i) {
        sum += csv->vals[i][column_number - 1];
    }

    return sum / csv->nrows;
}

void print_average_csv_t(csv_t *csv, int column_number, double average) {
    string header = csv->labs[column_number - 1];
    printf("average %s: %.2f (over %d values)\n",
        header, average, csv->nrows);
}

void init_bucket_t(csv_t *csv, bucket_t *bucket, int column_number) {
    int i;
    double min = get_by_func(csv, column_number, double_min);
    double max = get_by_func(csv, column_number, double_max);
    double step = (max + EPSILON - (min - EPSILON)) / GRAPHROWS;

    bucket->start = min - EPSILON;
    bucket->step = step;
    bucket->max_bucket = 0;

    for (i = 0; i < GRAPHROWS; ++i) {
        bucket->buckets[i] = 0;
    }
}

int calculate_bucket_index(double value, double min, double step) {
    return (int) ((value - min) / step);
}

void populate_bucket(csv_t *csv, bucket_t *bucket, int column_number) {
    int i;
    int bucket_index;
    int column_index = column_number - 1;

    for (i = 0; i < csv->nrows; ++i) {
        bucket_index = calculate_bucket_index(csv->vals[i][column_index],
            bucket->start, bucket->step);
        ++(bucket->buckets[bucket_index]);
        bucket->max_bucket = int_max(bucket->max_bucket,
            bucket->buckets[bucket_index]);
    }
}

void print_bucket_graph(csv_t *csv, bucket_t *bucket, int column_number) {
    int i, j;
    int scale;
    string header = csv->labs[column_number - 1];

    /* Second part of the addition deals with inexact division. For example
       when 60 / 60, it equals to 1, the second part of the equation will
       evaluate to 0, which gives a scale of 1 and its the desired scaling.
       But say when its 60 + x, where 0 < x < 60, then we do want to scale up.
       This time the second part of the addition equates to 1. So the scale
       will be 1 + 1 which is 2, the desired scale. */
    scale = bucket->max_bucket / GRAPHCOLS +
        (bucket->max_bucket % GRAPHCOLS ? 1 : 0);

    printf("graph of %s, scaled by a factor of %d\n", header, scale);
    for (i = GRAPHROWS - 1; i >= 0; --i) {
        printf("%6.2f--%6.2f [%3d]:", bucket->start + i * bucket->step,
            bucket->start + (i + 1) * bucket->step, bucket->buckets[i]);

        for (j = 0; j < (int) ceil((double) bucket->buckets[i] / scale); ++j) {
            printf("*");
        }
        printf("\n");
    }
}

void init_category_t(csv_t *csv, category_t *categories,
        int category_column) {
    int category_index = category_column - 1;
    int i;
    categories->number_of_categories = 0;

    /* inserts all category into array */
    for (i = 0; i < csv->nrows; ++i) {
        insert_category_category_t(categories, csv->vals[i][category_index]);
    }

    qsort(categories->categories, categories->number_of_categories,
        sizeof(double), double_compare);

    /* initialize count of each category */
    for (i = 0; i < categories->number_of_categories; ++i) {
        categories->count[i] = 0;
        categories->sum[i] = 0.0;
    }
}

int double_compare(const void *a, const void *b) {
    double answer = (*(double *)a - *(double *)b);
    if (answer < -EPSILON) {
        return -1;
    } else if (answer > EPSILON) {
        return 1;
    }
    return 0;
}

void insert_category_category_t(category_t *categories, double category) {
    int category_index = category_index_category_t(categories, category);

    if (category_index == -1) {
        assert(categories->number_of_categories <= MAXCATS);
        categories->categories[categories->number_of_categories++] = category;
    }
}

int category_index_category_t(category_t *categories, double category) {
    int i;

    for (i = 0; i < categories->number_of_categories; ++i) {
        if (-EPSILON <= categories->categories[i] - category &&
                categories->categories[i] - category <= EPSILON) {
            return i;
        }
    }

    return -1;
}

void sum_categories_category_t(csv_t *csv, category_t *categories,
        int category_column, int value_column) {
    int category_index = category_column - 1;
    int value_index = value_column - 1;
    int stored_category_index;
    int i;

    for (i = 0; i < csv->nrows; ++i) {
        /* Speed can be increased by using bsearch */
        stored_category_index =
            category_index_category_t(categories, csv->vals[i][category_index]);

        categories->count[stored_category_index]++;
        categories->sum[stored_category_index] += csv->vals[i][value_index];
    }
}

void print_category_average(csv_t *csv, category_t *categories,
        int category_column, int value_column) {
    int i;
    string category_label = csv->labs[category_column - 1];
    string value_label = csv->labs[value_column - 1];

    printf("%10s %9s %s\n", category_label, "average", value_label);

    for (i = 0; i < categories->number_of_categories; ++i) {
        printf("%10.2f %10.2f (over %d values)\n",
            categories->categories[i],
            categories->sum[i] / categories->count[i], categories->count[i]);
    }
}

double calculate_taua_correlation(csv_t *csv,
        int column_number_1, int column_number_2) {
    int column_index_1 = column_number_1 - 1;
    int column_index_2 = column_number_2 - 1;
    int i, j;
    int concordant_count = 0;
    int discordant_count = 0;
    double diff1, diff2;

    for (i = 0; i < csv->nrows; ++i) {
        for (j = i + 1; j < csv->nrows; ++j) {
            diff1 = csv->vals[i][column_index_1] - csv->vals[j][column_index_1];
            diff2 = csv->vals[i][column_index_2] - csv->vals[j][column_index_2];

            if ((diff1 < -EPSILON && diff2 < -EPSILON) ||
                    (diff1 > EPSILON && diff2 > EPSILON)) {
                ++concordant_count;
            } else if ((diff1 > EPSILON && diff2 < -EPSILON) ||
                    (diff1 < -EPSILON && diff2 > EPSILON)) {
                ++discordant_count;
            }
        }
    }

    return 2.0 * (concordant_count - discordant_count)
        / (csv->nrows * (csv->nrows - 1.0));
}

void print_taua_correlation(csv_t *csv, int column_number_1,
        int column_number_2, double taua_correlation) {
    string label_1 = csv->labs[column_number_1 - 1];
    string label_2 = csv->labs[column_number_2 - 1];

    printf("tau coefficient between %s and %s = %3.2f\n",
        label_1, label_2, taua_correlation);
}

void init_bucket_2d_t(csv_t *csv, bucket_2d_t *bucket,
        int column_number_1, int column_number_2) {
    int i, j;
    double min = get_by_func(csv, column_number_1, double_min);
    double max = get_by_func(csv, column_number_1, double_max);
    double step = (max + EPSILON - (min - EPSILON)) / GRAPHROWS;
    bucket->row_start = min - EPSILON;
    bucket->row_step = step;

    min = get_by_func(csv, column_number_2, double_min);
    max = get_by_func(csv, column_number_2, double_max);
    step = (max + EPSILON - (min - EPSILON)) / GRAPHCOLS;
    bucket->column_start = min - EPSILON;
    bucket->column_step = step;

    for (i = 0; i < GRAPHROWS; ++i) {
        for (j = 0; j < GRAPHCOLS; ++j) {
            bucket->buckets[i][j] = 0;
        }
    }
}

void populate_bucket_2d(csv_t *csv, bucket_2d_t *bucket,
        int column_number_1, int column_number_2) {
    int i;
    int row_bucket_index, column_bucket_index;
    int column_index_1 = column_number_1 - 1;
    int column_index_2 = column_number_2 - 1;

    for (i = 0; i < csv->nrows; ++i) {
        row_bucket_index = calculate_bucket_index(csv->vals[i][column_index_1],
            bucket->row_start, bucket->row_step);
        column_bucket_index = calculate_bucket_index(
            csv->vals[i][column_index_2],
            bucket->column_start,
            bucket->column_step
        );

        ++(bucket->buckets[row_bucket_index][column_bucket_index]);
    }
}

void print_bucket_2d_graph(csv_t *csv, bucket_2d_t *bucket,
        int column_number_1, int column_number_2) {
    int i, j;
    string vertical_label = csv->labs[column_number_1 - 1];
    string horizontal_label = csv->labs[column_number_2 - 1];

    printf("plot of %s (vertical) and %s (horizontal)\n",
        vertical_label, horizontal_label);

    for (i = GRAPHROWS - 1; i >= 0; --i) {
        printf("%6.2f--%6.2f: ", bucket->row_start + i * bucket->row_step,
            bucket->row_start + (i + 1) * bucket->row_step);
        for (j = 0; j < GRAPHCOLS; ++j) {
            printf("%c", decorate_value(bucket->buckets[i][j]));
        }
        printf("\n");
    }
}

char decorate_value(int value) {
    int scaled;
    if (!value) {
        return EMPTY_CHAR;
    }

    scaled = (int) (log(value + 1) / log(2));
    if (scaled < SCALE_START_2) {
        return scaled + SCALE_START_CHAR;
    }
    return scaled - SCALE_START_2 + SCALE_START_CHAR_2;
}
