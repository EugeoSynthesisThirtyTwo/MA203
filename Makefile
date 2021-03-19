CC=gcc
AR=ar
ARFLAGS=-cvq
CFLAGS=-I. -Wall -Wno-unused-result -O3
EXEC=genkey encrypt decrypt

.PHONY:clean mrproper

all:$(EXEC) clean

genkey:genkey.o Blum.o
	$(CC) $(CFLAGS) genkey.o Blum.o -o genkey -lgmp

genkey.o:genkey.c
	$(CC) $(CFLAGS) -c genkey.c -o genkey.o

encrypt:encrypt.o Blum.o
	$(CC) $(CFLAGS) encrypt.o Blum.o -o encrypt -lgmp

encrypt.o:encrypt.c
	$(CC) $(CFLAGS) -c encrypt.c -o encrypt.o

decrypt:decrypt.o Blum.o
	$(CC) $(CFLAGS) decrypt.o Blum.o -o decrypt -lgmp

decrypt.o:decrypt.c
	$(CC) $(CFLAGS) -c decrypt.c -o decrypt.o

Blum.o:Blum.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	find . -name '*.o' -exec rm -f {} ';'
	find . -name '*~' -exec rm -f {} ';'
	find . -name '#*#' -exec rm -f {} ';'
	find . -name '.#*' -exec rm -f {} ';'
	find . -name 'core' -exec rm -f {} ';'
	find . -name '*.core' -exec rm -f {} ';'

mrproper:clean
	rm -rf $(EXEC)
	rm -rf *.a
	rm -rf *.so

pkg:mrproper
	tar -cvf ../archive.tgz .
