CC = gcc
CFLAGS = -mavx512f -O -c
TARGET = chacha20_v
OBJS = chacha20_v.o double_whole_round.o state_init.o rows_to_vectors.o permute_v_state.o

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -mavx512f -O -o $(TARGET)

chacha20_v.o: chacha20_v.c
	$(CC) $(CFLAGS) chacha20_v.c 

double_whole_round.o: double_whole_round.c
	$(CC) $(CFLAGS) double_whole_round.c 

state_init.o: state_init.c
	$(CC) $(CFLAGS) state_init.c

rows_to_vectors.o: rows_to_vectors.c
	$(CC) $(CFLAGS) rows_to_vectors.c

permute_v_state.o: permute_v_state.c
	$(CC) $(CFLAGS) permute_v_state.c

clean: 
	cmd /C del /Q $(TARGET) $(OBJS) chacha20_v.exe

.PHONY: clean