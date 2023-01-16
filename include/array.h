#ifndef _ARRAY_H
#define _ARRAY_H
#define ROWS 4
#define COLS 4

typedef double array_2d[ROWS * COLS];
typedef double array_1d[COLS];

void print_array(array_2d *array);
void print_row(array_1d *array);
array_1d *get_row(int line_index, array_2d *array);
array_1d *get_col(int col_index, array_2d *array);

#endif
