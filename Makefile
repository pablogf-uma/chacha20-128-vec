# Detect the Operating System
ifeq ($(OS),Windows_NT)
    DEL = cmd /C del /Q
else
    DEL = rm -f
endif

CC = gcc
CFLAGS = -O3 -march=native -Wall -flto
GEN_FLAGS = $(CFLAGS) -fprofile-generate -c
USE_FLAGS = $(CFLAGS) -fprofile-use -c
TARGET = chacha20
OBJS = chacha20_v128.o double_whole_round_v128.o state_init.o state_to_vectors_v128.o \
		 permute_state_v128.o vectors_to_state_v128.o encrypt_v128.o run_encrypt_test.o \
		 calculate_throughput.o decrypt_v128.o run_decrypt_test.o encrypt_custom_input.o \
		 decrypt_custom_input.o

# Default target
all: profile-use

# Profile generation phase
profile-gen: clean
	$(MAKE) OBJS="$(patsubst %.o,%.gen.o,$(OBJS))" CFLAGS="$(GEN_FLAGS)" $(TARGET)
	# Run the program to generate profile data
	./$(TARGET)

# Use profile data for optimization
profile-use: profile-gen
	$(MAKE) clean
	$(MAKE) OBJS="$(patsubst %.o,%.use.o,$(OBJS))" CFLAGS="$(USE_FLAGS)" $(TARGET)

# Object file rules
%.gen.o: %.c
	$(CC) $(GEN_FLAGS) $< -o $@

%.use.o: %.c
	$(CC) $(USE_FLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

# Linking
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -flto -o $(TARGET)

clean:
	$(DEL) $(TARGET) *.o *.gen.o *.use.o *.gcda *.gcno chacha20.exe

.PHONY: clean all profile-gen profile-use
