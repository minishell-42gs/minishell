# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/19 13:29:27 by taegokim          #+#    #+#              #
#    Updated: 2026/07/19 16:38:07 by taegokim         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

CC := cc
RM := rm -f
CFLAGS := -Wall -Wextra -Werror  

MODULES := lexer parser util

INCS_DIR := include
INCLUDES := -I$(INCS_DIR) -I$(LIBFT_DIR)/includes
HEADERS := $(addprefix $(INCS_DIR)/, $(addsuffix .h, $(MODULES)))

SRC_DIR := src
SRCS := $(SRC_DIR)/main.c \
	$(SRC_DIR)/lexer/lexer.c \

OBJ_DIR := build
OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

LIBFT_DIR := ./libft
LIBFT_NAME := libft.a
LIBFT := $(LIBFT_DIR)/$(LIBFT_NAME)


all: $(LIBFT) $(NAME)

$(OBJ_DIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

clean:
	$(MAKE) clean -C $(LIBFT_DIR)
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(MAKE) fclean -C $(LIBFT_DIR)
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
