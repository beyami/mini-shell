#include "minishell.h"

// ヒアドキュメントの一時ファイルを生成する
int	create_temp_file(int index, char **temp_file)
{
	char	*index_str;

	index_str = ft_itoa(index);
	if (!index_str)
		return (-1);
	*temp_file = ft_strjoin(HEREDOC_TMP, index_str);
	if (!(*temp_file))
	{
		free(index_str);
		return (-1);
	}
	free(index_str);
	if (ft_strlen(HEREDOC_TMP) > PATH_MAX)
	{
		free(*temp_file);
		return (-1);
	}
	return (0);
}

// ヒアドキュメントの入力を一時ファイルに書き込む
int	write_heredoc_input(char *temp_file, t_redir *redir)
{
	int		temp_fd;
	char	*line;

	temp_fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (temp_fd < 0)
	{
		perror("minishell");
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, redir->str, ft_strlen(redir->str)) == 0)
		{
			free(line);
			break ;
		}
		write(temp_fd, line, ft_strlen(line));
		write(temp_fd, "\n", 1);
		free(line);
	}
    close(temp_fd);
	return (0);
}

// 一時ファイルを開いて、読み込みFDを返してからファイルを削除する
int	open_and_unlink_temp(char *temp_file)
{
	int	read_fd;

	read_fd = open(temp_file, O_RDONLY);
	if (read_fd < 0)
	{
		unlink(temp_file);
		return (-1);
	}
	unlink(temp_file);
	return (read_fd);
}

// ヒアドキュメントのメイン処理
int	get_heredoc_fd(t_redir *redir, int index)
{
	char	*temp_file;
	int		read_fd;

	if (create_temp_file(index, &temp_file) != 0)
		return (-1);
	if (write_heredoc_input(temp_file, redir) != 0)
    {
        free(temp_file);
        return (-1);
    }
	read_fd = open_and_unlink_temp(temp_file);
	if (read_fd < 0)
	{
		free(temp_file);
		return (-1);
	}
	free(temp_file);
	return (read_fd);
}
