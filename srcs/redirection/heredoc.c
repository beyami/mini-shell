#include "minishell.h"

// ヒアドキュメントの一時ファイルを生成する
int	create_temp_file(int index, char **temp_file)
{
	char	*index_str;
	size_t	path_len;

	index_str = ft_itoa(index);
	if (!index_str)
		return (1);
	path_len = ft_strlen(HEREDOC_TMP) + ft_strlen(index_str) + 1;
	if (path_len >= PATH_MAX)
	{
		free(index_str);
		return (1);
	}
	*temp_file = ft_strjoin(HEREDOC_TMP, index_str);
	free(index_str);
	if (!(*temp_file))
		return (1);
	return (0);
}

// ヒアドキュメントの入力を一時ファイルに書き込む
int	write_heredoc_input(char *temp_file, t_redir *redir)
{
	int		temp_fd;
	char	*line;

	temp_fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (temp_fd < 0)
	{
		ft_dprintf(STDERR_FILENO, "minishell: open: %s\n", strerror(errno));
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (g_sig_received)
		{
			break;
		}
		if (!line)
		{
			ft_dprintf(STDERR_FILENO,
				"minishell: warning: here-document delimited by end-of-file (wanted `%s')\n",
				redir->str);
			break ;
		}
		if (ft_strncmp(line, redir->str, ft_strlen(redir->str)) == 0
			&& ft_strlen(line) == ft_strlen(redir->str))
		{
			free(line);
			break ;
		}
		write(temp_fd, line, ft_strlen(line));
		write(temp_fd, "\n", 1);
		free(line);
	}
	close(temp_fd);
	return (0);
}

// すべてのヒアドキュメントの入力を読み取って一時ファイルを作成して書き込む
int	handle_all_heredocs(t_node *node)
{
	int	i;
	int	n;

	n = 0;
	while (node)
	{
		i = 0;
		while (i < node->redir_count)
		{
			if (node->redirs[i]->kind == REDIR_HEREDOC)
			{
				create_temp_file(n, &node->redirs[i]->temp_file);
				if (!node->redirs[i]->temp_file)
					return (1);
				if (write_heredoc_input(node->redirs[i]->temp_file,
						node->redirs[i]) != 0)
					return (1);
			}
			i++;
			n++;
		}
		node = node->next_cmd;
	}
	return (0);
}
