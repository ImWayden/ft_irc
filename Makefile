# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wayden <wayden@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/30 16:23:49 by wayden            #+#    #+#              #
#    Updated: 2025/07/01 15:59:45 by wayden           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -I./headers#-DDEBUG_BUILD=0

SRCS = main.cpp
OBJS = $(patsubst %.cpp,obj/%.o,$(SRCS))
NAME = ircserv

all: obj $(NAME)

obj:
	mkdir -p obj

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

obj/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

re: fclean all

clean:
	rm -rf obj

fclean: clean
	rm -f $(NAME) $(DEBUG_NAME)
