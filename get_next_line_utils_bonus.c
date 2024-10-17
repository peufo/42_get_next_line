/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvoisard <jvoisard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:37:22 by jvoisard          #+#    #+#             */
/*   Updated: 2024/10/17 21:03:33 by jvoisard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*get_str_left(char **strs_left, int fd)
{
	if (!strs_left[fd])
	{
		strs_left[fd] = malloc(BUFFER_SIZE);
		if (!strs_left[fd])
			return (0);
		strs_left[fd][0] = '\0';
	}
	return (strs_left[fd]);
}

void	*free_str_left(char **strs_left, int fd)
{
	free(strs_left[fd]);
	strs_left[fd] = NULL;
	return (0);
}
