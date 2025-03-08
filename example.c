#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <c_csv.h>

int main(int argc, char * argv[]) {
    C_CSV obj = C_CSV_Create();

    if (NULL != obj) {
        if (0 == C_CSV_SetSeparator(obj, ';')) {
            if (0 == C_CSV_ReadFile(obj, "file.csv")) {
                size_t rows = 0;
                if (0 == C_CSV_GetNumberOfRows(obj, &rows)) {
                    size_t i;
                    for (i = 0; i < rows; i++) {
                        char value[32] = "";
                        if (0 == C_CSV_GetValue(obj, i, 0, value))
                            printf("Row = %5lld, %10s = %d: %-10s | ", i, "columns", 0, value);

                        if (0 == C_CSV_GetValue(obj, i, 0, value))
                            printf("%10s = %5lld, %10s = %d: %-10s | ", "Rows", i, "columns", 3, value);

                        if (0 == C_CSV_GetLastSavedValue(obj, value))
                            printf("%s %s\n", "Last saved value:", value);
                    }
                }
            }

            size_t dataSize = 0;
            int i;
            for (i = 0; i < 6; i++) {
                if (0 == C_CSV_GetDataSize(obj, &dataSize)) {
                    if (0 == dataSize) {
                        /* You can add a row of data. The number of strings in the array
                            must be equal to the number of columns in the data already
                            loaded into the object. If the data has not been uploaded
                            yet (empty), you can add a different number of strings. A
                            separator will be automatically placed between the strings,
                            configured via C_CSV_SetSeparator. */
                        char * data[] = { "col_1", "col_2", "col_3", "col_4", "col_5", "col_6" };
                        if (0 == C_CSV_AddRow(obj, data))
                            if (C_CSV_WriteFile(obj, "file.csv"))
                                printf("Error writing to a file (line %d)\n", i);
                    } else {
                        char * data[] = { "Hello", "Hi", "World", "123", "456", "789" };
                        if (0 == C_CSV_AddRow(obj, data))
                            if (C_CSV_WriteFile(obj, "file.csv"))
                                printf("Error writing to a file (line %d)\n", i);
                    }
                }
            }
        }

        C_CSV_Delete(&obj);
    }


    printf("\nDone\n");

    return 0;
}