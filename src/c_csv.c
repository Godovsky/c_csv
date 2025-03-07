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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <c_csv.h>

typedef struct private {
    char * data;
    int dataSize;
    char separator;
    size_t numberOfRows;
    size_t numberOfColumns;
    size_t position;
    size_t lastValueLen;
} * PRIVATE;

C_CSV C_CSV_Create() {
    PRIVATE prvt = NULL;
    prvt = (PRIVATE)malloc(sizeof(struct private));
    if (NULL == prvt) {
        printf("C_CSV_Create: failed memory allocation\n");

        return NULL;
    }

    prvt->data = NULL;
    prvt->dataSize = 0;
    prvt->separator = 0;
    prvt->numberOfRows = 0;
    prvt->numberOfColumns = 0;

    return (C_CSV)prvt;
}

int C_CSV_Delete(C_CSV * obj) {
    if (NULL == obj) {
        printf("C_CSV_Delete: the pointer to the object is NULL\n");

        return 1;
    }

    if (NULL == *obj) {
        printf("C_CSV_Delete: the object is NULL\n");

        return 1;
    }

    if (NULL != ((PRIVATE)*obj)->data)
        free(((PRIVATE)*obj)->data);

    free(*obj);
    *obj = NULL;

    return 0;
}

int C_CSV_SetSeparator(C_CSV obj, char separator) {
    if (NULL == obj) {
        printf("C_CSV_Delete: the object is NULL\n");

        return 1;
    }

    if (separator != ';' &&
        separator != ' ' &&
        separator != ':' &&
        separator != ',' &&
        separator != '\t') {
        printf("C_CSV_SetSeparator: \";\", \"space\", \":\", \",\", \"tab\" only\n");

        return 1;
    }

    ((PRIVATE)obj)->separator = separator;

    return 0;
}

int C_CSV_GetDataSize(C_CSV obj, size_t * size) {
    if (NULL == obj) {
        printf("C_CSV_Delete: the object is NULL\n");

        return 1;
    }

    *size = ((PRIVATE)obj)->dataSize;

    return 0;
}

int C_CSV_GetNumberOfRows(C_CSV obj, size_t * numberOfRows) {
    if (NULL == obj) {
        printf("C_CSV_Delete: the object is NULL\n");

        return 1;
    }

    *numberOfRows = ((PRIVATE)obj)->numberOfRows;

    return 0;
}

int C_CSV_GetNumberOfColumns(C_CSV obj, size_t * numberOfColumns) {
    if (NULL == obj) {
        printf("C_CSV_Delete: the object is NULL\n");

        return 1;
    }

    *numberOfColumns = ((PRIVATE)obj)->numberOfColumns;

    return 0;
}

int C_CSV_ReadFile(C_CSV obj, char fileName[]) {
    if (NULL == obj) {
        printf("C_CSV_ReadFile: the object is NULL\n");

        return 1;
    }

    if (NULL == fileName) {
        printf("C_CSV_ReadFile: the name string is NULL\n");

        return 1;
    }

    if ('\0' == fileName[0]) {
        printf("C_CSV_ReadFile: empty file name\n");

        return 1;
    }

    PRIVATE prvt = (PRIVATE)obj;

    if (0 == prvt->separator) {
        printf("C_CSV_ReadFile: separator is not specified\n");

        return 1;
    }

    FILE * fp = NULL;
    fp = fopen(fileName, "rb");
    if (NULL == fp) {
        printf("C_CSV_ReadFile: failed %s opening\n", fileName);

        return 1;
    }

    if (fseek(fp, 0, SEEK_END)) {
        printf("C_CSV_ReadFile: error setting the position in the file\n");
        fclose(fp);

        return 1;
    }

    size_t fileSize = (size_t)ftell(fp);
    if (0 > fileSize) {
        printf("C_CSV_ReadFile: file size measurement error\n");
        fclose(fp);

        return 1;
    }

    if (fseek(fp, 0, SEEK_SET)) {
        printf("C_CSV_ReadFile: error setting the position in the file\n");
        fclose(fp);

        return 1;
    }

    if (fileSize == 0) {
        printf("C_CSV_ReadFile: the file is empty\n");
        fclose(fp);

        return 1;
    }

    prvt->data = (char *)malloc(fileSize);
    if (NULL == prvt->data) {
        printf("C_CSV_ReadFile: failed memory allocation\n");
        fclose(fp);
        prvt->dataSize = 0;

        return 1;
    }
    prvt->dataSize = fileSize;

    size_t n = fread(prvt->data, fileSize, 1, fp);
    if (n != 1) {
        printf("C_CSV_ReadFile: error reading data from a file\n");
        fclose(fp);

        return 1;
    }
    fclose(fp);

    int isFirstNewLine = 0;
    size_t nextNumOfColumns = 0;
    prvt->numberOfRows = 0;
    prvt->numberOfColumns = 0;

    int isSeparator = 0;

    size_t i;
    for (i = 0; i < fileSize; i++) {
        if (prvt->data[i] == prvt->separator || prvt->data[i] == '\n' || i + 1 == fileSize) {
            if (prvt->data[i] == prvt->separator)
                isSeparator = 1;

            nextNumOfColumns++;
            if (isFirstNewLine == 0)
                prvt->numberOfColumns++;

            if (prvt->data[i] == '\n' || i + 1 == fileSize) {
                if (isSeparator == 0) {
                    printf("C_CSV_ReadFile: the separator was not found in the data\n");

                    return 1;
                }

                prvt->numberOfRows++;

                if (nextNumOfColumns != prvt->numberOfColumns) {
                    printf("C_CSV_ReadFile: different number of columns in row %lld\n", prvt->numberOfRows);
                    free((void *)prvt->data);
                    prvt->data = NULL;
                    prvt->dataSize = 0;

                    return 1;
                }

                nextNumOfColumns = 0;

                if (isFirstNewLine == 0)
                    isFirstNewLine = 1;
            }
        }
    }

    if (prvt->data[prvt->dataSize - 1] != '\n') {
        char * pre = prvt->data;
        prvt->dataSize += 1;
        prvt->data = (char *)malloc(prvt->dataSize);
        if (NULL == prvt->data) {
            if (NULL != pre)
                free(pre);
            printf("read_file: failed memory allocation\n");

            return 1;
        }
        if (NULL != pre) {
            memcpy(prvt->data, pre, prvt->dataSize);
            free(pre);
        }

        prvt->data[prvt->dataSize - 1] = '\n';
    }

    return 0;
}

int c_csv_add_row(C_CSV obj, char * row[], size_t size) {
    if (NULL == obj) {
        printf("c_csv_add_row: the object is NULL\n");

        return 1;
    }

    if (NULL == row) {
        printf("c_csv_add_row: the row is NULL\n");

        return 1;
    }

    if (1 > size) {
        printf("c_csv_add_row: the size of a data cannot be less than 1\n");

        return 1;
    }

    PRIVATE prvt = (PRIVATE)obj;

    if (0 == prvt->separator) {
        printf("c_csv_add_row: separator is not specified\n");

        return 1;
    }

    if (prvt->numberOfColumns == 0) {
        prvt->numberOfColumns = size;
    } else if (prvt->numberOfColumns != size) {
        printf("c_csv_add_row: data has incorrect number of columns\n");

        return 1;
    }

    size_t i;
    for (i = 0; i < size; i++) {
        if (strchr(row[i], prvt->separator)) {
            printf("c_csv_add_row: the value %s contains a separator\n", row[i]);

            return 1;
        }

        size_t len = strlen(row[i]);
        char * pre = prvt->data;
        size_t prevSize = prvt->dataSize;
        prvt->dataSize += len + 1;
        prvt->data = (char *)malloc(prvt->dataSize);
        if (NULL == prvt->data) {
            if (NULL != pre)
                prvt->data = pre;

            printf("c_csv_add_row: failed memory allocation\n");

            return 1;
        }
        if (NULL != pre) {
            memcpy(prvt->data, pre, prevSize);
            free(pre);
        }

        memcpy(prvt->data + prevSize, row[i], len);

        if (i + 1 != size)
            prvt->data[prvt->dataSize - 1] = prvt->separator;
        else
            prvt->data[prvt->dataSize - 1] = '\n';
    }

    return 0;
}

int C_CSV_WriteFile(C_CSV obj, char fileName[]) {
    if (NULL == obj) {
        printf("C_CSV_WriteFile: the object is NULL\n");

        return 1;
    }

    if (NULL == fileName) {
        printf("C_CSV_WriteFile: the name string is NULL\n");

        return 1;
    }

    if ('\0' == fileName[0]) {
        printf("C_CSV_WriteFile: empty file name\n");

        return 1;
    }

    PRIVATE prvt = (PRIVATE)obj;

    if (NULL == prvt->data) {
        printf("C_CSV_WriteFile: the data is NULL\n");

        return 1;
    }
    if (0 == prvt->dataSize) {
        printf("C_CSV_WriteFile: the data is empty\n");

        return 1;
    }

    FILE * fp = NULL;
    fp = fopen(fileName, "wb");
    if (NULL == fp) {
        printf("C_CSV_WriteFile: failed %s opening\n", fileName);

        return 1;
    }

    size_t n = fwrite(prvt->data, prvt->dataSize, 1, fp);
    if (n != 1) {
        printf("C_CSV_WriteFile: error writting data to a file\n");
        fclose(fp);

        return 1;
    }
    fclose(fp);

    return 0;
}

int c_csv_get_value(C_CSV obj, size_t row, size_t col, char * buffer, size_t bufSize) {
    if (NULL == obj) {
        printf("C_CSV_GetValue: the object is NULL\n");

        return 1;
    }

    if (NULL == buffer) {
        printf("C_CSV_GetValue: the buffer is NULL\n");

        return 1;
    }

    if (0 == bufSize) {
        printf("C_CSV_GetValue: the buffer size argument is 0\n");

        return 1;
    }

    PRIVATE prvt = (PRIVATE)obj;

    if (row > prvt->numberOfRows - 1 ||
        row < 0 ||
        col > prvt->numberOfColumns - 1 ||
        col < 0) {
        printf("C_CSV_GetValue: the arguments passed are out of range (%lld rows, %lld columns)\n", prvt->numberOfRows, prvt->numberOfColumns);

        return 1;
    }

    size_t tmpCol = 0;
    size_t tmpRow = 0;
    size_t i;
    for (i = 0; i < prvt->dataSize; i++) {
        if (row == tmpRow && col == tmpCol) {
            prvt->lastValueLen = 0;
            while (prvt->data[i + prvt->lastValueLen] != prvt->separator &&
                prvt->data[i + prvt->lastValueLen] != '\n' &&
                i + prvt->lastValueLen != prvt->dataSize) {
                prvt->lastValueLen++;
            }

            if (prvt->lastValueLen >= bufSize){
                printf("C_CSV_GetValue: the length (%lld) of the proposed buffer is less than the length (%lld) of the value\n", bufSize, prvt->lastValueLen);
                /* strncpy(buffer, &prvt->data[i], bufSize - 1); */
                strcpy(buffer, "");
                
                return 1;
            } else {
                strncpy(buffer, &prvt->data[i], prvt->lastValueLen);
            }
            prvt->position = i;

            return 0;
        }
        if (prvt->data[i] == prvt->separator || prvt->data[i] == '\n' || i + 1 == prvt->dataSize) {
            tmpCol++;

            if (prvt->data[i] == '\n' || i + 1 == prvt->dataSize) {
                tmpRow++;
                tmpCol = 0;
            }
        }
    }

    return 1;
}

int c_csv_getlastsavedvalue(C_CSV obj, char * buffer, size_t bufSize) {
    if (NULL == obj) {
        printf("C_CSV_GetValue: the object is NULL\n");

        return 1;
    }

    PRIVATE prvt = (PRIVATE)obj;

    if (prvt->lastValueLen >= bufSize){
        printf("C_CSV_GetValue: the length (%lld) of the proposed buffer is less than the length (%lld) of the value\n", bufSize, prvt->lastValueLen);
        /* strncpy(buffer, &prvt->data[prvt->position], bufSize - 1); */
        strcpy(buffer, "");

        return 1;
    } else {
        strncpy(buffer, &prvt->data[prvt->position], prvt->lastValueLen);
    }

    return 0;
}