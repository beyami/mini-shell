/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 01:29:37 by yumiyao           #+#    #+#             */
/*   Updated: 2025/04/29 17:18:21 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_cwd(void)
{
	char	*rtn;

	rtn = (char *)malloc(sizeof(char) * PATH_MAX);
	if (!rtn)
	{
		perror("minishell");
		return (NULL);
	}
	if (!getcwd(rtn, PATH_MAX))
	{
		perror("minishell");
		free(rtn);
		return (NULL);
	}
	return (rtn);
}

char	*get_pwd(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (strncmp(envp[i], "PWD=", 4) == 0)
			return (envp[i]);
		++i;
	}
	return (get_cwd());
}

int	is_pwd_error(int argc, char **argv)
{
	if (argc == 1)
		return (0);
	if (argv[1][0] != '-' || strncmp(argv[1], "--", 3) == 0)
		return (0);
	if (argv[1][1] == '\0')
		return (0);
	return (1);
}

int	pwd(int argc, char **argv, char **envp)
{
	char	*bufs;

	bufs = NULL;
	if (is_pwd_error(argc, argv))
	{
		write(STDERR_FILENO, "minishell: pwd: ", 16);
		write(STDERR_FILENO, argv[1], 2);
		write(STDERR_FILENO, ": invalid option\n", 17);
		return (EXIT_FAILURE);
	}
	bufs = get_pwd(envp);
	if (!bufs)
	{
		perror("minishell");
		return (EXIT_FAILURE);
	}
	printf("%s\n", bufs + 4);
	return (EXIT_SUCCESS);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	pwd(argc, argv, envp);
// }
