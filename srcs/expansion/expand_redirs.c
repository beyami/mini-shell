#include "minishell.h"

static int	split_expanded_redir_env(t_exp_tkn **head, char *str)
{
	int			count;
	t_exp_tkn	*cur;

	// 変数展開の結果を単語分割
	if (split_exp_tokens(head) != 0)
	{
		free_exp_tokens(*head);
		return (1);
	}
	// 変数展開→単語分割後の文字列の数を数える
	count = 0;
	cur = *head;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	// 文字列が複数ならambiguous redirectエラー
	if (count > 1)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: ambiguous redirect\n", str);
		free_exp_tokens(*head);
		return (1);
	}
	return (0);
}

static int	process_expand_redirs(t_exp_tkn **new, t_redir *redir, int env_flag)
{
	if (tokenize_with_expansion(new, redir->str, env_flag) != 0)
	{
		free_exp_tokens(*new);
		return (1);
	}
	if (env_flag)
		if (split_expanded_redir_env(new, redir->str) != 0)
			return (1);
	free(redir->str);
	if (!*new)
	{
		redir->str = ft_strdup("");
		if (!redir->str)
			return (1);
	}
	else
	{
		redir->str = (*new)->str;
		free(*new);
	}
	return (0);
}

// リダイレクトのファイル名またはデリミタの展開
int	expand_redirs(t_node *node)
{
	int			i;
	t_exp_tkn	*new;
	int			env_flag;

	i = 0;
	while (node->redirs[i])
	{
		new = NULL;
		env_flag = 0;
		// heredocでなければ$展開を行う
		if (node->redirs[i]->kind != REDIR_HEREDOC)
			env_flag = 1;
		if (process_expand_redirs(&new, node->redirs[i], env_flag) != 0)
			return (1);
		i++;
	}
	return (0);
}
