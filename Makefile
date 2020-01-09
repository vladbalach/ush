NAME = ush

INC = inc/ush.h \
	./libmx/inc/libmx.h \
	./libmx/libmx.a

HEADTMP = ush.h\

OBJ_DIR = ./obj/
SRC_DIR = ./src/

SRC = main.c \
	mx_cd.c \
	mx_printstr_env.c \
	mx_env.c mx_pwd.c mx_echo.c \
	mx_export.c
	

SRCS = $(addprefix $(SRC_DIR), $(SRC))

OBJ = $(SRC:.c=.o)

OBJ_D = $(addprefix $(OBJ_DIR), $(OBJ))

CFLSGS = -std=c11 #-Wall -Wextra -Wpedantic -Werror 

all: install clean

install:
	@make install -C ./libmx
	@cp $(INC) $(SRCS) .
	@clang $(CFLSGS) -c  $(SRC)
	@mkdir -p obj
	@mv $(OBJ) ./obj
	@clang $(CFLSGS) $(OBJ_D) libmx.a -o $(NAME)
	@make uninstall -C ./libmx
uninstall:
	@make clean
	@rm -rf $(NAME)
clean: 
	@rm -rf $(OBJ_D) $(SRC) $(HEADTMP) libmx.a
	@rm -rf ./obj
reinstall: uninstall install