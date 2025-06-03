#include "minishell.h"

// 環境変数名に含むことができる文字かどうか判定
int	is_valid_var_char(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

// 変数名部分を抜粋して引数のnameに設定（例：$HOME@# -> HOME）
int	extract_var_name(char *str, char **name)
{
	int	len;

	*name = NULL;
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
char	*get_var_value(char *name)
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
char	*process_dollar(char **s, char *result)
{
	char	*name;
	char	*ret;
	char	*value;

	(*s)++;
	if (extract_var_name(*s, &name) != 0)
	{
		free(result);
		return (NULL);
	}
	if (!name)
	{
		ret = ft_strjoin(result, "$");
		free(result);
		return (ret);
	}
	*s += ft_strlen(name);
	value = get_var_value(name);
	free(name);
	if (!value)
		return (NULL);
	ret = ft_strjoin(result, value);
	free(result);
	free(value);
	return (ret);
}
