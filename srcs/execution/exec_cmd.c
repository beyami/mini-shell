#include "minishell.h"

// 相対パスなら実行
void	exec_if_relative_path(char **cmds, char **envp)
{
	if (ft_strncmp(cmds[0], "./", 2) == 0 || ft_strncmp(cmds[0], "../", 3) == 0)
	{
		if (access(cmds[0], F_OK) == 0)
		{
			if (access(cmds[0], X_OK) == 0)
			{
				execve(cmds[0], cmds, envp);
				ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmds[0],
					strerror(errno));
				free_2d_array(cmds);
				exit(1);
			}
		}
		ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmds[0],
			strerror(errno));
		free_2d_array(cmds);
		exit(EXIT_FAILURE);
	}
}

// 絶対パスなら実行
void	exec_if_absolute_path(char **cmds, char **envp)
{
	if (access(cmds[0], F_OK) == 0)
	{
		if (access(cmds[0], X_OK) == 0)
		{
			execve(cmds[0], cmds, envp);
			ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmds[0],
				strerror(errno));
			free_2d_array(cmds);
			exit(EXIT_FAILURE);
		}
	}
	ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmds[0], strerror(errno));
	free_2d_array(cmds);
	exit(EXIT_FAILURE);
}

// コマンド実行
void	exec_cmd(char **cmds, char **envp)
{
	char	*path_env;
	char	*cmd_path;
	int		status;

	status = 1;
	exec_if_relative_path(cmds, envp);
	path_env = ft_getenv("PATH", envp);
	if (!path_env || ft_strncmp(cmds[0], "/", 1) == 0)
		exec_if_absolute_path(cmds, envp);
	cmd_path = resolve_cmd_path(cmds[0], path_env, &status);
	if (!cmd_path)
	{
		free_2d_array(cmds);
		exit(status);
	}
	execve(cmd_path, cmds, envp);
	ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmds[0], strerror(errno));
	free_2d_array(cmds);
	exit(EXIT_FAILURE);
}

void	handle_std_redirs(int in_fd, int out_fd)
{
	if (in_fd != STDIN_FILENO && in_fd != -1)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
		{
			perror("minishell");
			exit(EXIT_FAILURE);
		}
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO && out_fd != -1)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
		{
			perror("minishell");
			exit(EXIT_FAILURE);
		}
		close(out_fd);
	}
}

void	process_exec_cmd(t_node *node, char **envp, int in_fd, int out_fd)
{
	handle_std_redirs(in_fd, out_fd);
	if (apply_redirs(node) != 0)
		exit(EXIT_FAILURE);
	if (is_builtin(node->argv[0]))
	{
		exec_builtin_cmd(node, envp);
		exit(sh_stat(ST_GET, 0));
	}
	else
		exec_cmd(node->argv, envp);
}
