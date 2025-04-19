#ifndef MINISHELL_H
# define MINISHELL_H

# include "ft_printf.h"
# include "get_next_line.h"
# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum e_token_type
{
	TK_RESERVED,
	TK_WORD,
	TK_EOF
}					t_token_type;

typedef struct s_token
{
	char			*str;
	t_token_type	type;
	struct s_token	*next;
}					t_token;

// execution
// child.c
void				exec_if_relative_path(char **cmds, char **envp);
void				exec_if_absolute_path(char **cmds, char **envp);
void				execute_in_child(char **cmds, char **envp);

// execute.c
int					execute(char **cmds, char **envp);

// find_path.c
char				*search_path(char *cmd_name, char **path_list,
						char *path_tail);
char				*resolve_cmd_path(char *cmd, char *path_env);

// free.c
void				free_2d_array(void **array);

// tokenization
// tokenize.c
t_token				*tokenize(char *line);

// free
// free.c
void				free_tokens(t_token *token);

// debug
// debug_tokenize.c
void				print_tokens(t_token *token);
void				debug_tokenizer(t_token *tokens);

#endif