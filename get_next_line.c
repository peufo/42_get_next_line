/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvoisard <jonas.voisard@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 19:32:03 by jvoisard          #+#    #+#             */
/*   Updated: 2024/10/13 20:25:57 by jvoisard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static short	readable_len(t_block *b)
{
	return (b->len - (b->cursor - b->buffer));
}

static ssize_t	index_of(t_block *b, char c)
{
	ssize_t	i;
	ssize_t	len;

	i = 0;
	len = readable_len(b);
	while (len-- && b->cursor[i] != c)
		i++;
	if (len >= 0 && b->cursor[i] == c)
		return (i);
	return (-1);
}

static t_block	*free_block(t_block *b, short free_next)
{
	if (b->buffer)
		free(b->buffer);
	if (free_next && b->next)
		free_block(b->next, 1);
	free(b);
	return (NULL);
}

static t_block	*new_block(void)
{
	t_block	*new;

	new = malloc(sizeof(*new));
	if (!new)
		return (0);
	new->next = NULL;
	new->buffer = malloc(BUFFER_SIZE);
	new->cursor = new->buffer;
	if (!new->buffer)
		return (free_block(new, 0));
	return (new);
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
	if (b->len)
	{
		b->next = read_next_block(fd);
		if (!b->next)
			return (free_block(b, 0));
	}
	return (b);
}

static ssize_t	get_line_len(t_block *b)
{
	ssize_t	nl_index;
	ssize_t	len;

	len = readable_len(b);
	if (!len)
		return (0);
	nl_index = index_of(b, '\n');
	if (nl_index != -1)
		return (nl_index + 1);
	if (b->next)
		return (len + get_line_len(b->next));
	return (len);
}

static t_block	*write_next_block(t_block *b, char *line)
{
	t_block	*next;
	ssize_t	len;

	len = readable_len(b);
	while (len-- && *(b->cursor) != '\n')
		(*line++) = *(b->cursor++);
	if (*(b->cursor) == '\n')
	{
		*(line++) = '\n';
		*line = '\0';
		b->cursor++;
		return (b);
	}
	*line = '\0';
	next = b->next;
	free_block(b, 0);
	if (!next)
		return (0);
	if (!next->len)
		return (next);
	return (write_next_block(next, line));
}

char	*get_next_line(int fd)
{
	ssize_t			line_len;
	char			*line;
	static t_block	*block;

	if (!block)
		block = read_next_block(fd);
	if (!block)
		return (0);
	if (!block->len)
	{
		free_block(block, 1);
		block = NULL;
		return (0);
	}
	line_len = get_line_len(block);
	if (!line_len)
	{
		free_block(block, 1);
		block = NULL;
		return (0);
	}
	line = malloc(line_len + 1);
	if (!line)
	{
		free_block(block, 1);
		block = NULL;
		return (0);
	}
	block = write_next_block(block, line);
	return (line);
}
