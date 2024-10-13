/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvoisard <jonas.voisard@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 19:46:24 by jvoisard          #+#    #+#             */
/*   Updated: 2024/10/13 23:13:11 by jvoisard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

typedef struct s_block	t_block;
struct s_block
{
	t_block	*next;
	char	*buffer;
	ssize_t	len;
	ssize_t	len_total;
};

static void *free_block(t_block *b, short free_next)
{
	if (!b)
		return (0);
	if (b->buffer)
		free(b->buffer);
	if (free_next && b->next)
		free_block(b->next, 1);
	free(b);
	return (0);
}

static t_block	*new_block(void)
{
	t_block	*b;

	b = malloc(sizeof(*b));
	if (!b)
		return (0);
	b->buffer = malloc(BUFFER_SIZE);
	b->next = NULL;
	b->len = 0;
	b->len_total = 0;
	if (!b->buffer)
		return (free_block(b, 0));
	return (b);
}

static t_block	*read_next_block(int fd)
{
	t_block	*b;

	b = new_block();
	if (!b)
		return (0);
	b->len = read(fd, b->buffer, BUFFER_SIZE);
	if (b->len == -1)
		return (free_block(b, 0));
	b->len_total = b->len;
	if (b->len == 0)
		return (b);	
	b->next = read_next_block(fd);
	if (!b->next)
		return (free_block(b, 0));
	b->len_total += b->next->len_total;
	return (b);
}

static void write_next_block(char *dest, t_block *b)
{
	char	*buffer;

	buffer = b->buffer;
	while (b->len--)
		*(dest++) = *(buffer++);
	if (b->next)
		write_next_block(dest, b->next);
}

char *read_file(int fd)
{
	t_block	*b;
	char	*content;

	b = read_next_block(fd);
	if (!b)
		return (0);
	content = malloc(b->len_total + 1);
	if (!content)
		return (free_block(b, 1));
	write_next_block(content, b);
	content[b->len_total] = '\0';
	free_block(b, 1);
	return (content);
}
