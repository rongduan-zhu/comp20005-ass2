#ifndef __HELPER_H__
#define __HELPER_H__

#define MAXROWS 1000    /* altered from 10000 by Alistair, 15-05-07 */
#define MAXCOLS 50

#define LINELEN 500 /* maximum length of any input line */

#define ERROR   (-1)    /* error return value from some functions */
#define COMMA   ',' /* separator for CSV files */

#define INDEXX  'i' /* command to list the column headings */
#define DATDMP  'd' /* command to dump out the data */
#define AVERGE  'a' /* command to average a column */
#define CATAVG  'c' /* command to category-average a column */
#define GRAPH1  'g' /* command to graph a column */
#define GRAPH2  'p' /* command to plot a 2d correlation */
#define KNDALL  'k' /* command to compute Kendall's tau */
#define ALLOPS  "acdgikp"
            /* list of all valid commands */

#define ARGS_0  "di"    /* commands that take no arguments */
#define ARGS_1  "ag"    /* commands that take one argument */
#define ARGS_2  "ckp"   /* commands that take two arguments */
#define MAXARGS  2      /* maximum number of arguments to any command */

#define GRAPHROWS 20    /* number of rows in graph */
#define GRAPHCOLS 60    /* number of columns in graph */
#define EPSILON   1e-6  /* small adjustment for graph computation */

#define MAXCATS 1000    /* maximum number of categories to be handled */

#define FILINP  1   /* indicates command input coming from a file */
#define PROMPT  "> "

#define EMPTY_CHAR '.'
#define SCALE_START_CHAR '0'
#define SCALE_START_CHAR_2 'a'
/* first point to start using a */
#define SCALE_START_2 (2 << 9)

typedef char *string;

typedef char input_line_t[LINELEN+1];

typedef struct {
    input_line_t labelstring;
    char *labs[MAXCOLS+1];
    int nrows;
    int ncols;
    double vals[MAXROWS][MAXCOLS];
} csv_t;

typedef struct {
    char command;
    int nargs;
    int argvals[MAXARGS];
} command_t;

typedef struct {
    int buckets[GRAPHROWS];
    int max_bucket;
    double start;
    double step;
} bucket_t;

typedef struct {
    int number_of_categories;
    double categories[MAXCATS];
    int count[MAXCATS];
    double sum[MAXCATS];
} category_t;

typedef struct {
    int buckets[GRAPHROWS][GRAPHCOLS];
    double row_start;
    double row_step;
    double column_start;
    double column_step;
} bucket_2d_t;

double _get_by_func(csv_t *csv, int column_number, double (*get)(double , double));
double double_max(double a, double b);
double double_min(double a, double b);
int int_max(int a, int b);
double _average_csv_t(csv_t *csv, int column_number);
void _print_average_csv_t(csv_t *csv, int column_number, double average);
void _init_bucket_t(csv_t *csv, bucket_t *bucket, int column_number);
int _calculate_bucket_index(double value, double min, double step);
void _populate_bucket(csv_t *csv, bucket_t *bucket, int column_number);
void _print_bucket_graph(csv_t *csv, bucket_t *bucket, int column_number);
void _init_category_t(csv_t *csv, category_t *categories, int category_column);
int double_compare(const void *a, const void *b);
void _insert_category_category_t(category_t *categories, double category);
int _category_index_category_t(category_t *categories, double category);
void _sum_categories_category_t(csv_t *csv, category_t *categories,
        int category_column, int value_column);
void _print_category_average(csv_t *csv, category_t *categories,
        int category_column, int value_column);
double _calculate_taua_correlation(csv_t *csv,
        int column_number_1, int column_number_2);
void _print_taua_correlation(csv_t *csv, int column_number_1,
        int column_number_2, double taua_correlation);
void _init_bucket_2d_t(csv_t *csv, bucket_2d_t *bucket,
        int column_number_1, int column_number_2);
void _populate_bucket_2d(csv_t *csv, bucket_2d_t *bucket,
        int column_number_1, int column_number_2);
void _print_bucket_2d_graph(csv_t *csv, bucket_2d_t *bucket,
        int column_number_1, int column_number_2);
char _decorate_value(int value);

#endif
