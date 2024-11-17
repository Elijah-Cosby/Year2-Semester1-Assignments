CC = gcc
CFLAGS = -O3
EXEC = VADER_sentiment

SRC = main.c vaderSentiment.c 
HEADER = utility.h 

all: $(EXEC)

$(EXEC): $(SRC) $(HEADER)
	$(CC) $(CFLAGS) $(SRC) -lm -o $(EXEC)

clean:
	rm -rf $(EXEC)

run: $(EXEC)
	./$(EXEC) 