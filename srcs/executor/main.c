/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogenc <ogenc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 03:56:43 by ogenc             #+#    #+#             */
/*   Updated: 2023/11/02 09:11:05 by ogenc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	if (!s1 || !s2)
		return (-1);
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

void	free_commands(char **commands)
{
	int	x;

	x = -1;
	while (commands[++x])
		free(commands[x]);
	free(commands);
}

char	*ft_join_m(t_exec *data, char **commands)
{
	char	*str;
	char	*str1;
	char	*str2;

	str = ft_strjoin("/", commands[0]);
	str2 = find_access(data, str);
	str1 = ft_strjoin(str2, commands[0]);
	free(str);
	if (ft_strcmp(str2, "error"))
		free(str2);
	return (str1);
}

void    change_output_or_input(void)
{
	if (g_data.fdout == 1)
	{
		dup2(g_data.out_fd, 1);
		close(g_data.out_fd);
	}
}

static void	delete_hat(void)
{
	struct termios	termios_p;

	if (tcgetattr(0, &termios_p) != 0)
		perror("Minishell: tcgetattr");
	termios_p.c_lflag &= ~ECHOCTL;
	if (tcsetattr(0, 0, &termios_p) != 0)
		perror("Minishell: tcsetattr");
}

int	is_built_in(t_exec *data, char **content)
{
	data->res = 0;
	if (ft_strcmp(content[0], "exit") == 0)
		ft_exit(data, content);
	else if (ft_strcmp(content[0], "cd") == 0)
		ft_cd(data, content);
	else if (!(ft_strcmp(content[0], "echo")))
	{
		ft_echo(content);
		data->res = 1;
	}
	else if (!(ft_strcmp(content[0], "pwd")))
		ft_pwd(data);
	else if (!(ft_strcmp(content[0], "export")))
		ft_export(data, content);
	else if (!(ft_strcmp(content[0], "unset")))
		ft_unset(data, content);
	else if (!(ft_strcmp(content[0], "env")))
		ft_p_env(data);
	return (data->res);
}
void	init_signals()
{
	delete_hat();
	signal(SIGINT, &handle_signals);
	signal(SIGQUIT, &handle_signals);
}

int	main (int argc, char **argv, char **env)
{
	t_exec	*data;

	(void)argv;
	(void)argc;
	init_signals();
	data = malloc(sizeof(t_exec));
	data->t_exp = malloc(sizeof(t_list));
	struct_initilaize(NULL, 1);
	set_envp(data, env);
	g_data.default_in = dup(0);
	g_data.default_out = dup(1);
	ft_readline(data);
}

void	struct_initilaize(char **envp, int rule)
{
	(void)envp;
	if (rule == 1)
		g_data.error_code = 0;
	g_data.error_flag = 0;
	g_data.quot = 0;
	g_data.in_rdr = 0;
	g_data.quot_type = 1000;
	g_data.fdin = 0;
	g_data.fdout = 0;
	g_data.err_ty = 0;
	g_data.exec_check = 0;
	dup2(g_data.default_in, 0);
	dup2(g_data.default_out, 1);
}
