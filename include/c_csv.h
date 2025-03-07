/* Copyright (c) 2025 Godov Andrey <andygodov@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#ifndef C_CSV_H
#define C_CSV_H

#define SAVED_VALUE_LENGTH 32

/** A pointer to the object that is used to work with the csv format. */
typedef void * C_CSV;

/** Structure for saving the value found in the CSV file. */
typedef struct {
    char lastSavedValue[SAVED_VALUE_LENGTH];
} LAST_VALUE;

/** The function allocates memory for the C_CSV (void *) object and returs it.
 * All object parameters will be initialized with zeros.
 * In case of an error, NULL is returned. */
C_CSV C_CSV_Create();

/** The function deletes a previously created C_CSV object and frees up memory.
 * After execution, the object will be assigned NULL.
 * Returns 0 in case of success, returns 1 in case of error. */
int C_CSV_Delete(C_CSV * obj);

/** Setting the data separator.
 * In csv files, data is grouped by rows with values in columns separated by a separating symbol.
 * To work with data, you need to specify a separator.
 * Available separators: ";", " ", ":", ",", "\t".
 * Returns 0 in case of success, returns 1 in case of error. */
int C_CSV_SetSeparator(C_CSV obj, char separator);

/** The function of getting the size in bytes of all the data stored in the object.
 * The value will be assigned to the "size" argument, which must be passed by pointer.
 * Returns 0 in case of success, returns 1 in case of error. */
int C_CSV_GetDataSize(C_CSV obj, size_t * size);

/** The function of getting the number of rows in the data stored in the object.
 * The value will be assigned to the "numberOfRows" argument, which must be passed by pointer.
 * Returns 0 in case of success, returns 1 in case of error. */
int C_CSV_GetNumberOfRows(C_CSV obj, size_t * numberOfRows);

/** The function of getting the number of columns in the data stored in the object.
 * The value will be assigned to the "numberOfColumns" argument, which must be passed by pointer.
 * Returns 0 in case of success, returns 1 in case of error. */
int C_CSV_GetNumberOfColumns(C_CSV obj, size_t * numberOfColumns);

/** The function of reading a file and saving all its data to a object.
 * It is not recommended to use the same file (file name) for reading and then for writing.
 * This is because errors in opening and reading the file can lead to overwriting and data loss.
 * Returns 0 in case of success, returns 1 in case of error. */
int C_CSV_ReadFile(C_CSV obj, char fileName[]);

/** The function of writing all the data stored in the object to a file.
 * It is not recommended to use the same file (file name) for reading and then for writing.
 * This is because errors in opening and reading the file can lead to overwriting and data loss.
 * Returns 0 in case of success, returns 1 in case of error. */
int C_CSV_WriteFile(C_CSV obj, char fileName[]);

/** A function to get the value stored in row "row" and column "col".
 * Returns a pointer to a string with a value or NULL in case of an error. */
int C_CSV_GetValue(C_CSV obj, size_t row, size_t col, LAST_VALUE * LastValue);

/** The function of getting the last value that was read from the file through the C_CSV_GetValue function.
 * Returns a pointer to a string with a value or NULL in case of an error. */
int C_CSV_GetLastSavedValue(C_CSV obj, LAST_VALUE * LastValue);

/** The function of adding a string to the end of the data stored in the object.
 * Use the macro C_CSV_AddRow(obj, row) to avoid errors related to the length of the row.
 * The row argument is an array of pointers to strings and looks like this: char * row[].
 * Returns 0 in case of success, returns 1 in case of error. */
#define C_CSV_AddRow(obj, row) c_csv_add_row((obj), (row), sizeof((row)) / sizeof((row)[0]))
int c_csv_add_row(C_CSV obj, char * row[], size_t size);

#endif /* C_CSV_H */