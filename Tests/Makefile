NAME := build/test_token
SANITIZER_NAME := build/test_token_sanitize

CC := cc
CFLAGS := -Wall -Wextra -Werror -g3
CPPFLAGS := -IUnity/src -I../Assignments/include -I../Assignments/libft
SANITIZER_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer

UNITY_SOURCE := Unity/src/unity.c
TEST_SOURCES := $(wildcard token/test_*.c)
TOKEN_SOURCES := $(wildcard ../Assignments/src/token/*.c)
SOURCES := $(UNITY_SOURCE) $(TEST_SOURCES) $(TOKEN_SOURCES)
HEADERS := Unity/src/unity.h Unity/src/unity_internals.h \
	../Assignments/include/token.h ../Assignments/include/status.h \
	../Assignments/libft/libft.h

LIBFT_DIR := ../Assignments/libft
LIBFT := $(LIBFT_DIR)/libft.a

all: test

test: $(NAME)
	./$(NAME)

token: test

sanitize: $(SANITIZER_NAME)
	ASAN_OPTIONS=detect_leaks=0 ./$(SANITIZER_NAME)

memory: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all \
		--errors-for-leak-kinds=all --error-exitcode=1 ./$(NAME)

$(NAME): $(SOURCES) $(HEADERS) $(LIBFT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(SOURCES) $(LIBFT) -o $@

$(SANITIZER_NAME): $(SOURCES) $(HEADERS) $(LIBFT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(SANITIZER_FLAGS) $(CPPFLAGS) \
		$(SOURCES) $(LIBFT) $(SANITIZER_FLAGS) -o $@

$(LIBFT): FORCE
	$(MAKE) -C $(LIBFT_DIR)

FORCE:

clean:
	$(RM) -r build

fclean: clean

re: fclean all

.PHONY: all test token sanitize memory clean fclean re FORCE
