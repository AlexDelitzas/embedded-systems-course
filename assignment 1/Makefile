SHELL := /bin/bash

# ==================================================
# COMMANDS
CC = gcc -O3
RM = rm -f
MKDIR = mkdir -p
RMDIR = $(RM) -r

# ==================================================
# DIRECTORIES
SRC = src
BIN = bin

# ==================================================
# TARGETS
EXEC1 = $(BIN)/sampling_v1
EXEC2 = $(BIN)/sampling_v2

# ==================================================
# COMPILATION
all: $(BIN) $(EXEC1) $(EXEC2)

$(BIN):
	$(MKDIR) $@

$(EXEC1): $(SRC)/sampling_v1.c
	$(CC) -o $@ $^ -lm

$(EXEC2): $(SRC)/sampling_v2.c
	$(CC) -o $@ $^

clean:
	$(RM) $(EXEC1) $(EXEC2) $(SRC)/*~ *~

purge: clean
	$(RMDIR) $(BIN)
