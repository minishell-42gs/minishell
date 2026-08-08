# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyuckwon <hyuckwon@student.42gyeongsan.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/19 13:29:27 by taegokim          #+#    #+#              #
#    Updated: 2026/08/02 15:56:29 by hyuckwon         ###   ########.fr        #
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
			util 
INCS_DIR := include
LIBFT_DIR := libft
OBJ_DIR := build
SRC_DIR := src

INCLUDES := -I$(INCS_DIR) -I$(LIBFT_DIR)
HEADERS := $(addprefix $(INCS_DIR)/, $(addsuffix .h, $(MODULES)))

SRCS := $(SRC_DIR)/main.c\
		$(SRC_DIR)/app/app.c\
		$(SRC_DIR)/cmd/cmd_factory.c\
		$(SRC_DIR)/cmd/cmd_list.c\
		$(SRC_DIR)/cmd/cmd.c\
		$(SRC_DIR)/lexer/lexer.c\
		$(SRC_DIR)/parser/parser.c\
		$(SRC_DIR)/parsing_facade/parsing_facade.c\
		$(SRC_DIR)/token/token_factory.c\
		$(SRC_DIR)/token/token_list.c\
		$(SRC_DIR)/token/token.c\
		$(SRC_DIR)/util/free_split.c\
		$(SRC_DIR)/util/is_blank.c
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
