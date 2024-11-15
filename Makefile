CC = gcc
CFLAGS = -mavx512f -O3 -Wall -c
TARGET = chacha20
OBJS = chacha20_v128.o double_whole_round_v128.o state_init.o state_to_vectors_v128.o permute_state_v128.o vectors_to_state_v128.o encrypt_v128.o run_test.o calculate_throughput.o

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -mavx512f -O3 -Wall -o $(TARGET)

chacha20_v128.o: chacha20_v128.c
	$(CC) $(CFLAGS) chacha20_v128.c 

double_whole_round_v128.o: double_whole_round_v128.c
	$(CC) $(CFLAGS) double_whole_round_v128.c 

state_init.o: state_init.c
	$(CC) $(CFLAGS) state_init.c

state_to_vectors_v128.o: state_to_vectors_v128.c
	$(CC) $(CFLAGS) state_to_vectors_v128.c

permute_state_v128.o: permute_state_v128.c
	$(CC) $(CFLAGS) permute_state_v128.c

vectors_to_state_v128.o: vectors_to_state_v128.c
	$(CC) $(CFLAGS) vectors_to_state_v128.c

encrypt_v128.o: encrypt_v128.c
	$(CC) $(CFLAGS) encrypt_v128.c

run_test.o: run_test.c
	$(CC) $(CFLAGS) run_test.c

calculate_throughput.o: calculate_throughput.c
	$(CC) $(CFLAGS) calculate_throughput.c

clean: 
	cmd /C del /Q $(TARGET) $(OBJS) chacha20.exe

.PHONY: clean