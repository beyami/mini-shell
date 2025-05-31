/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_move.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:10:15 by yumiyao           #+#    #+#             */
/*   Updated: 2025/06/01 03:01:05 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*make_abs_path(char **dest_split, char **inner_split, int inner_len)
{
	int	i;

	i = 0;
	while (dest_split[i])
	{
		if (ft_strncmp(dest_split[i], "..", 3) == 0 && inner_len > 0)
		{
			--inner_len;
			free(inner_split[inner_len]);
			inner_split[inner_len] = NULL;
		}
		else if (ft_strncmp(dest_split[i], ".", 2) != 0)
		{
			inner_split = get_longer_split(inner_split,
					dest_split[i], inner_len);
			if (!inner_split)
			{
				free_2d_array(dest_split);
				return (NULL);
			}
			++inner_len;
		}
		++i;
	}
	return (ft_union(inner_split, '/'));
}

char	*get_abs_path(char *dest)
{
	char	*inner_pwd;
	char	**inner_split;
	char	**dest_split;
	char	*rtn;
	int		inner_len;

	inner_pwd = ft_cwd(PWD_GET, NULL);
	if (!inner_pwd || !dest)
		return (NULL);
	inner_split = ft_split(inner_pwd, '/');
	if (!inner_split)
		return (NULL);
	dest_split = ft_split(dest, '/');
	if (!dest_split)
	{
		free_2d_array(inner_split);
		return (NULL);
	}
	inner_len = 0;
	while (inner_split[inner_len])
		++inner_len;
	rtn = make_abs_path(dest_split, inner_split, inner_len);
	free_2d_array(inner_split);
	free_2d_array(dest_split);
	return (rtn);
}

int	check_cwd(char *path)
{
	char	buf[PATH_MAX];
	char	*rtn;

	if (!path)
		return (1);
	if (path[0] != '.')
		return (0);
	rtn = getcwd(buf, PATH_MAX);
	if (!rtn)
	{
		ft_dprintf(STDERR_FILENO, "cd: error retrieving "
			"current directory: getcwd: %s\n", strerror(errno));
		return (1);
	}
	return (0);
}

char	*get_path(char *dest)
{
	char	*path;
	char	*joined;

	if (dest[0] == '/')
		path = ft_strdup(dest);
	else if (ft_strncmp("./", dest, 2) == 0)
		path = get_abs_path(dest);
	else
	{
		joined = ft_strjoin("./", dest);
		path = get_abs_path(joined);
		free(joined);
	}
	return (path);
}

char	*move_to_some(char *dest)
{
	char	*path;
	int		res;

	if (check_cwd(dest))
		return (NULL);
	path = get_path(dest);
	if (!path)
		inner_exit(1);
	if (check_access(dest, path))
	{
		free(path);
		return (NULL);
	}
	res = chdir(path);
	if (res != 0)
	{
		ft_dprintf(STDERR_FILENO, "cd: %s\n", strerror(errno));
		free(path);
		return (NULL);
	}
	return (path);
}
