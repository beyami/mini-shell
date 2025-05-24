#include "minishell.h"

int	is_empty_cmds(char **argv)
{
	int	i;

	if (!argv || !argv[0])
		return (1);
	i = 0;
	while (argv[i])
	{
		if (argv[i][0] != '\0')
			return (0);
		i++;
	}
	return (1);
}

// 子プロセス内での実行
void	child_exec(t_node *node)
{
	char	**cp_env;

	cp_env = (char **)ft_env(ENV_GET_ALL_EX, NULL);
	if (!cp_env)
		exit(1);
	if (apply_redirs(node->redir_count, node->redirs) != 0)
		exit(1);
	if (is_empty_cmds(node->argv))
		exit(0);
	if (is_builtin(node->argv[0]))
	{
		exec_builtin_cmd(node);
		exit(sh_stat(ST_GET, 0));
	}
	else
		exec_cmd(node->argv, cp_env);
}

// パイプラインの実行
pid_t	run_pipeline(t_node *node, int count)
{
	pid_t	pid;

	if (!node)
		return (-1);
	prepare_pipe(node);
	pid = fork();
	if (pid < 0)
	{
		perror("minishell");
		return (-1);
	}
	if (pid == 0)
	{
		prepare_pipe_child(node, count);
		child_exec(node);
	}
	prepare_pipe_parent(node, count);
	if (node->next_cmd)
		return (run_pipeline(node->next_cmd, count + 1));
	return (pid);
}

// 実行部メイン処理
// ASTのコマンドノードをリストとしてつなげる -> ヒアドキュメントの入力を処理 -> ビルトイン単体かその他かで分岐して実行
void	execute(t_node *root)
{
	pid_t	last_pid;
	t_node	*first_cmd;
	t_node	*last_cmd;

	if (!root)
		return ;
	first_cmd = NULL;
	last_cmd = NULL;
	link_exec_nodes(root, root->rhs, &first_cmd, &last_cmd);
	if (handle_all_heredocs(first_cmd) != 0)
	{
		unlink_all_temp(first_cmd->redir_count, first_cmd->redirs);
		sh_stat(ST_SET, 1);
		return ;
	}
	// ビルトインコマンド単体の場合
	if (root->kind == ND_CMD && root->argv && is_builtin(root->argv[0]))
	{
		process_builtin_direct(root);
		return ;
	}
	last_pid = run_pipeline(first_cmd, 0);
	sh_stat(ST_SET, wait_children(last_pid));
}
