/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 00:20:46 by yumiyao           #+#    #+#             */
/*   Updated: 2025/06/01 03:46:06 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_n_option(char *opt)
{
	int	i;

	if (opt[0] != '-')
		return (0);
	i = 1;
	while (opt[i])
	{
		if (opt[i] != 'n')
			return (0);
		++i;
	}
	return (1);
}

int	check_argv(char **argv, int *endnew)
{
	int	i;
	int	op;

	i = 1;
	op = 1;
	*endnew = 1;
	while (argv[i])
	{
		if (is_n_option(argv[i]))
		{
			op = i + 1;
			*endnew = 0;
		}
		++i;
	}
	return (op);
}

int	echo(int argc, char **argv)
{
	int	endnew;
	int	i;

	if (argc < 1)
		return (EXIT_SUCCESS);
	i = check_argv(argv, &endnew);
	while (i < argc)
	{
		ft_dprintf(STDOUT_FILENO, "%s", argv[i++]);
		if (i < argc)
			ft_dprintf(STDOUT_FILENO, " ");
	}
	if (endnew)
		ft_dprintf(STDOUT_FILENO, "\n");
	return (EXIT_SUCCESS);
}

// int main(int argc, char **argv)
// {
// 	echo(argc, argv);
// }
