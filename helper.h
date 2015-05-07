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

double _average_csv_t(csv_t *csv, int column_number);
void _print_average_csv_t(csv_t *csv, int column_number, double average);

#endif