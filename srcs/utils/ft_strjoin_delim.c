/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_delim.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 23:52:14 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/31 23:52:41 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_delim(char *s1, char delim, char *s2)
{
	char	*rtn;
	int		len1;
	int		len2;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	rtn = (char *)ft_malloc(sizeof(char) * (len1 + len2 + 2));
	if (!rtn)
		return (NULL);
	ft_strlcpy(rtn, s1, len1 + 1);
	rtn[len1] = delim;
	rtn[len1 + 1] = '\0';
	ft_strlcat(rtn, s2, len2 + len1 + 2);
	return (rtn);
}
