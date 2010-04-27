.SILENT:

CC = gcc -c
CC_OP = -Wall -o

LD = gcc
LD_OP = -lssl -o

CRYPT_SRC = ./src

CRYPT_OBJ_DIR = ./objs

CRYPT_OBJS = ./objs/main.o ./objs/crypt.o ./objs/imageUtils.o

CRYPT_NAME = crypt

all:
	@-echo ""
	@-echo "Compilando crypt..."
	$(CC) $(CC_OP) $(CRYPT_OBJ_DIR)/main.o $(CRYPT_SRC)/main.c
	$(CC) $(CC_OP) $(CRYPT_OBJ_DIR)/crypt.o $(CRYPT_SRC)/crypt.c
	$(CC) $(CC_OP) $(CRYPT_OBJ_DIR)/imageUtils.o $(CRYPT_SRC)/imageUtils.c
	$(LD) $(LD_OP) $(CRYPT_NAME) $(CRYPT_OBJS)
	-make clean        

clean:
	@-echo ""
	@-echo "Borrando los archivos objeto."
	-rm $(CRYPT_OBJ_DIR)/*.o
	@-echo ""
	@-echo "Fin"
