#include "minishell.h"

static int	process_expand_cmds(t_exp_tkn **head, char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		// 変数展開とクォート除去
		if (tokenize_with_expansion(head, argv[i], 1) != 0)
		{
			free_exp_tokens(*head);
			return (1);
		}
		i++;
	}
	return (0);
}

// コマンドのargvの展開
int	expand_cmds(t_node *node)
{
	t_exp_tkn	*head;

	head = NULL;
	if (process_expand_cmds(&head, node->argv) != 0)
		return (1);
	// 変数展開の結果を単語分割
	if (split_exp_tokens(&head) != 0)
	{
		free_exp_tokens(head);
		return (1);
	}
	if (update_args_from_exp(head, node) != 0)
		return (1);
	return (0);
}
