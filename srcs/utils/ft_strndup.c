#include "minishell.h"

// 文字数指定ft_strdup
char	*ft_strndup(char *s, int len)
{
	char	*new;
	int		i;

	if (!s || len <= 0)
		return (NULL);
	new = (char *)malloc(sizeof(char) * (len + 1));
	if (!new)
	{
		perror("minishell");
		return (NULL);
	}
	i = 0;
	while (s[i] && i < len)
	{
		new[i] = s[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}
