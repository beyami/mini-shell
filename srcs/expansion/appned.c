#include "minishell.h"

// dstの末尾にsrcを追加した文字列を返す
// dstとsrcはfreeする（どちらもmallocされた文字列の前提）
char	*append_string_free(char *dst, char *src)
{
	size_t	new_len;
	char	*new;

	if (!dst || !src)
	{
		free(dst);
		free(src);
		return (NULL);
	}
	new_len = ft_strlen(dst) + ft_strlen(src) + 1;
	new = (char *)malloc(new_len);
	if (!new)
	{
		perror("minishell");
		free(dst);
		free(src);
		return (NULL);
	}
	new[0] = '\0';
	ft_strlcpy(new, dst, new_len);
	ft_strlcat(new, src, new_len);
	free(dst);
	free(src);
	return (new);
}

// dstの末尾にcを追加した文字列を返す
// dstはfreeする（mallocされた文字列の前提）
char	*append_char_free(char *dst, char c)
{
	size_t	len;
	char	*new;
	char	tmp[2];

	if (!dst)
	{
		tmp[0] = c;
		tmp[1] = '\0';
		return (ft_strdup(tmp));
	}
	len = ft_strlen(dst);
	new = (char *)malloc(len + 2);
	if (!new)
	{
		perror("minishell");
		free(dst);
		return (NULL);
	}
	ft_strlcpy(new, dst, len + 1);
	new[len] = c;
	new[len + 1] = '\0';
	free(dst);
	return (new);
}
