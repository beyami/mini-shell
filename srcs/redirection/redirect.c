#include "minishell.h"

// ヒアドキュメントの適用
int	apply_heredoc_redir(t_redir *redir)
{
	int	heredoc_fd;

	heredoc_fd = open(redir->temp_file, O_RDONLY);
	if (heredoc_fd < 0)
	{
		ft_dprintf(STDERR_FILENO, "minishell: open: %s\n", strerror(errno));
		return (1);
	}
	if (dup2(heredoc_fd, STDIN_FILENO) == -1)
	{
		close(heredoc_fd);
		ft_dprintf(STDERR_FILENO, "minishell: dup2: %s\n", strerror(errno));
		return (1);
	}
	close(heredoc_fd);
	return (0);
}

// 入出力（ヒアドキュメント以外）のリダイレクト適用
int	apply_io_redir(t_redir *redir, int flags, int std_fd)
{
	int	fd;

	fd = open(redir->str, flags, 0644);
	if (fd < 0)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n",
			redir->str, strerror(errno));
		return (1);
	}
	if (dup2(fd, std_fd) == -1)
	{
		close(fd);
		ft_dprintf(STDERR_FILENO, "minishell: dup2: %s\n", strerror(errno));
		return (1);
	}
	close(fd);
	return (0);
}

// 適用するリダイレクトによって分岐
int	process_redir(t_redir *redir)
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
		return (apply_heredoc_redir(redir));
}

// リダイレクトの適用メイン処理
int	apply_redirs(int redir_count, t_redir **redirs)
{
	int	i;

	i = 0;
	while (i < redir_count)
	{
		if (process_redir(redirs[i]) != 0)
		{
			unlink_all_temp(redir_count, redirs);
			return (1);
		}
		i++;
	}
	unlink_all_temp(redir_count, redirs);
	return (0);
}
