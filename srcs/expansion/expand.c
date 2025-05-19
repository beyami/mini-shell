#include "minishell.h"

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

// 最後の終了ステータスを取得
// シグナルがあったなら128+シグナルの値
int get_last_exit_status(void)
{
    if (g_sig_received)
        return (128 + g_sig_received);
    return (sh_stat(ST_GET, 0));
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
	{
		value = ft_itoa(get_last_exit_status());
		printf("value: %s\n", value);
	}
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

// 文字列を展開
int	expand_node_str(char **str, char **envp)
{
	char	*expanded;

	expanded = process_quotes(*str, envp);
	if (!expanded)
		return (1);
	free(*str);
	*str = expanded;
	return (0);
}

// ND_CMDの展開
void	expand_cmd_node(t_node *node, char **envp)
{
	int	i;

	i = 0;
	while (i < node->argc)
	{
		if (expand_node_str(&node->argv[i], envp) != 0)
		{
			sh_stat(ST_SET, 1);
			return ;
		}
		i++;
	}
	i = 0;
	while (i < node->redir_count)
	{
		if (expand_node_str(&node->redirs[i]->str, envp) != 0)
		{
			sh_stat(ST_SET, 1);
			return ;
		}
		i++;
	}
}

// 変数展開メイン処理
void	expand(t_node *node, char **envp)
{
	if (!node)
		return ;
	// 再帰で一番左のノードから根に向かって展開
	expand(node->lhs, envp);
	if (sh_stat(ST_GET, 0) != 0)
		return ;
	// 自身がND_PIPEなら先に右のノードを展開
	expand(node->rhs, envp);
	if (sh_stat(ST_GET, 0) != 0)
		return ;
	// ND_CMDのみ展開を行う
	if (node->kind != ND_CMD)
		return ;
	expand_cmd_node(node, envp);
	// コマンドと引数の文字列を展開
	// while (i < node->argc)
	// {
	// 	if (expand_node_str(&node->argv[i], envp) != 0)
	// 	{
	// 		sh_stat(ST_SET, 1);
	// 		return ;
	// 	}
	// 	i++;
	// }
	// // リダイレクトの文字列を展開
	// if (expand_redirs(node->redirs, node->redir_count, envp) != 0)
	// {
	// 	sh_stat(ST_SET, 1);
	// 	return ;
	// }
}
