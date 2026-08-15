CC := cc
CFLAGS := -Wall -Wextra -Werror -g3
CPPFLAGS := -IUnity/src -I../Assignments/include -I../Assignments/libft
LDLIBS := -lreadline
SANITIZER_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer

UNITY_SOURCE := Unity/src/unity.c
TEST_RUNNER := scripts/run_tests.sh
TEST_SOURCES := $(wildcard app/test_*.c cmd/test_*.c lexer/test_*.c \
	parser/test_*.c parsing_facade/test_*.c token/test_*.c util/test_*.c)
TEST_SOURCES += $(wildcard env/test_*.c)
TEST_BINARIES := $(patsubst %.c,build/%,$(TEST_SOURCES))
SANITIZER_BINARIES := $(patsubst %.c,build/sanitize/%,$(TEST_SOURCES))
PRODUCTION_SOURCES := $(wildcard ../Assignments/src/*/*.c)
LIBFT_SOURCES := $(wildcard ../Assignments/libft/*.c)
PRODUCTION_OBJECTS := $(patsubst ../Assignments/src/%.c,build/obj/src/%.o,\
	$(PRODUCTION_SOURCES))
LIBFT_OBJECTS := $(patsubst ../Assignments/libft/%.c,build/obj/libft/%.o,\
	$(LIBFT_SOURCES))
SANITIZER_PRODUCTION_OBJECTS := $(patsubst ../Assignments/src/%.c,\
	build/sanitize/obj/src/%.o,$(PRODUCTION_SOURCES))
SANITIZER_LIBFT_OBJECTS := $(patsubst ../Assignments/libft/%.c,\
	build/sanitize/obj/libft/%.o,$(LIBFT_SOURCES))
HEADERS := Unity/src/unity.h Unity/src/unity_internals.h \
	$(wildcard ../Assignments/include/*.h) ../Assignments/libft/libft.h

.SECONDARY: $(PRODUCTION_OBJECTS) $(LIBFT_OBJECTS) \
	$(SANITIZER_PRODUCTION_OBJECTS) $(SANITIZER_LIBFT_OBJECTS)

all: test

test: $(TEST_BINARIES)
	@$(TEST_RUNNER) $(TEST_BINARIES)

app: $(filter build/app/%,$(TEST_BINARIES))
cmd: $(filter build/cmd/%,$(TEST_BINARIES))
lexer: $(filter build/lexer/%,$(TEST_BINARIES))
parser: $(filter build/parser/%,$(TEST_BINARIES))
parsing_facade: $(filter build/parsing_facade/%,$(TEST_BINARIES))
token: $(filter build/token/%,$(TEST_BINARIES))
util: $(filter build/util/%,$(TEST_BINARIES))
env: $(filter build/env/%,$(TEST_BINARIES))

app cmd lexer parser parsing_facade token util env:
	@$(TEST_RUNNER) $^

sanitize: $(SANITIZER_BINARIES)
	@$(TEST_RUNNER) --sanitize $(SANITIZER_BINARIES)

memory: $(TEST_BINARIES)
	@$(TEST_RUNNER) --valgrind $(TEST_BINARIES)

build/sanitize/%: %.c $(UNITY_SOURCE) $(SANITIZER_PRODUCTION_OBJECTS) \
		$(SANITIZER_LIBFT_OBJECTS) $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(SANITIZER_FLAGS) $(CPPFLAGS) \
		$(UNITY_SOURCE) $< $(SANITIZER_PRODUCTION_OBJECTS) \
		$(SANITIZER_LIBFT_OBJECTS) $(SANITIZER_FLAGS) $(LDLIBS) -o $@

build/%: %.c $(UNITY_SOURCE) $(PRODUCTION_OBJECTS) $(LIBFT_OBJECTS) $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(UNITY_SOURCE) $< \
		$(PRODUCTION_OBJECTS) $(LIBFT_OBJECTS) $(LDLIBS) -o $@

build/obj/src/%.o: ../Assignments/src/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/obj/libft/%.o: ../Assignments/libft/%.c ../Assignments/libft/libft.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/sanitize/obj/src/%.o: ../Assignments/src/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(SANITIZER_FLAGS) $(CPPFLAGS) -c $< -o $@

build/sanitize/obj/libft/%.o: ../Assignments/libft/%.c \
		../Assignments/libft/libft.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(SANITIZER_FLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) -r build

fclean: clean

re: fclean all

.PHONY: all test app cmd lexer parser parsing_facade token util env sanitize \
	memory clean fclean re
