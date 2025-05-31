#include "minishell.h"

// ヒアドキュメントの一時ファイルをすべて削除
void	unlink_all_temp(int redir_count, t_redir **redirs)
{
	int	i;

	i = 0;
	while (i < redir_count)
	{
		if (redirs[i]->temp_file)
		{
			unlink(redirs[i]->temp_file);
			free(redirs[i]->temp_file);
			redirs[i]->temp_file = NULL;
		}
		i++;
	}
}

static void	unlink_last_temp(t_node *last, int end_index)
{
	int	i;

	i = 0;
	while (i < end_index)
	{
		if (last->redirs[i] && last->redirs[i]->temp_file)
		{
			unlink(last->redirs[i]->temp_file);
			free(last->redirs[i]->temp_file);
			last->redirs[i]->temp_file = NULL;
		}
		i++;
	}
}

void	unlink_all_temp_partial(t_node *start, t_node *end, int end_index)
{
	t_node	*cur;
	int		i;

	cur = start;
	while (cur && cur != end)
	{
		i = 0;
		while (i < cur->redir_count)
		{
			if (cur->redirs[i] && cur->redirs[i]->temp_file)
			{
				unlink(cur->redirs[i]->temp_file);
				free(cur->redirs[i]->temp_file);
				cur->redirs[i]->temp_file = NULL;
			}
			i++;
		}
		cur = cur->next_cmd;
	}
	if (cur == end)
		unlink_last_temp(cur, end_index);
}
