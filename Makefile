NAME = bin/cub3D
SRC = main.c parser.c get_next_line.c get_next_line_utils.c ft_split.c ft_strlcpy.c ft_split.c ft_atoi.c renderer.c
CC = gcc
CFLAGS = -Wall -Wextra -c -Iinclude -g -fsanitize=address,integer,undefined   -O3 -ffast-math -msse4.2 -mtune=intels
LFLAGS = -lmlx -framework OpenGL -g -framework AppKit -fsanitize=address,integer,undefined

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

run: $(NAME)
	./$(NAME) maps/subject_map.cub

re : fclean $(NAME)