/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvoisard <jonas.voisard@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:05:17 by jvoisard          #+#    #+#             */
/*   Updated: 2024/10/16 16:07:46 by jvoisard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

static void	put_str(char *str)
{
	while (*str)
		write(1, str++, 1);
}

int	main(void)
{
	int		fd;
	char	*value;

	fd = open("test.txt", O_RDONLY);
	value = get_next_line(fd);
	while (*value)
	{
		put_str(value);
		value = get_next_line(fd);
	}
	put_str("(end)\n");
	close(fd);
}
