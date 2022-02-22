# Author: Martin Pech <xpechm00@stud.fit.vutbr.cz>
PROJECT = ipk_1

CC = gcc
CFLAGS = -std=c99 -Werror -Wall -Wextra -pedantic

default: 
	$(CC) -o hinfosvc index.c

clear:
	rm *.c hinfosvc makefile

run:
	./hinfosvc 12345