# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tg <tg@student.42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/19 13:29:27 by taegokim          #+#    #+#              #
#    Updated: 2026/09/13 15:38:06 by tg               ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

CC := cc
RM := rm -f
CFLAGS := -Wall -Wextra -Werror 
LDFLAGS := -lreadline

MODULES := 	app \
			cmd \
			lexer \
			parser \
			parsing_facade \
			token \
			env \
			error \
			executor \
			built_in \
			io_mgr \
			proc_mgr \
			util 
INCS_DIR := include
LIBFT_DIR := libft
OBJ_DIR := build
SRC_DIR := src

INCLUDES := -I$(INCS_DIR) -I$(LIBFT_DIR)
HEADERS := $(addprefix $(INCS_DIR)/, $(addsuffix .h, $(MODULES)))

SRCS := $(SRC_DIR)/main.c\
		$(SRC_DIR)/app/app.c\
		$(SRC_DIR)/app/app_line.c\
		$(SRC_DIR)/app/signals.c\
		$(SRC_DIR)/cmd/cmd_factory.c\
		$(SRC_DIR)/cmd/cmd_list.c\
		$(SRC_DIR)/cmd/cmd.c\
		$(SRC_DIR)/cmd/create_cmd_path.c\
		$(SRC_DIR)/lexer/lexer.c\
		$(SRC_DIR)/lexer/lexer_tokenize.c\
		$(SRC_DIR)/lexer/lexer_check_syntax.c\
		$(SRC_DIR)/lexer/lexer_check_line_syntax.c\
		$(SRC_DIR)/lexer/lexer_token_helpers.c\
		$(SRC_DIR)/expansion/expansion.c\
		$(SRC_DIR)/expansion/expansion_word.c\
		$(SRC_DIR)/expansion/expansion_process.c\
		$(SRC_DIR)/expansion/expansion_variable.c\
		$(SRC_DIR)/expansion/expansion_heredoc.c\
		$(SRC_DIR)/expansion/expansion_quotes.c\
		$(SRC_DIR)/redirection/redirection.c\
		$(SRC_DIR)/heredoc/heredoc.c\
		$(SRC_DIR)/heredoc/heredoc_file.c\
		$(SRC_DIR)/heredoc/heredoc_write.c\
		$(SRC_DIR)/heredoc/heredoc_readline.c\
		$(SRC_DIR)/env/env_list_extra.c\
		$(SRC_DIR)/parser/parser.c\
		$(SRC_DIR)/parser/parser_factory.c\
		$(SRC_DIR)/parser/parser_impl.c\
		$(SRC_DIR)/parsing_facade/parsing_facade.c\
		$(SRC_DIR)/token/token_list.c\
		$(SRC_DIR)/token/token.c\
		$(SRC_DIR)/env/env_list_get_impl.c\
		$(SRC_DIR)/env/env_list_set_impl.c\
		$(SRC_DIR)/env/env_list_to_envp_impl.c\
		$(SRC_DIR)/env/env_list_unset_impl.c\
		$(SRC_DIR)/env/env_list.c\
		$(SRC_DIR)/env/env.c\
		$(SRC_DIR)/error/error.c\
		$(SRC_DIR)/error/error_syntax.c\
		$(SRC_DIR)/error/error_command.c\
		$(SRC_DIR)/error/error_system.c\
		$(SRC_DIR)/error/error_builtin.c\
		$(SRC_DIR)/error/error_redirection.c\
		$(SRC_DIR)/error/error_internal.c\
		$(SRC_DIR)/util/free_split.c\
		$(SRC_DIR)/util/is_blank.c \
		$(SRC_DIR)/util/is_same_str.c\
		$(SRC_DIR)/executor/executor.c\
		$(SRC_DIR)/executor/executor_parent.c\
		$(SRC_DIR)/io_mgr/io_mgr.c\
		$(SRC_DIR)/proc_mgr/proc_mgr.c\
		$(SRC_DIR)/proc_mgr/proc_mgr_impl.c\
		$(SRC_DIR)/proc_mgr/proc_mgr_wait.c\
		$(SRC_DIR)/proc_mgr/proc_mgr_exec_external.c\
		$(SRC_DIR)/built_in/built_in.c\
		$(SRC_DIR)/built_in/built_in_util.c\
		$(SRC_DIR)/built_in/built_in_echo.c\
		$(SRC_DIR)/built_in/built_in_pwd.c\
		$(SRC_DIR)/built_in/built_in_cd.c\
		$(SRC_DIR)/built_in/built_in_env.c\
		$(SRC_DIR)/built_in/built_in_export.c\
		$(SRC_DIR)/built_in/built_in_unset.c\
		$(SRC_DIR)/built_in/built_in_exit.c
OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

LIBFT_NAME := libft.a
LIBFT := $(LIBFT_DIR)/$(LIBFT_NAME)

all: $(LIBFT) $(NAME)

$(OBJ_DIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT): FORCE
	$(MAKE) -C $(LIBFT_DIR)

FORCE:

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

clean:
	$(MAKE) clean -C $(LIBFT_DIR)
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(MAKE) fclean -C $(LIBFT_DIR)
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re FORCE
