/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 20:58:51 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/31 20:59:13 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long long int	ft_atol(char *num, long long int minus, int digit)
{
	long long int	rtn;
	int				i;

	rtn = 0;
	i = 0;
	while (i < digit)
	{
		rtn *= 10;
		rtn += num[i] - '0';
		++i;
	}
	rtn *= minus;
	return (rtn);
}