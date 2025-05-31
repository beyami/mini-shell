#include "minishell.h"

// pipe()でパイプを作る
int	prepare_pipe(t_node *node)
{
	if (node->next_cmd)
	{
		if (pipe(node->pipefd) < 0)
		{
			ft_dprintf(STDERR_FILENO, "minishell: "
				"pipe: %s\n", strerror(errno));
			return (-1);
		}
	}
	return (0);
}

// 子プロセスのファイルディスクリプタ処理
void	prepare_pipe_child(t_node *node, int count)
{
	// 最初のコマンド以外：前のパイプの読み取り側をSTDINに
	if (count > 0)
	{
		if (dup2(node->in_fd, STDIN_FILENO) == -1)
		{
			ft_dprintf(STDERR_FILENO, "minishell: dup2: %s\n", strerror(errno));
			exit(1);
		}
		close(node->in_fd);
	}
	// 最後のコマンド以外：パイプの書き込み側をSTDOUTに & パイプの読み取り側（使わない）を閉じる
	if (node->next_cmd)
	{
		if (dup2(node->pipefd[1], STDOUT_FILENO) == -1)
		{
			ft_dprintf(STDERR_FILENO, "minishell: dup2: %s\n", strerror(errno));
			exit(1);
		}
		close(node->pipefd[1]);
		close(node->pipefd[0]);
	}
}

// 親プロセスのファイルディスクリプタ処理
void	prepare_pipe_parent(t_node *node, int count)
{
	// 最初のコマンド以外：前のパイプの読み取り側を閉じる
	if (count > 0)
		close(node->in_fd);
	// 最後のコマンド以外：パイプの書き込み側（使わない）を閉じる & パイプの読み取り側を次のコマンドの読み取りに
	if (node->next_cmd)
	{
		close(node->pipefd[1]);
		node->next_cmd->in_fd = node->pipefd[0];
	}
}
