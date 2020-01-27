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
	mx_export.c \
	mx_parsing.c \
	mx_ush_init.c \
	mx_insert_tree.c \
	mx_create_tnode.c \
	mx_delete_tnode.c \
	mx_get_min_tnode.c \
	mx_find_tnode.c \
	mx_create_token.c \
	mx_lexer.c \
	mx_input.c \
	out_monitor.c \
	mx_clear_tokens.c \
	mx_is_char.c \
	mx_get_next_token.c \
	mx_execute.c \
	mx_get_token_type.c \
	mx_create_ast.c \
	mx_syntax_analyzer.c \
	Key_TAB.c \
	mx_delete_ast.c \
	mx_execute_tree.c \
	mx_exec_more.c \
	mx_one_symbol.c \
	mx_not_ascii.c \
	bit_function.c \

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