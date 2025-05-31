/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils_delete.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 23:49:03 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/31 23:50:28 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_del(t_env **head, char *name)
{
	t_env	*tmp;
	t_env	*tmp_before;
	int		name_len;

	tmp = *head;
	tmp_before = *head;
	name_len = ft_strlen(name);
	while (tmp)
	{
		if (ft_strncmp(name, tmp->name, name_len) == 0)
			break ;
		tmp_before = tmp;
		tmp = tmp->next;
	}
	if (!tmp)
		return (NULL);
	if (tmp == *head)
		*head = tmp->next;
	else
		tmp_before->next = tmp->next;
	free_env(tmp);
	return (*head);
}

t_env	*env_del_all(t_env **head)
{
	t_env	*tmp;

	while (*head)
	{
		tmp = (*head)->next;
		free_env(*head);
		*head = tmp;
	}
	ft_cwd(PWD_SET, NULL);
	return (NULL);
}
