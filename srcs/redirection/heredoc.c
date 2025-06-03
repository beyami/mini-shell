#include "minishell.h"

static int	process_input_line(char *line, int temp_fd, t_redir *redir)
{
	if (ft_strncmp(line, redir->str, ft_strlen(redir->str)) == 0
		&& ft_strlen(line) == ft_strlen(redir->str))
	{
		free(line);
		return (1);
	}
	if (expand_heredoc_line(&line) != 0)
	{
		free(line);
		return (-1);
	}
	write(temp_fd, line, ft_strlen(line));
	write(temp_fd, "\n", 1);
	free(line);
	return (0);
}

static int	write_heredoc_input(char *line, int temp_fd, t_redir *redir)
{
	int	ret;

	ret = 0;
	while (1)
	{
		line = readline("> ");
		if (g_sig_received)
		{
			if (line)
				free(line);
			break ;
		}
		if (!line)
		{
			ft_dprintf(STDERR_FILENO, HEREDOC_EOF_MSG, redir->str);
			break ;
		}
		ret = process_input_line(line, temp_fd, redir);
		if (ret == 1)
			break ;
		if (ret == -1)
			return (1);
	}
	return (0);
}

static int	handle_single_heredoc(t_node *cur, int i, int n)
{
	char	*line;
	int		temp_fd;

	line = NULL;
	if (create_temp_file(n, &cur->redirs[i]->temp_file) != 0)
		return (1);
	temp_fd = open(cur->redirs[i]->temp_file, O_WRONLY | O_CREAT | O_TRUNC,
			0644);
	if (temp_fd < 0)
	{
		ft_dprintf(STDERR_FILENO, "minishell: open: %s\n", strerror(errno));
		return (1);
	}
	if (write_heredoc_input(line, temp_fd, cur->redirs[i]) != 0)
	{
		close(temp_fd);
		return (1);
	}
	close(temp_fd);
	return (0);
}

// すべてのヒアドキュメントの入力を読み取って一時ファイルを作成して書き込む
int	handle_all_heredocs(t_node *node)
{
	int		i;
	int		n;
	t_node	*cur;

	n = 0;
	cur = node;
	while (cur)
	{
		i = 0;
		while (i < cur->redir_count)
		{
			if (cur->redirs[i]->kind == REDIR_HEREDOC)
			{
				if (handle_single_heredoc(cur, i, n) != 0)
				{
					unlink_all_temp(node);
					return (1);
				}
			}
			i++;
			n++;
		}
		cur = cur->next_cmd;
	}
	return (0);
}
