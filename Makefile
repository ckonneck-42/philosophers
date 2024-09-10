# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ckonneck <ckonneck@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/28 11:07:45 by ckonneck          #+#    #+#              #
#    Updated: 2024/09/06 14:12:16 by ckonneck         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


# Source files for Philosophers
PHILO_SRSC = philo.c
PHILO_OBJS = $(PHILO_SRSC:.c=.o)
PHILO_DEPS = $(PHILO_SRSC:.c=.d)
CC = cc -pthread
CFLAGS = -Wall -Wextra -Werror -Iinclude
NAME = philo

all: $(NAME)

$(NAME): $(PHILO_OBJS)
	$(CC) $(CFLAGS) $(PHILO_OBJS) -o $(NAME)
	
%.o: %.c FORCE
	$(CC) -MMD -c $< -o $@

-include $(PHILO_DEPS)

clean:
	rm -f $(PHILO_OBJS) $(PHILO_DEPS)

fclean: clean
	rm -f $(NAME)

re: fclean all

# valgrind rule
valgrind: $(NAME)
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) $(ARGS)
# extra options: --verbose --log-file=valgrind-out.txt
#norminette rule
norminette: $(PHILO_SRSC)
		norminette $(PHILO_SRSC)

# Declare phony targets
.PHONY: all clean fclean re valgrind norminette FORCE

