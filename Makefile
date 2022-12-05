NAME = bin/cub3D
SRC = main.c parser.c get_next_line.c get_next_line_utils.c ft_split.c ft_strlcpy.c ft_split.c\
ft_atoi.c
CC = gcc
CFLAGS = -Wall -Wextra -Werror -c -Iinclude -g -fsanitize=address
LFLAGS = -lmlx -framework OpenGL -framework AppKit -fsanitize=address

SRCS = $(addprefix src/, $(SRC))
OBJS = $(addprefix obj/, $(SRC:.c=.o))
OBJDIR = obj
SRCDIR = src

INC = cub3D.h parser.h

$(OBJDIR)/%.o : $(SRCDIR)/%.c $($(addprefix include/, $(INC)))
	$(CC) $(CFLAGS) $< -o $@

$(NAME) : $(OBJS)
	$(CC) $^ $(LFLAGS) -o $@	

clean :
	/bin/rm -rf $(OBJS)

fclean: clean
	/bin/rm -rf $(NAME)

re : fclean $(NAME)