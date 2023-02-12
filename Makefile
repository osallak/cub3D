NAME = bin/cub3D
SRC = main.c parser.c get_next_line.c get_next_line_utils.c ft_split.c ft_strlcpy.c ft_split.c ft_atoi.c renderer.c math.c\
      init_textures.c init_gun_tex.c cub_manager_init.c move_player.c rotate_player.c cast.c cast_utils.c assets.c color.c\
	  border_checker.c checker_utils.c checker.c error.c map.c parser_utils.c cast_init.c assets_utils.c\
	  get_textures.c player.c render_walls.c move_player_utils.c color_utils.c renderer_utils.c draw.c handle_events.c common_utils.c init_ver_check.c map_checker.c
CC = gcc
CFLAGS = -Wall -Wextra -Werror -c  -Iinclude   -O3 -ffast-math
LFLAGS = -lmlx -framework OpenGL  -framework AppKit

SRCS = $(addprefix src/, $(SRC))
OBJS = $(addprefix obj/, $(SRC:.c=.o))
OBJDIR = obj
SRCDIR = src

INC = cub3D.h

$(OBJDIR)/%.o : $(SRCDIR)/%.c $(addprefix include/, $(INC))
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