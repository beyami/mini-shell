/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 00:47:01 by yumiyao           #+#    #+#             */
/*   Updated: 2025/04/18 02:24:38 by yumiyao          ###   ########.fr       */
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
			return (3);
		}
		return (2);
	}
	if (argv[1][0] == '-')
	{
		if (argv[1][1] == '\0')
		{
			if (argc == 2)
				return (1);
			return (3);
		}
		return (4);
	}
	return (2);
}

void	print_env_err(int option_type, char *option)
{
	write(STDERR_FILENO, "env: ", 5);
	if (option_type == 4)
	{
		write(STDERR_FILENO, "invalid option -- '", 19);
		write(STDERR_FILENO, &option[1], 1);
		write(STDERR_FILENO, "'\n", 2);
	}
	else
	{
		write(STDERR_FILENO, "unrecognized option '", 21);
		write(STDERR_FILENO, option, ft_strlen(option));
		write(STDERR_FILENO, "'\n", 2);
	}
}

int	env(int argc, char **argv, char **envp)
{
	int	i;
	int	option_type;

	i = 0;
	option_type = get_env_option(argc, argv);
	if (option_type == 1)
		return (EXIT_SUCCESS);
	else if (option_type > 1)
	{
		if (option_type == 3)
			print_env_err(option_type, argv[2]);
		else
			print_env_err(option_type, argv[1]);
		return (EXIT_FAILURE);
	}
	else
	{
		while (envp[i])
			printf("%s\n", envp[i++]);
	}
	return (EXIT_SUCCESS);
}
