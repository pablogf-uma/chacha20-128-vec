CC = gcc
CFLAGS = -O -c
TARGET = chacha20_v
OBJS = chacha20_v.o whole_round.o state_init.o rows_to_vectors.o

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -O -o $(TARGET)

chacha20_v.o: chacha20_v.c
	$(CC) $(CFLAGS) chacha20_v.c 

whole_round.o: whole_round.c
	$(CC) $(CFLAGS) whole_round.c 

state_init.o: state_init.c
	$(CC) $(CFLAGS) state_init.c

rows_to_vectors.o: rows_to_vectors.c
	$(CC) $(CFLAGS) rows_to_vectors.c

clean: 
	cmd /C del /Q $(TARGET) $(OBJS) chacha20_v.exe

.PHONY: clean