#include "minishell.h"

static int	process_double_quote(t_exp_tkn **head, char **p, int env_flag)
{
	int			len;
	t_exp_tkn	*new_tkn;

	while (**p && **p != '"')
	{
		if (**p == '$' && env_flag)
			new_tkn = expand_env_var(p, true);
		else
		{
			len = 0;
			while ((*p)[len] && (*p)[len] != '"')
			{
				if ((*p)[len] == '$' && env_flag)
					break ;
				len++;
			}
			new_tkn = extract_literal(p, len, true);
		}
		if (!new_tkn)
			return (1);
		append_exp_token(head, new_tkn);
	}
	return (0);
}

// ダブルクォート内の処理
static int	handle_double_quote(t_exp_tkn **head, char **p, int env_flag)
{
	t_exp_tkn	*new_tkn;

	if (**p == '"')
	{
		new_tkn = extract_literal(p, 0, true);
		if (!new_tkn)
			return (1);
		append_exp_token(head, new_tkn);
		return (0);
	}
	if (process_double_quote(head, p, env_flag) != 0)
		return (1);
	return (0);
}

// シングルクォート内の処理
static int	handle_single_quote(t_exp_tkn **head, char **p)
{
	int			len;
	t_exp_tkn	*new_tkn;

	if (**p == '\'')
		new_tkn = extract_literal(p, 0, true);
	else
	{
		len = 0;
		while ((*p)[len] && (*p)[len] != '\'')
			len++;
		new_tkn = extract_literal(p, len, true);
	}
	if (!new_tkn)
		return (1);
	append_exp_token(head, new_tkn);
	return (0);
}

// クォートなし部分の処理
static int	handle_no_quote(t_exp_tkn **head, char **p, int env_flag)
{
	int			len;
	t_exp_tkn	*new_tkn;

	if (**p == '$' && env_flag)
		new_tkn = expand_env_var(p, false);
	else
	{
		len = 0;
		while ((*p)[len] && (!env_flag || (*p)[len] != '$'))
			len++;
		new_tkn = extract_literal(p, len, false);
	}
	if (!new_tkn)
		return (1);
	append_exp_token(head, new_tkn);
	return (0);
}

// 変数展開とクォート除去
// env_flagは$展開を行うかどうかのフラグ（heredocのデリミタなら$展開を行わない）
int	tokenize_with_expansion(t_exp_tkn **head, char *str, int env_flag)
{
	char	*p;
	int		res;

	p = str;
	while (*p)
	{
		if (*p == '"')
		{
			p++;
			res = handle_double_quote(head, &p, env_flag);
			p++;
		}
		else if (*p == '\'')
		{
			p++;
			res = handle_single_quote(head, &p);
			p++;
		}
		else
			res = handle_no_quote(head, &p, env_flag);
		if (res != 0)
			return (1);
	}
	return (0);
}
