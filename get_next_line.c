/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvoisard <jonas.voisard@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 19:32:03 by jvoisard          #+#    #+#             */
/*   Updated: 2024/10/12 22:53:44 by jvoisard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static ssize_t	index_of(t_block *b, char c)
{
	ssize_t	i;
	ssize_t	len;

	i = 0;
	len = b->len;
	while (len-- && b->buffer[i] != c)
		i++;
	if (len)
		return (i);
	return (-1);
}

static t_block	*free_block(t_block *b)
{
	if (b->buffer)
		free(b->buffer);
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
		return (free_block(new));
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
		return (free_block(b));
	if (b->len)
	{
		b->next = read_next_block(fd);
		if (!b->next)
			return (free_block(b));
	}
	return (b);
}

static ssize_t	get_line_len(t_block *b)
{
	ssize_t	nl_index;

	if (!b->len)
		return (0);
	nl_index = index_of(b, '\n');
	if (nl_index != -1)
		return (nl_index + 1);
	return (b->len + get_line_len(b->next));
}

static t_block	*write_next_block(t_block *b, char *line)
{
	t_block	*next;

	while ((b->cursor - b->buffer) < b->len && *(b->cursor) != '\n')
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
	free_block(b);
	if (!next)
		return (0);
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
		free_block(block);
		block = NULL;
		return (0);
	}
	line_len = get_line_len(block);
	line = malloc(line_len + 1);
	if (!line)
		return (0);
	block = write_next_block(block, line);
	return (line);
}
