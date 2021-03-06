/* Skeleton "interpreter" for numeric processing on CSV files

   Written by Alistair Moffat, May 2015

   Modifications by Rongduan Zhu (rz), May 2015
   Github: github.com/rongduan-zhu

   Purpose: The program reads and processes a csv file. It supports commands to
   list the column headings, dump the data, calculates the average of a column,
   graph a column, plot a 2d correlation graph, and computes the Kendall's tau
   correlation.

   programming is fun
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#include "helper.h"

/* function prototypes */

void    read_csv_file(char *fname, csv_t *D);
void    reassign_input(char *fname);
void    print_prompt();
int     read_command(command_t *comd, int fileinput, int ncols);
void    process_line(command_t *comd, csv_t *D);
void    do_datdmp(csv_t *D);
void    do_indexx(csv_t *D);
void    do_averge(csv_t *D, int col);
void    do_graph1(csv_t *D, int col);
void    do_catavg(csv_t *D, int cat, int col);
void    do_kndall(csv_t *D, int col1, int col2);
void    do_graph2(csv_t *D, int col1, int col2);

/****************************************************************/

/* main program controls all the action
 */
int
main(int argc, char *argv[]) {
    int fileinput=0;
    command_t comd;
    csv_t D;

    /* first argument on commandline is the data file name */
    read_csv_file(argv[1], &D);

    /* second argument, if it exists, is file of input commands */
    if (argc==3) {
        fileinput = 1;
        reassign_input(argv[2]);
    }

    /* start the main execution loop */
    print_prompt();
    while (read_command(&comd, fileinput, D.ncols)) {
        process_line(&comd, &D);
        /* then round we go */
        print_prompt();
    }

    /* all done, so pack up and go home */
    printf("bye\n");
    return 0;
}

/****************************************************************/

/* reads a csv file in to the defined structure, with empty or non-numeric
   values replaced by 0.0/0.0 = nan so as to prevent downstream arithmetic
   being interpreted incorrectly. Probably best to just leave this function
   alone, you don't need to edit it to complete the project
*/

void
read_csv_file(char *fname, csv_t *D) {
    FILE *fp;   /* used to read from a named file */
    input_line_t line;
    int cols=0, rows=0, bytes=0;
    int c, i, j, chr, ncommas, empties=0;
    double x;
    double nan = 0.0/0.0;

    /* first argument on commandline should the data file name */
    if (fname==NULL) {
        /* and it wasn't there... */
        printf("No csv file specified on commandline\n");
        exit(EXIT_FAILURE);
    }

    /* try and open the named file for reading */
    if ((fp=fopen(fname,"r")) == NULL) {
        printf("Error: unable to open %s\n", fname);
        exit(EXIT_FAILURE);
    }

    /* file is open, can now use fp to access CSV data,
       start by reading the bytes of the header row */
    while ((c=getc(fp)) != '\n') {
        D->labelstring[bytes++] = c;
    }
    D->labelstring[bytes] = '\0';

    /* now process line again, breaking in to separate labels by
       replacing commas by nulls, and tracking the start of each of
       the column headings */
    D->labs[cols++] = D->labelstring;
    for (i=1; i<bytes; i++) {
        if (D->labelstring[i]==COMMA) {
            D->labelstring[i] = '\0';
            D->labs[cols++] = D->labelstring+i+1;
        }
        if (cols>MAXCOLS && i<bytes) {
            printf("Too many columns, limit is %d\n",
                MAXCOLS);
            exit(EXIT_FAILURE);
        }
    }
    D->labs[cols] = NULL;

    /* ok, that's the labels sorted, now for the data */
    while ((chr=getc(fp)) != EOF) {

        /* there is another row, because first character of it
           just got read, next step is to get the rest of them */
        i = 0;
        line[i++] = chr;
        ncommas = (chr==COMMA) ;
        while (((chr=getc(fp))!=EOF) && (chr!='\n' && chr!='\r')) {
            line[i++] = chr;
            ncommas += (chr==COMMA) ;
        }
        line[i] = '\0';
        if (ncommas!=cols-1) {
            printf("Data input error line %d\n", rows+2);
            exit(EXIT_FAILURE);
        }
        /* then process the line from the right end */
        j = i-1;
        for (c=cols-1; c>=0; c--) {
            /* look for next previous comma */
            while (j>=0 && line[j]!=COMMA) {
                j--;
            }
            /* access the value */
            if (sscanf(line+j+1, "%lf", &x) == 1) {
                D->vals[rows][c] = x;
            } else {
                D->vals[rows][c] = nan;
                empties++;
            }
            /* mark the new end of the string */
            line[j--] = '\0';
        }
        rows++;
        /* check to make sure don't overflow array */
        if (rows==MAXROWS) {
            /* time to stop reading data */
            printf("Too many rows, truncated at %d\n", MAXROWS);
            break;
        }
        /* if not full, go round and see if there is another data row */
    }

    /* either input has all been read or array is full */
    printf("file %s:\n    %d columns and %d rows of data\n",
            fname, cols, rows);
    if (empties) {
        printf("    %d entries were empty or non-numeric\n",
            empties);
    }
    /* finish building the structure */
    D->nrows = rows;
    D->ncols = cols;
    return;
}

/****************************************************************/

/* if there is a valid filename on the commandline, redirect stdin
   so that the file is read, and return FILINP to show that input
   input lines should be echoed to the output when they are read
*/
void
reassign_input(char *fname) {
    if (freopen(fname, "r", stdin)==NULL) {
        printf("Unable to open \"%s\"\n", fname);
        exit(EXIT_FAILURE);
    }
    /* stdin successfully reopened to the named file */
    printf("Input file: %s\n", fname);
    return;
}

/****************************************************************/

/* print the "ready for input" prompt
 */
void
print_prompt() {
    printf(PROMPT);
    return;
}

/****************************************************************/

/* read a line of input into the array passed as argument
   returns false if there is no input available
   all whitespace characters are removed
*/
int
read_command(command_t *comd, int fileinput, int ncols) {
    int i=0, c;
    int col;
    input_line_t line;
    /* get a whole input line, single blank of multiples */
    while (((c=getchar())!=EOF) && (c!='\n')) {
        if (i<LINELEN) {
            line[i] = c;
            if (i==0 || (isspace(line[i-1])*isspace(line[i])==0)) {
                i++;
            }
        }
    }
    line[i] = '\0';
    if (fileinput) {
        /* print out the input command */
        printf("%s\n", line);
    }
    /* nothing typed? straight back to caller */
    if (i==0 && c==EOF) {
        return 0;
    }
    if (i==0) {
        return 1;
    }
    /* something typed? parse into parts needed */
    comd->command = line[0];
    comd->nargs = 0;
    for (i=1; line[i]; i++) {
        if (!isdigit(line[i]) && !isspace(line[i])) {
            printf("Invalid input character\n");
            return 1;
        }
        if (line[i-1]==' ' && line[i]!=' ') {
            col = atoi(line+i);
            comd->argvals[comd->nargs++] = col;
        }
    }
    return ((i>0) || (c!=EOF));
}

/****************************************************************/

/* process a command by parsing the input line into parts and
   carrying out the specified action
 */
void
process_line(command_t *comd, csv_t *D) {
    int optype, col1=0, col2=0;

    /* determine the operation to be performed, it
       must be first character in line
     */
    optype = comd->command;
    if (strchr(ALLOPS, optype) == NULL) {
        printf("Unknown operator\n");
        return;
    }

    /* determine the string argument (if one is required),
       it must start in second character of line
     */
    if (strchr(ARGS_0, optype)) {
        if (comd->nargs!=0) {
            printf("No argument required for '%c'\n", optype);
            return;
        }
    } else if (strchr(ARGS_1, optype)) {
        if (comd->nargs!=1) {
            printf("One argument required for '%c'\n", optype);
            return;
        }
        col1 = comd->argvals[0];
        if (col1>D->ncols) {
            printf("Invalid column number, ");
            printf("max is %d\n", D->ncols);
            return;
        }
    } else if (strchr(ARGS_2, optype)) {
        if (comd->nargs!=2) {
            printf("Two arguments required for '%c'\n", optype);
            return;
        }
        col1 = comd->argvals[0];
        col2 = comd->argvals[1];
        if (col1>D->ncols || col2>D->ncols) {
            printf("Invalid column number, ");
            printf("max is %d\n", D->ncols);
            return;
        }
    }

    /* finally, do the actual operation
     */
    if (optype == INDEXX) {
        do_indexx(D);
    } else if (optype == DATDMP) {
        do_datdmp(D);
    } else if (optype == AVERGE) {
        do_averge(D, col1);
    } else if (optype == GRAPH1) {
        do_graph1(D, col1);
    } else if (optype == CATAVG) {
        do_catavg(D, col1, col2);
    } else if (optype == KNDALL) {
        do_kndall(D, col1, col2);
    } else if (optype == GRAPH2) {
        do_graph2(D, col1, col2);
    }
    return;
}

/****************************************************************/

/* provide an index list of the column headings
*/
void
do_indexx(csv_t *D) {
    int c;
    printf("      col  data\n");
    for (c=0; c<D->ncols; c++) {
        printf("      %3d  %s\n", c+1, D->labs[c]);
    }
    return;
}

/****************************************************************/

/* dump out the data in the CSV structure D
*/
void
do_datdmp(csv_t *D) {
    int r, c;
    /* first the header labels */
    printf("      ");
    for (c=0; c<D->ncols; c++) {
        printf("%10s ", D->labs[c]);
    }
    printf("\n");
    /* now the values in the data rows */
    for (r=0; r<D->nrows; r++) {
        printf("%4d: ", r+1);
        for (c=0; c<D->ncols; c++) {
            printf("%10.2f ", D->vals[r][c]);
        }
        printf("\n");
    }
    return;
}

/****************************************************************/

/* implement the 'a' averaging command
*/
void
do_averge(csv_t *D, int col) {
    assert(col <= D->ncols && col > 0);

    double average = average_csv_t(D, col);
    print_average_csv_t(D, col, average);
}

/****************************************************************/

/* implement the 'g' graphing command
*/
void
do_graph1(csv_t *D, int col) {
    bucket_t bucket;
    init_bucket_t(D, &bucket, col);
    populate_bucket(D, &bucket, col);
    print_bucket_graph(D, &bucket, col);
}

/****************************************************************/

/* implement the 'c' category average command
*/
void
do_catavg(csv_t *D, int cat, int col) {
    category_t categories;
    init_category_t(D, &categories, cat);
    sum_categories_category_t(D, &categories, cat, col);
    print_category_average(D, &categories, cat, col);
}

/****************************************************************/

/* implement the 'k' command to compute Kendall's tau-b
   coefficient for similarity between two sets of paired values
*/
void
do_kndall(csv_t *D, int col1, int col2) {
    double taua_coefficient = calculate_taua_correlation(D, col1, col2);
    print_taua_correlation(D, col1, col2, taua_coefficient);
}

/****************************************************************/

/* implement the 'p' plot command to generate
   a 2d graph showing correlation between two columns
*/
void
do_graph2(csv_t *D, int col1, int col2) {
    bucket_2d_t bucket;
    init_bucket_2d_t(D, &bucket, col1, col2);
    populate_bucket_2d(D, &bucket, col1, col2);
    print_bucket_2d_graph(D, &bucket, col1, col2);
}
