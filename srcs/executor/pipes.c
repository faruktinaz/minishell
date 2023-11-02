/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogenc <ogenc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 08:53:09 by ogenc             #+#    #+#             */
/*   Updated: 2023/11/02 12:42:23 by ogenc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exec_w_pipes(t_exec *data, char **commands)
{
	t_newlst	*tmp;
	int			total_pipe;
	int			total_exec;
	int			in;
	pid_t		pid;

	tmp = g_data.arg;
	total_pipe = g_data.counter->pipe;
	total_exec = total_pipe + 1;
	in = 0;
	(void)commands;
	while (total_pipe >= 0)
	{
		if (tmp->list_type != WORD)
			ft_exec_rdr(&tmp);
		pipe(g_data.fd);
		pid = fork();
		if (!pid)
		{
			if (check_is_dir(tmp->content[0]) == 1)
				g_data.err_ty = 1;
			if (access(tmp->content[0], F_OK) != 0)
				data->path = ft_join_m(data, tmp->content);
			else
				data->path = ft_strdup(tmp->content[0]);
			if (g_data.fdout == 1)
				g_data.fd[1] = g_data.out_fd;
			dup2(in, 0);
			if (total_pipe - 1 != -1 || g_data.fdout == 1)
				dup2(g_data.fd[1], 1);
			close(g_data.fd[0]);
			close(g_data.fd[1]);
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
		else
		{
			if (in != 0)
				close(in);
			if (total_pipe != 0)
				in = dup(g_data.fd[0]);
			close(g_data.fd[1]);
			close(g_data.fd[0]);
		}
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