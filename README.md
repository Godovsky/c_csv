# A simple library for working with CSV files.
I decided to write this library for myself to help me work with various CSV format data. I used to use EXCEL, which also works with CSV files, but it is able to process only a limited number of rows, and EXCEL can be very slow if the file is very large. The library allows you to conveniently read CSV files with several types of separators (';', ' ' (space), ':', ',', '\ t' (tab)), extract values by row and column numbers, add rows to the end of the data, write a file. An example of using the library is given in the example.c file in the apps directory.

# Building
Enter the make command, after that the utility will give you possible options
(examples: "* make lib - to build libc_csv.dll", "* make apps - to build libc_csv.dll and example.exe",
"* make run-example - to run one of the app", "* make clean - to remove all the binaries"):

```shell
make
```