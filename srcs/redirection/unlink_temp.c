#include "minishell.h"

// ヒアドキュメントの一時ファイルをすべて削除
void	unlink_all_temp(t_node *first)
{
	t_node	*cur;
	int		i;

	if (!first)
		return ;
	cur = first;
	while (cur)
	{
		i = 0;
		while (i < cur->redir_count)
		{
			if (cur->redirs[i]->temp_file)
			{
				unlink(cur->redirs[i]->temp_file);
				free(cur->redirs[i]->temp_file);
				cur->redirs[i]->temp_file = NULL;
			}
			i++;
		}
		cur = cur->next_cmd;
	}
}
