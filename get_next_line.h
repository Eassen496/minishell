/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 01:47:02 by abitonti          #+#    #+#             */
/*   Updated: 2022/11/10 18:23:33 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

typedef struct s_line
{
	char			*buff;
	int				start;
	int				end;
	int				i;
	struct s_line	*next;
}	t_line;

char	*get_next_line(int fd);

#endif
