/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 22:38:00 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/31 21:34:17 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*rm_quotes(char *val)
{
	char	quote;
	int		i;
	int		j;
	char	*rtn;

	i = 0;
	j = 0;
	quote = '\0';
	if (!val)
		return (NULL);
	while (val[i])
	{
		if ((quote && val[i] != quote) || !is_quote(&val[i]))
			++j;
		else if (!quote)
			quote = val[i];
		else if (quote)
			quote = '\0';
		++i;
	}
	rtn = (char *)ft_malloc(sizeof(char) * j + 1);
	if (!rtn)
	{
		free(val);
		return (NULL);
	}
	i = 0;
	j = 0;
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
	free(val);
	return (rtn);
}

/*
新しいenv構造体をつくる
str はAAA+=XXXを許容する
+,=以降はなくても構わない
=がない場合VAL_SHとして、ある場合VAL_EXとして構造体が作成される
offsetは+が存在する場合のindexのずらす値である
*/
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
	if ((eq && !rtn->val) || !rtn->name || !rtn->full)
	{
		free_env(rtn);
		return (NULL);
	}
	return (rtn);
}

// 新しいt_envを返す
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

t_env	*search_val(t_env *head, char *name, int offset, t_val_type type)
{
	t_env	*tmp;
	int		name_len;
	char	*eq;

	eq = ft_strchr(name, '=');
	if (eq)
		name_len = (int)(eq - name - offset);
	else
		name_len = ft_strlen(name);
	tmp = head;
	while (tmp)
	{
		if (((type == VAL_EX && tmp->type == VAL_EX) || type == VAL_SH)
			&& ft_strncmp(tmp->name, name, name_len) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

// 環境変数(VAL_EX)であるt_envのうち、nameに一致するものを返す
void	*get_env(t_env *head, char *name, t_env_op op)
{
	t_env	*target;

	target = search_val(head, name, 0, VAL_EX);
	if (!target)
		return (NULL);
	if (op == ENV_GET_VAL)
		return (target->val);
	return (target);
}

int	list_len(t_env *head, t_env_op op)
{
	t_env	*tmp;
	int		len;

	len = 0;
	tmp = head;
	while (tmp)
	{
		if (op == ENV_GET_ALL_SH || (op == ENV_GET_ALL_EX
				&& tmp->type == VAL_EX))
			++len;
		tmp = tmp->next;
	}
	return (len);
}

//変数全て or VAL_EXのみをまとめて返す
void	*get_env_all(t_env **head, t_env_op op)
{
	int		len;
	char	**rtn;
	t_env	*tmp;
	int		i;

	len = list_len(*head, op);
	rtn = (char **)ft_malloc(sizeof(char *) * (len + 1));
	if (!rtn)
	{
		return (NULL);
	}
	tmp = *head;
	i = 0;
	while (tmp)
	{
		if (op == ENV_GET_ALL_SH || (op == ENV_GET_ALL_EX
				&& tmp->type == VAL_EX))
			rtn[i++] = tmp->full;
		tmp = tmp->next;
	}
	rtn[i] = NULL;
	return (rtn);
}

/*
既存の変数を更新する。
存在しない場合新規作成する。
offsetはname+=valの場合にindexをずらすための値である。
*/
t_env	*ft_update_env(t_env **head, char *str, int offset)
{
	t_env	*target;
	char	*val;
	char	*tmp;

	target = search_val(*head, str, offset, VAL_SH);
	if (!target)
		return (ft_add_env(head, str, offset));
	val = ft_strchr(str, '=') + 1;
	if (offset && target->type == VAL_EX)
		tmp = rm_quotes(ft_strjoin(target->val, val));
	else
	{
		target->type = VAL_EX;
		tmp = rm_quotes(ft_strdup(val));
	}
	if (!tmp)
		return (NULL);
	free(target->val);
	target->val = tmp;
	tmp = ft_strjoin_delim(target->name, '=', target->val);
	if (!tmp)
		return (NULL);
	free(target->full);
	target->full = tmp;
	return (*head);
}

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

void	*ft_env(t_env_op op, char *str)
{
	static t_env	*head;

	if (op == ENV_GET_VAL || op == ENV_GET_STRUCT)
		return (get_env(head, str, op));
	else if (op == ENV_GET_ALL_EX || op == ENV_GET_ALL_SH)
		return (get_env_all(&head, op));
	else if (op == ENV_ADD)
		return (ft_add_env(&head, str, 0));
	else if (op == ENV_SET)
		return (ft_update_env(&head, str, 0));
	else if (op == ENV_SET_PLUS)
		return (ft_update_env(&head, str, 1));
	else if (op == ENV_DEL)
		return (env_del(&head, str));
	else if (op == ENV_DEL_ALL)
		return (env_del_all(&head));
	return (NULL);
}
