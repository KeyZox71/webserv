# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/25 16:09:27 by adjoly            #+#    #+#              #
#    Updated: 2025/05/28 09:42:17 by adjoly           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL = bash

NAME = webserv

CC = clang++

OBJSDIR = obj/

INCLUDES = ./includes

SRCS = $(shell find . -name '*.cpp')

OBJS = $(addprefix $(OBJSDIR), $(SRCS:.cpp=.o))

FLAGS = -Wall -Werror -Wextra -std=c++98 -MMD -MP -g #-fsanitize=address

RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[1;33m
PURPLE = \e[0;35m
NC = \033[0m
DELETE = \x1B[2K\r

ifeq ($(VERBOSE),true)
	FLAGS += -D VERBOSE
endif

ifeq ($(TTY),true)
	FLAGS += -D TTY
endif

ifeq ($(PKGS),true)
	FLAGS += -D PKGS
endif

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) -I$(INCLUDES) $(OBJS) -o $(NAME)
	@printf "$(YELLOW)「✨」 feat($(NAME)): program compiled\n"

$(OBJSDIR)%.o: %.cpp
	@mkdir -p $(@D)
	@$(CC) $(FLAGS) -I$(INCLUDES) -Ilib/tomlpp/includes -c $< -o $@
	@printf "$(DELETE)$(GREEN)「🔨」 build($<): object compiled\n"

clean:
	@rm -f $(OBJS)
	@printf "$(DELETE)$(RED)「🗑️」 clean($(OBJS)): object deleted\n"

fclean: clean
	@rm -f $(NAME)
	@rm -Rf $(OBJSDIR)
	@printf "$(RED)「🗑️」 fclean($(NAME)): program deleted\n"

re: fclean
	@$(MAKE) -s all

.PHONY: clean fclean all re
