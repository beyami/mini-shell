#include "minishell.h"

void	free_2d_array(void **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void free_tokens(t_token *token)
{
    t_token *tmp;

    while (token)
    {
        tmp = token->next;
        free(token->str);
        free(token);
        token = tmp;
    }
}
