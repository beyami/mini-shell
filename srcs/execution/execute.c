#include "minishell.h"

// パイプラインの実行
void	exec_pipe(t_node *node, t_exec *ctx, char **envp, int in_fd, int out_fd)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("minishell");
		close_fds(in_fd, out_fd);
		return ;
	}
	execute_segment(node->lhs, ctx, envp, in_fd, pipefd[1]);
	close(pipefd[1]);
	execute_segment(node->rhs, ctx, envp, pipefd[0], out_fd);
	close(pipefd[0]);
}

// 木構造をたどりながら実行
void	execute_segment(t_node *node, t_exec *ctx, char **envp, int in_fd,
		int out_fd)
{
	pid_t	pid;

	if (!node)
	{
		close_fds(in_fd, out_fd);
		return ;
	}
	if (node->kind == ND_PIPE)
		exec_pipe(node, ctx, envp, in_fd, out_fd);
	else if (node->kind == ND_CMD)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("minishell");
			close_fds(in_fd, out_fd);
			return ;
		}
		if (pid == 0)
			process_exec_cmd(node, envp, in_fd, out_fd);
		ctx->child_pids[ctx->child_count++] = pid;
		close_fds(in_fd, out_fd);
	}
}

// 実行部メイン処理
void	execute(t_node *root, char **envp)
{
	t_exec	ctx;

	if (!root)
		return ;
	ctx.child_count = 0;
    // ビルトインコマンド単体の場合直接実行
	if (root->kind == ND_CMD && is_builtin(root->argv[0]))
	{
		process_builtin_direct(root, envp);
		return ;
	}
	execute_segment(root, &ctx, envp, STDIN_FILENO, STDOUT_FILENO);
	wait_children(ctx);
}
