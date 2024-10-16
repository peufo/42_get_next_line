/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvoisard <jonas.voisard@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 19:32:03 by jvoisard          #+#    #+#             */
/*   Updated: 2024/10/15 22:34:26 by jvoisard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*get_reader_nl(t_reader *reader)
{
	char	*cursor;
	ssize_t	len;

	len = reader->len;
	cursor = reader->cursor;
	while (len--)
	{
		if (*cursor == '\n')
			return (cursor);
		cursor++;
	}
	return (0);
}

static t_reader	*next_reader(t_reader *reader, int fd)
{
	if (!reader)
	{
		reader = malloc(sizeof(*reader));
		if (!reader)
			return (0);
		reader->cursor = reader->buffer;
		reader->len = read(fd, reader->buffer, BUFFER_SIZE);
		if (reader->len == -1)
		{
			free(reader);
			return (0);
		}
		reader->next = NULL;
		reader->nl = get_reader_nl(reader);
		reader->is_end = reader->len < BUFFER_SIZE;
	}
	if (reader->nl || reader->is_end)
		return (reader);
	reader->next = next_reader(reader->next, fd);
	if (!reader->next)
	{
		free(reader);
		return (0);
	}
	return (reader);
}

static ssize_t	get_line_len(t_reader *reader)
{
	if (reader->is_end)
		return (0);
	if (reader->nl)
		return (reader->nl - reader->cursor + 1);
	return (reader->len + get_line_len(reader->next));
}

static t_reader	*write_line(t_reader *reader, char *line)
{
	t_reader	*next;

	while (reader->len && (!reader->nl || reader->cursor <= reader->nl))
	{
		*(line++) = *(reader->cursor++);
		reader->len--;
	}
	reader->nl = get_reader_nl(reader);
	if (reader->len)
		return (reader);
	if (!reader->next)
		return (reader);
	next = write_line(reader->next, line);
	free(reader);
	return (next);
}

static short	is_end(t_reader *reader)
{
	if (reader->len)
		return (0);
	if (reader->next)
		return (is_end(reader->next));
	return (reader->is_end);
}

static void	*free_reader(t_reader *reader)
{
	if (reader->next)
		free_reader(reader->next);
	free(reader);
	return (0);
}

char	*get_next_line(int fd)
{
	static t_reader	*reader;
	ssize_t			line_len;
	char			*line;

	reader = next_reader(reader, fd);
	if (!reader)
		return (0);
	line_len = get_line_len(reader);
	if (!line_len && is_end(reader))
	{
		free_reader(reader);
		reader = NULL;
		return (0);
	}
	line = malloc(line_len + 1);
	if (!line)
	{
		//clean reader
		return (0);
	}
	reader = write_line(reader, line);
	line[line_len] = '\0';
	return (line);
}
