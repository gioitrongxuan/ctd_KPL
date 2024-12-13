# Tên chương trình thực thi
TARGET = main

# Trình biên dịch và các cờ biên dịch
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Danh sách tệp nguồn và tệp đối tượng
SRC = main.c
OBJ = $(SRC:.c=.o)

# Quy tắc mặc định
all: $(TARGET)

# Quy tắc để tạo tệp thực thi
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Quy tắc biên dịch tệp .o từ tệp .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Quy tắc dọn dẹp các tệp tạm
clean:
	rm -f $(OBJ) $(TARGET)
