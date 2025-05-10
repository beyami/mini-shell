/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 03:40:56 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/07 03:58:18 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_getenv(char *name, char ***envp)
{
	int	i;
	int	name_len;

	if (!is_valid_name(name))
		return (-1);
	i = 0;
	name_len = ft_strlen(name);
	while ((*envp)[i])
	{
		if (ft_strncmp(name, (*envp)[i], name_len) == 0
			&& (*envp)[i][name_len] == '=')
			return (i);
		++i;
	}
	return (-1);
}

int	move_to_home(char ***envp)
{
	int	home_idx;

	home_idx = ft_getenv("HOME", envp);
	if (home_idx < 0)
	{
		write(STDERR_FILENO, "minishell: cd: HOME not set\n", 28);
		return (EXIT_FAILURE);
	}
	return (chdir((*envp)[home_idx] + 5));
}

void	update_pwd()

int	cd(int argc, char **argv, char ***envp)
{
	int	res;

	if (argc > 2)
	{
		write(STDERR_FILENO, "minishell: cd: too many arguments\n", 34);
		return (EXIT_FAILURE);
	}
	if (argc == 1)
		res = move_home(envp);
	else
		res = chdir(argv[1]);
	if (res != EXIT_SUCCESS)
	{
		perror("minishell");
		return (EXIT_FAILURE);
	}

}
