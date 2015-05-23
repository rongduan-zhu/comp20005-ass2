/*
    Author: Rongduan Zhu (584 169)
*/

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

#define EMPTY_CHAR '.'          /* char to represent a value of 0 */
#define SCALE_START_CHAR '0'    /* base char to represent value
                                   greater than 1 */
#define SCALE_START_CHAR_2 'a'  /* base char to represent value
                                   greater than 1023 */
#define SCALE_START_2 10        /* first point to start using a */

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

/* structure to represent a single dimension bucket */
typedef struct {
    int buckets[GRAPHROWS];
    int max_bucket;
    double start;
    double step;
} bucket_t;

/* structure to represent category averages */
typedef struct {
    int number_of_categories;
    double categories[MAXCATS];
    int count[MAXCATS];
    double sum[MAXCATS];
} category_t;

/* structure to represent a two dimensional bucket */
typedef struct {
    int buckets[GRAPHROWS][GRAPHCOLS];
    double row_start;
    double row_step;
    double column_start;
    double column_step;
} bucket_2d_t;

/* Function that iterates over a column in a csv file and get a value based
   based on a get function

   @param csv               a csv struct
   @param column_number     the column number to extract value from (1 indexed)
   @param get               the get function, which takes the current selected
                            value and the next value in the column, and returns
                            one of these two values based on internal logic

   @return                  a value in the column based on the logic in the
                            get function
*/
double get_by_func(csv_t *csv, int column_number,
    double (*get)(double , double));

/* Finds the bigger value between a and b

   @param a     a double value, a
   @param b     a double value, b

   @return      the value of the bigger number (a or b)
*/
double double_max(double a, double b);

/* Finds the smaller value between a and b

   @param a     a double value, a
   @param b     a double value, b

   @return      the value of the smaller number (a or b)
*/
double double_min(double a, double b);

/* Finds the bigger value between a and b

   @param a     a int value, a
   @param b     a int value, b

   @return      the value of the bigger number (a or b)
*/
int int_max(int a, int b);

/* Calculates the average of a column in a csv

   @param csv               a csv struct
   @param column_number     the column number (1 indexed)

   @return                  the average of the column
*/
double average_csv_t(csv_t *csv, int column_number);

/* Prints the average of a column

   @param csv               a csv struct
   @param column_number     the column that the average is for (1 indexed)
   @param average           the average of the column
*/
void print_average_csv_t(csv_t *csv, int column_number, double average);

/* Initializes a bucket struct for a column

   @param csv               a csv struct
   @param bucket            a bucket to be initialized (assumes not null)
   @param column_number     the column number to initialize the bucket for
                            (1 indexed)
*/
void init_bucket_t(csv_t *csv, bucket_t *bucket, int column_number);

/* Find the index of the bucket which the value should be in

   @param value     the value to find the bucket index for
   @param min       the lowerbound of the first bucket
   @param step      the range of each bucket

   @return          the index of the bucket the value should be in
*/
int calculate_bucket_index(double value, double min, double step);

/* Populates the buckets based using the values in specified column

   @param csv               a csv struct
   @param bucket            a initialized bucket ready to be populated
   @param column_number     the column number to populate the bucket
*/
void populate_bucket(csv_t *csv, bucket_t *bucket, int column_number);

/* Print a graph using the value in the bucket

   @param csv               a csv struct
   @param bucket            a populated bucket to print the graph for
   @param column_number     the column number that the graph is for
*/
void print_bucket_graph(csv_t *csv, bucket_t *bucket, int column_number);

/* Initializes a category struct for a column

   @param csv               a csv struct
   @param categories        a category struct to be initialized for
   @param category_column   the column that will be used to initialize the
                            categories
*/
void init_category_t(csv_t *csv, category_t *categories, int category_column);

/* Compare two doubles and returns the order

   @param a     a double (casted) value, a
   @param b     a double (casted) value, b

   @return      lt 0 if a < b, 0 if a == b, gt 0 if a > b
*/
int double_compare(const void *a, const void *b);

/* Inserts the category value into the category struct. If the category already
   exists, do nothing

   @param categories    a category struct to insert the category into
   @param category      the category value to be inserted in the category struct
*/
void insert_category_category_t(category_t *categories, double category);

/* Finds the index of the category in the category struct, if it doesn't exist
   the function returns -1

   @param categories    a category struct
   @param category      the category to find the index for
   @return              the index of the category in the category struct, -1 is
                        returned if the category does not exist
*/
int category_index_category_t(category_t *categories, double category);

/* Calculates the sum of each individual category

   @param csv               a csv struct
   @param categories        a populated category struct
   @param category_column   the column to use for categories
   @param value_column      the column to use for values
*/
void sum_categories_category_t(csv_t *csv, category_t *categories,
        int category_column, int value_column);

/* Prints the sum of each individual category

   @param csv               a csv struct
   @param categories        a populated category struct
   @param category_column   the column to use for categories
   @param value_column      the column to use for values
*/
void print_category_average(csv_t *csv, category_t *categories,
        int category_column, int value_column);

/* Calculates the tau-a correlation between two columns

   @param csv               a csv struct
   @param column_number_1   a column to use for tau-a correlation
   @param column_number_2   another column to use for tau-a correlation

   @return                  the tau a correlation between the two columns
*/
double calculate_taua_correlation(csv_t *csv,
        int column_number_1, int column_number_2);

/* Prints the tau-a correlation between two columns

   @param csv               a csv struct
   @param column_number_1   a column to use for tau-a correlation
   @param column_number_2   another column to use for tau-a correlation
   @param taua_correlation  the tau-a correlation between the two columns
*/
void print_taua_correlation(csv_t *csv, int column_number_1,
        int column_number_2, double taua_correlation);

/* Initialize the two dimensional bucket based on the two columns from the csv

   @param csv               a csv struct
   @param bucket            a bucket struct to be initialized (assumes not null)
   @param column_number_1   a column to use for one dimension of the
                            bucket (row)
   @param column_number_2   a column to use for another dimension of
                            the bucket (colulmn)
*/
void init_bucket_2d_t(csv_t *csv, bucket_2d_t *bucket,
        int column_number_1, int column_number_2);

/* Populate the two dimensional bucket using the two columns in the csv

   @param csv               a csv struct
   @param bucket            a bucket struct to be populated
   @param column_number_1   a column to use for one dimension of the
                            bucket (row)
   @param column_number_2   a column to use for another dimension of
                            the bucket (colulmn)
*/
void populate_bucket_2d(csv_t *csv, bucket_2d_t *bucket,
        int column_number_1, int column_number_2);

/* Prints a graph of the two dimensional bucket

   @param csv               a csv struct
   @param bucket            a bucket struct to be used to print the graph
   @param column_number_1   a column to use for the y axis (rows)
   @param column_number_2   a column to use for the x axis (columns)
*/
void print_bucket_2d_graph(csv_t *csv, bucket_2d_t *bucket,
        int column_number_1, int column_number_2);

/* Decorates a int value to a char. The formula used is floor(log2(value + 1))

   @param value     the value to be decorated for

   @return          a corresponding char after the value has been "normalized"
*/
char decorate_value(int value);

#endif
