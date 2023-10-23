/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogenc <ogenc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 15:28:25 by segurbuz          #+#    #+#             */
/*   Updated: 2023/10/20 01:35:29 by ogenc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libary/libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <dirent.h>
# include <stdarg.h>
# include <string.h>
# include <stdbool.h>

enum	character {
	WORD,
    INPUT_RDR,
    OUTPUT_RDR,
    DOUBLE_OUTPUT_RDR,
    DOUBLE_INPUT_RDR,
    PIPE,
};
typedef struct s_counter
{
	int	rdr;
	int	heredoc;
	int	pipe;
	int	word;
}		t_counter;

typedef struct s_arg
{
	char			*content;
	int				type;
	struct s_arg	*next;
}					t_arg;

typedef struct s_newlst
{
	char			**content;
	int				type;
	struct s_newlst	*next;
}					t_newlst;

typedef struct s_exec
{
	char	**env_p;
	char 	*path;
}	t_exec;

typedef struct s_data
{
	t_counter	*counter;
	t_arg		*list;
	t_newlst	*arg;
	char		*line;
	char		**envp;
	char		*path;
	char		**redirection;
	int			error_flag;
	int			error_code;
	int			quot;
	int			fd[2];
	int			tmp;
	int			quot_type;
}				t_data;

t_data	g_data;

void	ft_parse(void);
void	freelizer(t_arg **free);
int		is_check(char c);
void	struct_initilaize(char **envp, int rule);
bool	env_check(char const *str, char c, int rule);
char	*env_find(char *path);
void	check_quot_list(t_arg *temp);
char	*env_add_dollars(char *str, char *path);
int		env_control(char *str, int i);
void	ft_error(char *str);
void	change_list(t_arg *temp);
void	find_env_name(t_arg *temp);
void	parse_error(int error_code);
void	*error_check(t_arg *temp);
t_arg	*ms_lstnew(int type, void *content);
void	ms_lstadd_back(t_arg **lst, t_arg *new);
t_arg	*ms_lstlast(t_arg *lst);
char	*ms_strjoin(char *s1, char *s2);
void	make_sense(t_arg **list);

#endif
