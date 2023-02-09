CC = gcc
src_dir = src
obj_dir = obj
header_dir = include
# cflags = -g -Wall -Werror -Wextra -I $(header_dir)
cflags = -g -Wall -Wextra -I $(header_dir)
headers = $(wildcard $(header_dir)/*.h)
sources = $(wildcard $(src_dir)/*.c)
objects = $(patsubst $(src_dir)/%.c,$(obj_dir)/%.o,$(sources))
product_name = $(notdir $(shell pwd))

$(product_name): obj $(objects)
	@echo Compiling of $(sources) finished
	@$(CC) $(cflags) -o $(product_name) $(objects)

all: $(product_name)

obj: 
	@echo Directory obj created
	@mkdir obj

$(obj_dir)/%.o: $(src_dir)/%.c $(headers)
	@echo Compiling $@ from $<
	@$(CC) $(cflags) -o $@ -c $<

clean:
	@rm -rf obj $(product_name)

.PHONY: clean build
