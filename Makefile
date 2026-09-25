CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g3
SOURCES = graphics.c utils.c sound.c eastereggs/cobra/cobra.c \
	eastereggs/devcalc/diff.c eastereggs/devcalc/error.c \
	eastereggs/devcalc/parse.c eastereggs/devcalc/simplify.c \
	eastereggs/devcalc/struct.c eastereggs/devcalc/utility.c \
	eastereggs/devcalc/devcalc.c problems.c \
	problems/problem_0.c problems/lesson01/problem_1.c problems/lesson02/problem_2.c \
	problems/lesson02/problem_3.c problems/lesson03/problem_4.c problems/lesson03/problem_5.c \
	problems/lesson03/problem_6.c problems/lesson04/problem_7.c \
	problems/lesson04/problem_71.c problems/lesson04/problem_72.c problems/lesson04/problem_73.c \
	problems/lesson04/problem_74.c problems/lesson04/problem_75.c problems/lesson04/problem_76.c \
	problems/lesson04/problem_77.c main.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = main.exe

BASS_DIR = lib
BASS_DLL = $(BASS_DIR)/bass.dll
BASS_DEF = $(BASS_DIR)/bass.def
BASS_LIB = $(BASS_DIR)/libbass.a
LDFLAGS  = -L$(BASS_DIR) -lbass

all: $(TARGET)

$(TARGET): $(OBJECTS) bass.dll
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(OBJECTS): $(BASS_LIB)

$(BASS_LIB): $(BASS_DEF) $(BASS_DLL)
	dlltool -d $(BASS_DEF) -D bass.dll -l $(BASS_LIB)

bass.dll: $(BASS_DLL)
	copy /Y "$(subst /,\,$(BASS_DLL))" bass.dll >nul

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q $(subst /,\,$(OBJECTS)) $(TARGET) bass.dll