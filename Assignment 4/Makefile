CC= gcc
CFLAGS= -Wall -O3
EXEC = pso

SRC = main.c PSO.c OF.c 
HEADERS = utility.h OF_lib.h 
OBJECTS = $(SRC:.c=.o)

all: $(EXEC)
$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXEC)