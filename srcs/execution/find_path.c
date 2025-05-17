#include "minishell.h"

// 環境変数PATHの値を一つずつ確認
char	*search_path(char *cmd_name, char **path_list, char *path_tail,
		int *status)
{
	char	*path;
	int		i;

	i = 0;
	while (path_list[i])
	{
		path = ft_strjoin(path_list[i], path_tail);
		if (!path)
		{
			perror("minishell");
			return (NULL);
		}
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	ft_dprintf(STDERR_FILENO, "minishell: %s: command not found\n", cmd_name);
	(*status) = 127;
	return (NULL);
}

// 環境変数PATHからコマンドのパスを探す
char	*resolve_cmd_path(char *cmd, char *path_env, int *status)
{
	char	*path_tail;
	char	**path_list;
	char	*filepath;

	path_tail = ft_strjoin("/", cmd);
	if (!path_tail)
	{
		perror("minishell");
		return (NULL);
	}
	path_list = ft_split(path_env, ':');
	if (!path_list)
	{
		perror("minishell");
		free(path_tail);
		return (NULL);
	}
	filepath = search_path(cmd, path_list, path_tail, status);
	if (!filepath)
	{
		free(path_tail);
		free_2d_array(path_list);
		return (NULL);
	}
	return (filepath);
}
