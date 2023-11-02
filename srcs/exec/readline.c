/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogenc <ogenc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 09:04:12 by ogenc             #+#    #+#             */
/*   Updated: 2023/11/03 01:28:00 by ogenc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_check_f(t_exec *data)
{
	if (check_is_dir(g_data.arg->content[0]) == 1)
		g_data.err_ty = 1;
	if (access(g_data.arg->content[0], F_OK))
		data->path = ft_join_m(data, g_data.arg->content);
	else
		data->path = ft_strdup(g_data.arg->content[0]);
}

void	ft_err_p(void)
{
	errno = 127;
	if (g_data.err_ty == 0)
		printf("command not found: %s\n", g_data.arg->content[0]);
	else if (g_data.err_ty == 1)
		printf("No such file or directory: %s\n", g_data.arg->content[0]);
}

int	ft_sig_rdr_h(void)
{
	int	res;

	res = 0;
	if (g_data.arg->list_type != WORD && g_data.counter->pipe < 1)
	{
		ft_exec_rdr(&g_data.arg);
		change_output_or_input();
	}
	if (g_data.in_rdr == 1)
	{
		g_data.in_rdr = 0;
		res = 1;
	}
	return (res);
}

void	ft_exec_p(t_exec *data)
{
	int	pid;

	if (g_data.counter->pipe > 0)
		ft_exec_w_pipes(data);
	else if (g_data.arg->content[0] && \
				is_built_in(data, g_data.arg->content) != 1)
	{
		ft_check_f(data);
		pid = fork();
		if (pid == 0)
		{
			execve(data->path, g_data.arg->content, data->env_p);
			ft_err_p();
			exit(errno);
		}
		else
			waitpid(pid, &g_data.error_code, 0);
		free(data->path);
	}
}

void	ft_readline(t_exec *data)
{
	while (1)
	{
		g_data.line = readline("\033[34mminishell \033[0;35m$ \033[0m");
		if (!g_data.line)
		{
			write(1, "exit", 4);
			exit(0);
		}
		if (ft_strncmp(g_data.line, "", ft_strlen(g_data.line)) != 0)
			add_history(g_data.line);
		ft_parse();
		if (!(ft_strncmp(g_data.line, "\0", 1) == 0) && g_data.error_flag == 0)
		{
			if (ft_sig_rdr_h() == 1)
				continue ;
			ft_exec_p(data);
		}
		free(g_data.line);
		free(g_data.counter);
		freelizer(&g_data.list, &g_data.arg);
		struct_initilaize(NULL, 0);
	}
}
