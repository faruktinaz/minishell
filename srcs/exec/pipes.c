/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogenc <ogenc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 08:53:09 by ogenc             #+#    #+#             */
/*   Updated: 2023/11/03 02:12:09 by ogenc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_child_exc(t_exec *data, t_newlst *tmp)
{
	if (tmp->content[0] && is_built_in(data, tmp->content) != 1)
	{
		if (execve(data->path, tmp->content, data->env_p) == -1)
		{
			errno = 127;
			if (g_data.err_ty == 0)
				printf("command not found: %s\n", tmp->content[0]);
			else if (g_data.err_ty == 1)
				printf("No such file or directory: %s\n", \
					tmp->content[0]);
		}
	}
	exit(errno);
}

void	ft_exec_child(t_exec *data, t_newlst *tmp, int total_pipe, int in)
{
	ft_execute(data, tmp);
	dup2(in, 0);
	if (total_pipe - 1 != -1 || g_data.fdout == 1)
		dup2(g_data.fd[1], 1);
	close(g_data.fd[0]);
	close(g_data.fd[1]);
	ft_child_exc(data, tmp);
}

void	ft_exec_parent(int **in, int total_pipe)
{
	if ((**in) != 0)
		close(((**in)));
	if (total_pipe != 0)
		(**in) = dup(g_data.fd[0]);
	close(g_data.fd[1]);
	close(g_data.fd[0]);
}

void	init_child_p(t_newlst *tmp, t_exec *data, int total_pipe, int *in)
{
	pid_t		pid;

	if (tmp->list_type != WORD)
		ft_exec_rdr(&tmp);
	pipe(g_data.fd);
	pid = fork();
	if (!pid)
		ft_exec_child(data, tmp, total_pipe, (*in));
	else
		ft_exec_parent(&in, total_pipe);
}


void	ft_exec_w_pipes(t_exec *data)
{
	t_newlst	*tmp;
	int			total_pipe;
	int			total_exec;
	int			in;

	tmp = g_data.arg;
	total_pipe = g_data.counter->pipe;
	total_exec = total_pipe + 1;
	in = 0;
	while (total_pipe >= 0)
	{
		init_child_p(tmp, data, total_pipe, &in);
		total_pipe--;
		if (tmp->next)
			tmp = tmp->next;
	}
	total_pipe = total_exec - 1;
	while (total_pipe >= 0)
	{
		waitpid(-1, NULL, 0);
		total_pipe--;
	}
	return ;
}
