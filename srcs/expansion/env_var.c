#include "minishell.h"

// 環境変数名に含むことができる文字かどうか判定
static int	is_valid_var_char(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

// 変数名部分を抜粋して引数のnameに設定（例：$HOME@# -> HOME）
static int	extract_var_name(char *str, char **name)
{
	int	len;

	if (*str == '?')
	{
		*name = ft_strdup("?");
		if (!(*name))
			return (1);
		return (0);
	}
	len = 0;
	while (str[len] && is_valid_var_char(str[len]))
		len++;
	if ((!ft_isalpha(str[0]) && str[0] != '_') || len == 0)
	{
		*name = NULL;
		return (0);
	}
	*name = ft_strndup(str, len);
	if (!(*name))
		return (1);
	return (0);
}

// 環境変数の値を取得
static char	*get_var_value(char *name)
{
	char	*value;
	char	*env_value;

	if (ft_strncmp(name, "?", 2) == 0)
		value = ft_itoa(sh_stat(ST_GET, 0));
	else
	{
		env_value = ft_env(ENV_GET_VAL, name);
		if (!env_value)
			value = ft_strdup("");
		else
			value = ft_strdup(env_value);
	}
	return (value);
}

// $記号の処理
static char	*process_dollar(char **s, char *result)
{
	char	*name;
	char	*temp;
	char	*value;

	(*s)++;
	if (extract_var_name(*s, &name) != 0)
	{
		free(result);
		return (NULL);
	}
	if (!name)
	{
		temp = ft_strjoin(result, "$");
		free(result);
		return (temp);
	}
	*s += ft_strlen(name);
	value = get_var_value(name);
	free(name);
	if (!value)
		return (result);
	temp = ft_strjoin(result, value);
	free(result);
	free(value);
	return (temp);
}

// $が連続している間展開して文字列を連結
char	*expand_env_var_process(char **s)
{
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (**s)
	{
		if (**s == '$')
		{
			result = process_dollar(s, result);
			if (!result)
				return (NULL);
		}
		else
			break ;
	}
	return (result);
}

// ${変数名}または$?を展開してexpトークンを作成
t_exp_tkn	*expand_env_var(char **s, bool is_quoted)
{
	char		*expanded;
	t_exp_tkn	*token;

	expanded = expand_env_var_process(s);
	if (!expanded)
		return (NULL);
	token = new_exp_token(expanded, true, is_quoted);
	if (!token)
	{
		free(expanded);
		return (NULL);
	}
	return (token);
}
