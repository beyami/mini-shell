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

// 環境変数名に含むことができる文字かどうか判定
int	is_var_char(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

// 変数名部分を抜粋して引数のnameに設定（例：$HOME@# -> HOME）
int	get_var_name(char *str, char **name)
{
	int	len;

	if (*str == '?')
	{
		*name = ft_strdup("?");
		if (!(*name))
			return (-1);
		return (0);
	}
	len = 0;
	while (str[len] && is_var_char(str[len]))
		len++;
	if (len == 0)
	{
		*name = NULL;
		return (0);
	}
	*name = ft_strndup(str, len);
	if (!(*name))
		return (-1);
	return (0);
}

// ${変数名}を変数展開した文字列を返す
// 例: $HOME -> /home/stakada
// 同時に${変数名}の次の文字までポインタを進める
char	*expand_var(char **s, char **envp)
{
	char	*name;
	char	*value;
	char	*env;

	(*s)++;
	if (get_var_name(*s, &name) < 0)
		return (NULL);
	if (!name)
		return (ft_strdup("$"));
	*s += ft_strlen(name);
	if (ft_strncmp(name, "?", 2) == 0)
		// TODO: 実際のexitステータスに変更
		value = ft_itoa(0);
	else
	{
		env = ft_getenv(name, envp);
		if (!env)
			value = ft_strdup("");
		else
			value = ft_strdup(env);
	}
	free(name);
	return (value);
}

// ${変数名}と$?を展開後の文字列を置き換える
// 同時にクォートの除去を行う
char	*process_quotes(char *str, char **envp)
{
	char	*result;
	char	*ptr;
	char	quote;
	char	*expanded;

	// 最初に空の文字列を用意
	result = ft_strdup("");
	ptr = str;
	while (*ptr)
	{
		if (*ptr == '\'' || *ptr == '"')
		{
			quote = *ptr;
			ptr++;
			while (*ptr && *ptr != quote)
			{
				if (quote == '"' && *ptr == '$')
				{
					expanded = expand_var(&ptr, envp);
					if (!expanded)
					{
						free(result);
						return (NULL);
					}
					result = append_string_free(result, expanded);
					if (!result)
						return (NULL);
				}
				else
				{
					result = append_char_free(result, *ptr);
					if (!result)
						return (NULL);
					ptr++;
				}
			}
			ptr++;
		}
		else if (*ptr == '$')
		{
			expanded = expand_var(&ptr, envp);
			if (!expanded)
			{
				free(result);
				return (NULL);
			}
			result = append_string_free(result, expanded);
			if (!result)
				return (NULL);
		}
		else
		{
			result = append_char_free(result, *ptr);
			if (!result)
				return (NULL);
			ptr++;
		}
	}
	return (result);
}

int expand_node_str(char **str, char **envp)
{
	char *expanded;

	expanded = process_quotes(*str, envp);
	if (!expanded)
		return (-1);
	free(*str);
	*str = expanded;
	return (0);
}

int expand_redirs(t_redir **redirs, int redir_count, char **envp)
{
	int i;

	i = 0;
	while (i < redir_count)
	{
		if (expand_node_str(&redirs[i]->str, envp) == -1)
			return (-1);
		i++;
	}
	return (0);
}

int expand(t_node *node, char **envp)
{
	int i;

	if (!node)
		return (0);
	if (expand(node->lhs, envp) == -1)
		return (-1);
	if (expand(node->rhs, envp) == -1)
		return (-1);
	i = 0;
	while (i < node->argc)
	{
		if (expand_node_str(&node->argv[i], envp) == -1)
			return (-1);
		i++;
	}
	if (expand_redirs(node->redirs, node->redir_count, envp) == -1)
		return (-1);
	return (0);
}

// // $展開とクォート除去
// int	expand_tokens(t_token *tokens, char **envp)
// {
// 	t_token	*cur;
// 	char	*expanded;

// 	cur = tokens;
// 	while (cur)
// 	{
// 		if (cur->type == TK_WORD)
// 		{
// 			expanded = process_quotes(cur->str, envp);
// 			if (!expanded)
// 				return (-1);
// 			free(cur->str);
// 			cur->str = expanded;
// 		}
// 		cur = cur->next;
// 	}
// 	return (0);
// }
