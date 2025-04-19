#include "minishell.h"

void	print_tokens(t_token *token)
{
	const char *type_str[] = {
		[TK_WORD] = "WORD",
		[TK_RESERVED] = "RESERVED",
		[TK_EOF] = "EOF"
	};

	while (token)
	{
		printf("[%-8s] %s\n", type_str[token->type], token->str);
		token = token->next;
	}
}

void	debug_tokenizer(t_token *tokens)
{
    if (!tokens)
    {
        ft_dprintf(STDERR_FILENO, "minishell: no tokens\n");
        return ;
    }
    print_tokens(tokens);
    free_tokens(tokens);
}
