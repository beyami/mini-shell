#include "minishell.h"

// ビルトインコマンドを実行
// 関数の戻り値をexitステータスにする
// TODO: marge後コメントアウトを外す
int	exec_builtin_cmd(t_node *node, char **envp)
{
	// char	*cmd;
	(void)node;
	(void)envp;
	// cmd = node->argv[0];
	// if (ft_strncmp(cmd, "echo", 5) == 0)
	// 	sh_stat(ST_SET, echo(node->argc, node->argv));
	// else if (ft_strncmp(cmd, "cd", 3) == 0)
	// 	sh_stat(ST_SET, cd(node->argc, node->argv));
	// else if (ft_strncmp(cmd, "pwd", 4) == 0)
	// 	sh_stat(ST_SET, pwd(node->argc, node->argv));
	// else if (ft_strncmp(cmd, "export", 7) == 0)
	// 	sh_stat(ST_SET, export(node->argc, node->argv));
	// else if (ft_strncmp(cmd, "unset", 6) == 0)
	// 	sh_stat(ST_SET, unset(node->argc, node->argv));
	// else if (ft_strncmp(cmd, "env", 4) == 0)
	// 	sh_stat(ST_SET, env(node->argc, node->argv));
	// else if (ft_strncmp(cmd, "exit", 5) == 0)
	// 	sh_stat(ST_SET, exit_builtin(node->argc, node->argv));
	return (sh_stat(ST_GET, 0));
}

// 標準入出力をデフォルト状態に戻す
int	restore_std_fds(int stashed_stdin, int stashed_stdout)
{
	if (dup2(stashed_stdin, STDIN_FILENO) == -1)
	{
		perror("minishell");
		return (1);
	}
	if (dup2(stashed_stdout, STDOUT_FILENO) == -1)
	{
		perror("minishell");
		return (1);
	}
	close(stashed_stdin);
	close(stashed_stdout);
	return (0);
}

// 標準入出力を退避（複製）→リダイレクト適用→ビルトインコマンドを実行→標準入出力を戻す
void	process_builtin_direct(t_node *node, char **envp)
{
	int	stashed_stdin;
	int	stashed_stdout;

	stashed_stdin = dup(STDIN_FILENO);
	stashed_stdout = dup(STDOUT_FILENO);
	if (stashed_stdin == -1 || stashed_stdout == -1)
	{
		perror("minishell");
		sh_stat(ST_SET, 1);
		return ;
	}
	if (apply_redirs(node) == 0)
		exec_builtin_cmd(node, envp);
	else
		sh_stat(ST_SET, 1);
	if (restore_std_fds(stashed_stdin, stashed_stdout) != 0)
	{
		sh_stat(ST_SET, 1);
		return ;
	}
}
