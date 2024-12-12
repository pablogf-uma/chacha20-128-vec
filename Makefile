# Detect the Operating System
ifeq ($(OS),Windows_NT)
    DEL = cmd /C del /Q
else
    DEL = rm -f
endif

CC = gcc
CFLAGS = -O3 -march=native -Wall -flto -fprofile-generate -c
TARGET = chacha20
OBJS = chacha20_v128.o double_whole_round_v128.o state_init.o state_to_vectors_v128.o \
		 permute_state_v128.o vectors_to_state_v128.o encrypt_v128.o run_encrypt_test.o \
		 calculate_throughput.o decrypt_v128.o run_decrypt_test.o encrypt_custom_input.o \
		 decrypt_custom_input.o

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -flto -o $(TARGET)

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

run_encrypt_test.o: run_encrypt_test.c
	$(CC) $(CFLAGS) run_encrypt_test.c
	
calculate_throughput.o: calculate_throughput.c
	$(CC) $(CFLAGS) calculate_throughput.c
	
decrypt_v128.o: decrypt_v128.c
	$(CC) $(CFLAGS) decrypt_v128.c

run_decrypt_test.o: run_decrypt_test.c
	$(CC) $(CFLAGS) run_decrypt_test.c

encrypt_custom_input.o: encrypt_custom_input.c
	$(CC) $(CFLAGS) encrypt_custom_input.c

decrypt_custom_input.o: decrypt_custom_input.c
	$(CC) $(CFLAGS) decrypt_custom_input.c

clean: 
	$(DEL) $(TARGET) $(OBJS) chacha20.exe

.PHONY: clean