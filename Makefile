ROOT_PATH=$(shell pwd)
BIN=httpd
SRC=httpd.c
OBJ=$(SEC:.c=.o)
CC=gcc

$(BIN):$(OBJ)
	$(CC) -O $@ $^
%.o:%.c
	$(CC) -c $<


.PHONY:debug


