# SPDX-License-Identifier: MIT
# Author:  Giovanni Santini
# Mail:    giovanni.santini@proton.me
# Github:  @San7o

OUT = buddy-system
OBJ = buddy-system.o \
      test.o
CFLAGS = -Wall -Wextra -Wpedantic -Werror

all: $(OUT)

.PHONY: run
run:
	./buddy-system

$(OUT): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(OUT)

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@
