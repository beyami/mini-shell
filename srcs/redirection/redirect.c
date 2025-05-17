#include "minishell.h"

// ヒアドキュメントの適用
int	apply_heredoc_redir(t_redir *redir, int index)
{
	int	cur_heredoc_fd;

	cur_heredoc_fd = get_heredoc_fd(redir, index);
	if (cur_heredoc_fd < 0)
	{
		perror("minishell");
		return (1);
	}
	if (dup2(cur_heredoc_fd, STDIN_FILENO) == -1)
	{
		close(cur_heredoc_fd);
		perror("minishell");
		return (1);
	}
	close(cur_heredoc_fd);
	return (0);
}

// 入出力（ヒアドキュメント以外）のリダイレクト適用
int	apply_io_redir(t_redir *redir, int flags, int std_fd)
{
	int	fd;

	fd = open(redir->str, flags, 0644);
	if (fd < 0)
	{
		perror("minishell");
		return (1);
	}
	if (dup2(fd, std_fd) == -1)
	{
		close(fd);
		perror("minishell");
		return (1);
	}
	close(fd);
	return (0);
}

// 適用するリダイレクトによって分岐
int	process_redir(t_redir *redir, int index)
{
	if (redir->kind == REDIR_IN)
		return (apply_io_redir(redir, O_RDONLY, STDIN_FILENO));
	else if (redir->kind == REDIR_OUT)
		return (apply_io_redir(redir, O_WRONLY | O_CREAT | O_TRUNC,
				STDOUT_FILENO));
	else if (redir->kind == REDIR_APPEND)
		return (apply_io_redir(redir, O_WRONLY | O_CREAT | O_APPEND,
				STDOUT_FILENO));
	else
		return (apply_heredoc_redir(redir, index));
}

// リダイレクトの適用メイン処理
int	apply_redirs(t_node *node)
{
	int	i;

	i = 0;
	while (i < node->redir_count)
	{
		if (process_redir(node->redirs[i], i) != 0)
			return (1);
		i++;
	}
	return (0);
}
