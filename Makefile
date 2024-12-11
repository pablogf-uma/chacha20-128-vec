# Detect the Operating System
ifeq ($(OS),Windows_NT)
    DEL = cmd /C del /Q
else
    DEL = rm -f
endif

CC = gcc
CFLAGS_BASE = -O3 -march=native -Wall -flto -c
CFLAGS_GEN = $(CFLAGS_BASE) -fprofile-generate # Flags for profiling generation
CFLAGS_USE = $(CFLAGS_BASE) -fprofile-use     # Flags for using profile data

TARGET = chacha20
OBJS = chacha20.o quarter_round.o state_init.o permute_state.o encrypt.o run_encrypt_test.o \
       calculate_throughput.o decrypt.o run_decrypt_test.o encrypt_custom_input.o decrypt_custom_input.o

# Profile Generation Step
profile: clean $(OBJS_GEN)
	$(CC) $(OBJS) -fprofile-generate -flto -o $(TARGET)
	@echo "Run the program './$(TARGET)' to generate profiling data"

# Optimized Build Step
$(TARGET): clean $(OBJS_USE)
	$(CC) $(OBJS) -fprofile-use -flto -o $(TARGET)

# Rules for Generating Object Files
%.o: %.c
	$(CC) $(CFLAGS_GEN) $< -o $@ # Profile generation step

# Recompile Object Files Using Profile Data
$(OBJS_USE): %.o: %.c
	$(CC) $(CFLAGS_USE) $< -o $@ # Profile usage step

clean:
	$(DEL) $(TARGET) $(OBJS) *.gcda *.gcno *.exe

.PHONY: clean profile
