/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogenc <ogenc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 00:33:47 by segurbuz          #+#    #+#             */
/*   Updated: 2023/11/02 12:24:04 by ogenc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void	double_input_rdr(t_newlst *tmp, int i)
{
	char	*input;
	char	*end_name;
	int		fd;
	
	end_name = tmp->content[i + 1];
	fd = open("heredoc.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
	while (1)
	{
		g_data.in_rdr = 2;
		input = readline("> ");
		if (g_data.in_rdr == 5 || !input)
		{
			free(input);
			dup2(g_data.default_in, 1);
			break ;
		}
		dup2(fd, 1);
		if (ft_strcmp(input, end_name) == 0)
		{
			free(input);
			dup2(g_data.default_out, 1);
			break ;
		}
		printf("%s\n", input);
		dup2(g_data.default_out, 1);
		free(input);
	}
	g_data.input_name = "heredoc.txt";
	g_data.fdin = 1;
	close(fd);
	if (g_data.in_rdr == 5)
	{
		fd = open("heredoc.txt", O_RDWR | O_TRUNC, 0644);
		close(fd);
	}		
}
