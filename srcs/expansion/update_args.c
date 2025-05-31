#include "minishell.h"

// コマンドの数を数える
static int	count_exp_tokens(t_exp_tkn *head)
{
	int			count;
	t_exp_tkn	*cur;

	count = 0;
	cur = head;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	return (count);
}

// リストから新しい二次元配列に文字列をコピー
static int	copy_exp_to_array(char **argv, t_exp_tkn *head)
{
	t_exp_tkn	*cur;
	int			i;

	cur = head;
	i = 0;
	while (cur)
	{
		argv[i] = ft_strdup(cur->str);
		if (!argv[i])
		{
			free_2d_array(argv);
			return (1);
		}
		i++;
		cur = cur->next;
	}
	argv[i] = NULL;
	return (0);
}

// リストから新しい二次元配列を構築
static char	**build_array_from_exp(t_exp_tkn *head, int count)
{
	char	**argv;

	argv = (char **)ft_malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	if (copy_exp_to_array(argv, head) != 0)
		return (NULL);
	return (argv);
}

int	update_args_from_exp(t_exp_tkn *head, t_node *node)
{
	node->argc = count_exp_tokens(head);
	node->argv_lst = head;
	free_2d_array(node->argv);
	if (node->argc == 0)
	{
		node->argv = NULL;
		return (0);
	}
	node->argv = build_array_from_exp(node->argv_lst, node->argc);
	if (!node->argv)
		return (1);
	return (0);
}
