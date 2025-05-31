#include "minishell.h"

// ビルトインコマンドを実行
// 関数の戻り値をexitステータスにする
int	exec_builtin_cmd(t_node *node)
{
	char	*cmd;
	int		stat;

	stat = 0;
	cmd = node->argv[0];
	if (ft_strncmp(cmd, "echo", 5) == 0)
		stat = echo(node->argc, node->argv);
    // stat = echo(node->argc, node->argv_lst); // こちらを使ってください
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		stat = cd(node->argc, node->argv);
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		stat = pwd(node->argc, node->argv);
	else if (ft_strncmp(cmd, "export", 7) == 0)
		stat = export(node->argc, node->argv);
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		stat = unset(node->argc, node->argv);
	else if (ft_strncmp(cmd, "env", 4) == 0)
		stat = env(node->argc, node->argv);
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		stat = ft_exit(node->argc, node->argv);
	return (sh_stat(ST_SET, stat));
}

// 標準入出力をデフォルト状態に戻す
int	restore_std_fds(int stashed_stdin, int stashed_stdout)
{
	if (dup2(stashed_stdin, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		return (1);
	}
	if (dup2(stashed_stdout, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		return (1);
	}
	close(stashed_stdin);
	close(stashed_stdout);
	return (0);
}

// 標準入出力を退避（複製）→リダイレクト適用→ビルトインコマンドを実行→標準入出力を戻す
int	process_builtin_direct(t_node *node)
{
	int	stashed_stdin;
	int	stashed_stdout;

	stashed_stdin = dup(STDIN_FILENO);
	stashed_stdout = dup(STDOUT_FILENO);
	if (stashed_stdin == -1 || stashed_stdout == -1)
	{
		perror("minishell: dup");
		return (1);
	}
	if (apply_redirs(node->redir_count, node->redirs) != 0)
	{
		restore_std_fds(stashed_stdin, stashed_stdout);
		return (1);
	}
	if (exec_builtin_cmd(node) == 1)
		return (1);
	if (restore_std_fds(stashed_stdin, stashed_stdout) != 0)
		return (1);
	return (0);
}
