/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils_new.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 23:58:50 by yumiyao           #+#    #+#             */
/*   Updated: 2025/06/01 03:31:28 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*check_new_env(char *eq, t_env *new)
{
	if ((eq && !new->val) || !new->name || !new->full)
	{
		free_env(new);
		return (NULL);
	}
	return (new);
}

char	*cp_without_quoutes(char *val, int len)
{
	char	*rtn;
	char	quote;
	int		i;
	int		j;

	rtn = (char *)ft_malloc(sizeof(char) * len + 1);
	if (!rtn)
		return (NULL);
	i = 0;
	j = 0;
	quote = '\0';
	while (val[i])
	{
		if ((quote && val[i] != quote) || !is_quote(&val[i]))
			rtn[j++] = val[i];
		else if (!quote)
			quote = val[i];
		else if (quote)
			quote = '\0';
		++i;
	}
	rtn[j] = '\0';
	return (rtn);
}

char	*rm_quotes(char *val)
{
	char	quote;
	int		i;
	int		len;
	char	*rtn;

	i = 0;
	len = 0;
	quote = '\0';
	if (!val)
		return (NULL);
	while (val[i])
	{
		if ((quote && val[i] != quote) || !is_quote(&val[i]))
			++len;
		else if (!quote)
			quote = val[i];
		else if (quote)
			quote = '\0';
		++i;
	}
	rtn = cp_without_quoutes(val, len);
	free(val);
	return (rtn);
}

t_env	*new_env(char *str, int offset)
{
	t_env	*rtn;
	char	*eq;

	rtn = (t_env *)ft_malloc(sizeof(t_env));
	if (!rtn)
		return (NULL);
	eq = ft_strchr(str, '=');
	if (!eq)
	{
		rtn->type = VAL_SH;
		rtn->name = ft_strdup(str);
		rtn->val = NULL;
		rtn->full = ft_strdup(rtn->name);
	}
	else
	{
		rtn->type = VAL_EX;
		rtn->name = ft_strndup(str, (int)(eq - str - offset));
		rtn->val = rm_quotes(ft_strdup(eq + 1));
		rtn->full = ft_strjoin_delim(rtn->name, '=', rtn->val);
	}
	rtn->next = NULL;
	return (check_new_env(eq, rtn));
}

t_env	*ft_add_env(t_env **head, char *str, int offset)
{
	t_env	*tmp;

	if (!(*head))
	{
		*head = new_env(str, offset);
		return (*head);
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_env(str, offset);
	if (!tmp->next)
	{
		env_del_all(head);
		return (NULL);
	}
	return (*head);
}
