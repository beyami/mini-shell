#include "minishell.h"

// 単一文字を結果文字列に追加
static char	*append_char(char *result, char c)
{
	char	*temp;

	temp = (char *)ft_malloc(ft_strlen(result) + 2);
	if (!temp)
	{
		free(result);
		return (NULL);
	}
	ft_strlcpy(temp, result, ft_strlen(result) + 1);
	temp[ft_strlen(result)] = c;
	temp[ft_strlen(result) + 1] = '\0';
	free(result);
	return (temp);
}

// ヒアドキュメント用の変数展開関数
int	expand_heredoc_line(char **line)
{
	char	*result;
	char	*s;

	result = ft_strdup("");
	if (!result)
		return (1);
	s = *line;
	while (*s)
	{
		if (*s == '$')
			result = process_dollar(&s, result);
		else
		{
			result = append_char(result, *s);
			s++;
		}
		if (!result)
			return (1);
	}
	free(*line);
	*line = result;
	return (0);
}
