/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 00:47:01 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/31 21:38:38 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	get_env_option(int argc, char **argv)
{
	if (argc == 1)
		return (0);
	if (ft_strncmp("--", argv[1], 2) == 0)
	{
		if (argv[1][2] == '\0')
		{
			if (argc == 2)
				return (0);
			return (1);
		}
		return (2);
	}
	if (argv[1][0] == '-')
	{
		if (argv[1][1] == '\0')
		{
			if (argc == 2)
				return (3);
			return (1);
		}
		return (4);
	}
	return (5);
}

void	print_env_err(int option_type, char **argv)
{
	ft_dprintf(STDERR_FILENO, "env: ");
	if (option_type == 1)
		ft_dprintf(STDERR_FILENO, "too many arguments\n");
	else if (option_type == 2)
		ft_dprintf(STDERR_FILENO, "unrecognized option '%s'\n", argv[1]);
	else if (option_type == 4)
		ft_dprintf(STDERR_FILENO, "invalid option -- '%c'\n", argv[1][1]);
}

int	env(int argc, char **argv)
{
	int		i;
	int		option_type;
	char	**envp;

	i = 0;
	option_type = get_env_option(argc, argv);
	if (option_type == 3)
		return (EXIT_SUCCESS);
	else if (option_type > 1)
	{
		print_env_err(option_type, argv);
		return (125);
	}
	else
	{
		envp = (char **)ft_env(ENV_GET_ALL_SH, NULL);
		if (!envp)
			inner_exit(125);
		while (envp[i])
			ft_dprintf(STDOUT_FILENO, "%s\n", envp[i++]);
	}
	return (EXIT_SUCCESS);
}
