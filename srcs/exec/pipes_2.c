/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogenc <ogenc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 02:12:15 by ogenc             #+#    #+#             */
/*   Updated: 2023/11/03 02:12:16 by ogenc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

void	change_output_or_input(void)
{
	if (g_data.fdout == 1)
	{
		dup2(g_data.out_fd, 1);
		close(g_data.out_fd);
	}
}

void	ft_execute(t_exec *data, t_newlst *tmp)
{
	if (check_is_dir(tmp->content[0]) == 1)
		g_data.err_ty = 1;
	if (access(tmp->content[0], F_OK) != 0)
		data->path = ft_join_m(data, tmp->content);
	else
		data->path = ft_strdup(tmp->content[0]);
	if (g_data.fdout == 1)
		g_data.fd[1] = g_data.out_fd;
}