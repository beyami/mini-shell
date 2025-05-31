/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 22:09:22 by stakada           #+#    #+#             */
/*   Updated: 2025/05/29 03:36:31 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_digits(long num)
{
	int	digits;

	digits = 1;
	if (num < 0)
	{
		digits++;
		num = -num;
	}
	while (num >= 10)
	{
		num /= 10;
		digits++;
	}
	return (digits);
}

char	*ft_itoa(int n)
{
	char	*str;
	long	num;
	int		sign;
	int		digits;

	num = (long)n;
	sign = 0;
	if (num < 0)
		sign = 1;
	digits = count_digits(num);
	str = (char *)ft_malloc(sizeof(char) * (digits + 1));
	if (!str)
		return (NULL);
	str[digits] = '\0';
	if (sign)
	{
		str[0] = '-';
		num = -num;
	}
	while (digits-- > sign)
	{
		str[digits] = num % 10 + '0';
		num /= 10;
	}
	return (str);
}
