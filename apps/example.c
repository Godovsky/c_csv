#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <c_csv.h>

int main(int argc, char *argv[]) {
    C_CSV obj = C_CSV_Create();

    if (NULL != obj) {
        if (0 == C_CSV_SetSeparator(obj, ';')) {
            char value[32] = "";
            size_t dataSize = 0;
            size_t tmpRow, tmpCol;
            size_t lastRow, lastCol;
            size_t numOfRows = 0;
            size_t i;

            if (0 == C_CSV_ReadFile(obj, "file.csv")) {
                if (0 == C_CSV_GetNumberOfRows(obj, &numOfRows)) {
                    for (i = 0; i < numOfRows; i++) {
                        if (0 == C_CSV_GetValue(obj, i, 0, value))
                            printf("Row = %5lu, %10s = %5d: %-10s | ", (unsigned long)i, "columns", 0, value);

                        if (0 == C_CSV_GetValue(obj, i, 0, value))
                            printf("%10s = %5lu, %10s = %5d: %-10s | ", "Rows", (unsigned long)i, "columns", 3, value);

                        if (0 == C_CSV_GetCurrentValue(obj, value))
                            printf("%s %s\n", "Last saved value:", value);
                    }
                }
            }

            if (0 == C_CSV_GetNumberOfRows(obj, &lastRow))
                if (0 == C_CSV_GetNumberOfColumns(obj, &lastCol))
                    printf("\nLast row = %lu, last column = %lu\n", (unsigned long)lastRow, (unsigned long)lastCol);

            for (i = 0; i < 6; i++) {
                if (0 == C_CSV_GetDataSize(obj, &dataSize)) {
                    if (0 == dataSize) {
                        /* You can add a row of data. The number of strings in the array
                            must be equal to the number of columns in the data already
                            loaded into the object. If the data has not been uploaded
                            yet (empty), you can add a different number of strings. A
                            separator will be automatically placed between the strings,
                            configured via C_CSV_SetSeparator. */
                        char *data[] = { "col_1", "col_2", "col_3", "col_4", "col_5", "col_6" };
                        if (0 == C_CSV_AddRow(obj, data))
                            if (C_CSV_WriteFile(obj, "file.csv"))
                                printf("Error writing to a file (line %lu)\n", (unsigned long)i);
                    } else {
                        char *data[] = { "Hello", "Hi", "World", "123", "456", "789" };
                        if (0 == C_CSV_AddRow(obj, data))
                            if (C_CSV_WriteFile(obj, "file.csv"))
                                printf("Error writing to a file (line %lu)\n", (unsigned long)i);
                    }
                }
            }

            if (0 == C_CSV_SetPosition(obj, 0, 0)) {
                if (0 == C_CSV_GetPosition(obj, &tmpRow, &tmpCol))
                    if (0 == C_CSV_GetCurrentValue(obj, value))
                        printf("\nRow = %lu, col = %lu, value = %s\n", (unsigned long)tmpRow, (unsigned long)tmpCol, value);
            }
            if (0 == C_CSV_SetPosition(obj, 2, 2)) {
                if (0 == C_CSV_GetPosition(obj, &tmpRow, &tmpCol))
                    if (0 == C_CSV_GetCurrentValue(obj, value))
                        printf("Row = %lu, col = %lu, value = %s\n", (unsigned long)tmpRow, (unsigned long)tmpCol, value);
            }
            if (0 == C_CSV_SetPosition(obj, 1, 1)) {
                if (0 == C_CSV_GetPosition(obj, &tmpRow, &tmpCol))
                    if (0 == C_CSV_GetCurrentValue(obj, value))
                        printf("Row = %lu, col = %lu, value = %s\n", (unsigned long)tmpRow, (unsigned long)tmpCol, value);
            }

            if (0 == C_CSV_GetNumberOfRows(obj, &lastRow))
                if (0 == C_CSV_GetNumberOfColumns(obj, &lastCol))
                    printf("\nLast row = %lu, last column = %lu\n", (unsigned long)lastRow, (unsigned long)lastCol);
        }

        C_CSV_Delete(&obj);
    }


    printf("\nDone\n");

    return 0;
}