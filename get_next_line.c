/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvoisard <jonas.voisard@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 19:32:03 by jvoisard          #+#    #+#             */
/*   Updated: 2024/10/16 15:59:28 by jvoisard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*use_str_left(char *buffer, char *str_left)
{
	char	*_str_left;

	if (!*str_left)
		return (buffer);
	_str_left = str_left;
	while (*str_left && *str_left != '\n')
		(*buffer++) = (*str_left++);
	if (*str_left == '\n')
		(*buffer++) = (*str_left++);
	*buffer = '\0';
	while (*str_left)
		*(_str_left++) = (*str_left++);
	*(_str_left) = '\0';
	return (buffer);
}

static char	*str_cut(char *buffer, char *cursor, size_t line_len)
{
	char	*cut;
	size_t	cut_len;
	if (!line_len && cursor == buffer)
		return (0);
	cut_len = cursor - buffer;
	cut = malloc(cut_len + line_len + 1);
	if (!cut)
		return (0);
	cut += cut_len + line_len;
	*cut = '\0';
	while (cut_len--)
		*(--cut) = *(--cursor);
	return (cut);
}

static void	save_str_left(char *str_left, char *cursor, size_t len)
{
	while (len--)
		*(str_left++) = *(cursor++);
	*str_left = '\0';
}

static char	*read_next(int fd, char *str_left, size_t line_len)
{
	char	buffer[BUFFER_SIZE];
	char	*cursor;
	ssize_t	bytes_read;
	ssize_t	buffer_len;

	cursor = buffer;
	bytes_read = read(fd, cursor, BUFFER_SIZE - (cursor - buffer));
	if (bytes_read == -1)
		return (0);
	if (!bytes_read)
		return (str_cut(buffer, cursor, line_len));
	buffer_len = bytes_read + (cursor - buffer);
	while (--bytes_read && *cursor != '\n')
		cursor++;
	if (*cursor == '\n')
	{
		save_str_left(str_left, cursor + 1, bytes_read);
		return (str_cut(buffer, cursor + 1, line_len));
	}
	cursor = read_next(fd, str_left, buffer_len + line_len);
	if (!cursor)
		return (0);
	while (buffer_len--)
		*(--cursor) = buffer[buffer_len];
	return (cursor);
}

char	*get_next_line(int fd)
{
	static char	str_left[BUFFER_SIZE];
	char		buffer[BUFFER_SIZE];
	char		*cursor;
	ssize_t		buffer_len;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (0);

	cursor = use_str_left(buffer, str_left);
	if (cursor > buffer && *(cursor - 1) == '\n')
		return (str_cut(buffer, cursor, 0));
	buffer_len = (cursor - buffer);
	cursor = read_next(fd, str_left, cursor - buffer);
	if (!cursor)
		return (0);
	while (buffer_len--)
		*(--cursor) = buffer[buffer_len];
	return (cursor);
}
