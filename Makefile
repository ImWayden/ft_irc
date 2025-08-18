# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wayden <wayden@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/30 16:23:49 by wayden            #+#    #+#              #
#    Updated: 2025/08/18 18:35:26 by wayden           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -g3 -std=c++98 -I./headers -I./headers/classes -I./headers/classes/struct#-DDEBUG_BUILD=0

NAME     := ircserv

SRC_DIR  := src
OBJ_DIR  := obj
LOG_DIR  := logs

SRCS     := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS     := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	@if [ -d $(LOG_DIR) ]; then rm -f $(LOG_DIR)/*.log; fi

fclean: clean
	rm -f $(NAME)
	@if [ -d $(LOG_DIR) ]; then rm -rf $(LOG_DIR); fi

re: fclean all

.PHONY: all clean fclean re