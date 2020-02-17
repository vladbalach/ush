NAME = ush

SRCD = src
INCD = inc
OBJD = obj
LBMXD = libmx

LMBX = libmx.a
INC = ush.h
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
	mx_key_tab.c \
	mx_delete_ast.c \
	mx_execute_tree.c \
	mx_exec_more.c \
	mx_one_symbol.c \
	mx_not_ascii.c \
	bit_function.c \
	mx_exec_less.c \
	mx_exec_token.c \
	mx_execute_proces.c \
	mx_write_from_to.c \
	mx_print_Tab_comands.c \
	mx_key_delite.c \
	mx_get_info.c \
	mx_ush_close.c \
	mx_close_all_pr.c \
	mx_add_process.c \
	mx_which.c \
	mx_is_buildin.c \
	mx_check_symbol.c \
	mx_end_flag.c \
	mx_parsing_input.c \
	mx_if_new_parameter.c \
	mx_create_comands.c \
	mx_audit_str.c \
	mx_str_bquote.c \
	mx_serch_list.c \
	mx_unset.c \
	mx_pipe_execute.c \
	mx_return_value.c \
	mx_HOME.c \
	mx_do_replace.c \
	mx_ctrl_v_and_not_ascii.c \
	mx_ascii.c \
	mx_ctrl_R.c \
	mx_print_esc.c \
	mx_print_cont.c \
	mx_print_susp.c \
	mx_get_name.c \
	mx_del_top_process.c \
	mx_jobs.c \
	mx_buildin_list.c \
	mx_fg.c \
	mx_exit.c \
	mx_wait_process.c \
	mx_is_link.c \
	mx_chdir_P.c \
	mx_add_one_rank.c \
	mx_del_last_rank.c \
	mx_chdir_L.c \
	mx_key_duble_tab.c \
	mx_mini_parser_tab.c \
	mx_read_comand.c \
	mx_is_operator.c \
	mx_signals.c \
	mx_parametr_shell.c \
	mx_flang_Comand.c \
	mx_read_user.c \
	mx_token_in_program.c \
	mx_start_program.c \
	mx_exec_env_pr.c \
	mx_push_env.c \
	mx_history.c \
	mx_unset_fds.c \
	mx_subs.c \

INCLUDE = -I $(LBMXD) \
	-I $(INCD) \

LBMXS = $(addprefix $(LBMXD)/, $(LMBX))
INCS = $(addprefix $(INCD)/, $(INC))
SRCS = $(addprefix $(SRCD)/, $(SRC))
OBJS = $(addprefix $(OBJD)/, $(SRC:%.c=%.o))
	
CFLAGS = -std=c11 # $(addprefix -W, all extra error pedantic)
CC = clang

all: $(NAME)

install: $(NAME) clean

$(LBMXS):
	@make -sC $(LBMXD)

$(NAME): $(OBJS) $(LBMXS)
	@$(CC) $(CFLAGS) $(LBMXS) $(OBJS) -o $@
	@printf "\x1b[32;1m$@ created\x1b[0m\n"

$(OBJD)/%.o: $(SRCD)/%.c $(INCS)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)
	@printf "\x1b[32mcompiled: \x1b[0m[$(<:$(SRCD)/%.c=%.c)]\n"

$(OBJS): | $(OBJD)

$(OBJD):
	@mkdir -p $@

uninstall: clean
	@make -sC $(LBMXD) $@
	@rm -rf $(NAME)
	@printf "\x1b[34;1mdeleted $(NAME)\x1b[0m\n"

clean:
	@make -sC $(LBMXD) $@
	@rm -rf $(OBJD)
	@printf "\x1b[34;1mdeleted $(OBJD)\x1b[0m\n"

reinstall: uninstall install