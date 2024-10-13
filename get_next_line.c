/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvoisard <jonas.voisard@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 19:32:03 by jvoisard          #+#    #+#             */
/*   Updated: 2024/10/13 23:18:14 by jvoisard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*read_file(int fd);
typedef struct s_content
{
	char	*str;
	char	*cursor;
	int		fd;
}	t_content;

static void *free_content(t_content **content)
{
	if (!*content)
		return (0);
	if ((*content)->str)
		free((*content)->str);
	free(*content);
	*content = NULL;
	return (0);
}

static t_content	*new_content(int fd)
{
	t_content	*content;

	content = malloc(sizeof(*content));
	if (!content)
		return (0);
	content->fd = fd;
	content->str = read_file(fd);
	content->cursor = content->str;
	if (!content->str)
		return (free_content(&content));
	return (content);
}

static size_t	get_next_line_len(t_content *content)
{
	size_t	len;

	len = 0;
	while (content->cursor[len])
	{
		if (content->cursor[len] == '\n')
			return (len + 1);
		len++;
	}
	return (len);
	
}

char	*get_next_line(int fd)
{
	static t_content	*content;
	size_t				line_len;
	char				*line;
	char				*_line;

	if (!content)
		content = new_content(fd);
	if (!content)
		return (0);
	line_len = get_next_line_len(content);
	if (!line_len)
		return (free_content(&content));
	line = malloc(line_len + 1);
	if (!line)
		return (free_content(&content));
	_line = line;
	while (line_len--)
		*(_line++) = *(content->cursor++);
	*_line = '\0';
	return (line);
}
