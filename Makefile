.PHONY: all lib example run clean

CC=gcc
CFLAGS=-Wall -ansi

INCDIR=include
SRCDIR=src

ECHO=echo

HFILES=$(wildcard $(INCDIR)/*.h)
CFILES=$(wildcard $(SRCDIR)/*.c)

CFLAGS+=-I$(INCDIR)

LIBNAME=c_csv

ifeq ($(OS),Windows_NT)
	TARGEXT=.exe
	LIBEXT=.dll
    RM=del
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
            
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
            
        endif
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
		TARGEXT=
		LIBEXT=.so
        RM = rm -fv
    endif
    ifeq ($(UNAME_S),Darwin)
    	TARGEXT=
        LIBEXT=.dylib
        RM=rm -fv
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        
    endif
endif

LIBRARY=lib$(LIBNAME)$(LIBEXT)
TARGET=example$(TARGEXT)

all:
	@$(ECHO) "* make lib     - to build $(LIBRARY)"
	@$(ECHO) "* make example - to build $(LIBRARY) and $(TARGET)"
	@$(ECHO) "* make run     - to run $(TARGET)"
	@$(ECHO) "* make clean   - to remove all the binaries"

example: $(LIBRARY) $(TARGET)

lib: $(LIBRARY)

$(TARGET): example.c $(CFILES) $(HFILES)
	@$(ECHO) "Building $@"
ifeq ($(UNAME_S),Darwin)
	@$(CC) $(CFLAGS) $< -o $@ -Wl,-rpath,@loader_path -L./ -l$(LIBNAME)
else
	@$(CC) $(CFLAGS) $< -o $@ -Wl,-rpath=./ -L./ -l$(LIBNAME)
endif
	

$(LIBRARY): $(SRCDIR)/c_csv.c $(CFILES) $(HFILES)
	@$(ECHO) "Building $@" 
ifeq ($(UNAME_S),Darwin)
	@$(CC) $(CFLAGS) -dynamiclib $< -o $@
else
	@$(CC) $(CFLAGS) -shared $< -o $@
endif

run:
	@./$(TARGET)

clean:
	@$(RM) $(TARGET)
	@$(RM) $(LIBRARY)