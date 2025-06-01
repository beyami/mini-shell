/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_longer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 21:18:52 by yumiyao           #+#    #+#             */
/*   Updated: 2025/06/01 12:53:48 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_longer_split(char **split, char *new, int len)
{
	int		i;
	char	**rtn;

	i = 0;
	rtn = (char **)ft_malloc(sizeof(char *) * (len + 2));
	if (!rtn || !new)
	{
		free(rtn);
		free_2d_array(split);
		return (NULL);
	}
	while (split[i])
	{
		rtn[i] = split[i];
		++i;
	}
	rtn[i] = ft_strdup(new);
	if (!rtn[i])
	{
		free_2d_array(split);
		return (NULL);
	}
	rtn[i + 1] = NULL;
	free(split);
	return (rtn);
}
