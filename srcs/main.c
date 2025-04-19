#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*input;
    t_token    *tokens;

	// TODO: (void)あとで消す(かも)
	(void)argc;
	(void)argv;
    (void)envp;
	input = NULL;
	tokens = NULL;
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
        tokens = tokenize(input);
        if (!tokens)
        {
            free(input);
            exit(EXIT_FAILURE);
        }
        /* TODO: 入力行を解析 */
        // TODO: debugあとで消す
        debug_tokenizer(tokens);
		// execute(tokens, envp);
		free(input);
	}
	exit(EXIT_SUCCESS);
}
