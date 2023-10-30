/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogenc <ogenc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 17:10:55 by segurbuz          #+#    #+#             */
/*   Updated: 2023/10/30 01:39:55 by ogenc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void    output_rdr(t_newlst *list, int i)
{
	g_data.out_fd = open(list->content[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	g_data.fdout = 1;
}

void    double_output_rdr(t_newlst *list, int i)
{
	g_data.out_fd = open(list->content[i + 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
	g_data.fdout = 1;
}

void    input_rdr(t_newlst *list, int i)
{
	g_data.in_fd = open(list->content[i + 1], O_RDONLY, 0644);
	g_data.fdin = 1;
}

int last_rdr_check(t_newlst *tmp, int i)
{
	int count;

	count = 0;
	while (tmp->content[i] != NULL)
	{
		if (tmp->content[i][0] == '>' || tmp->content[i][0] == '<')
			count = i;
		i++;
	}
	return (count + 2);
}

void    select_rdr_type(t_newlst *tmp, int i)
{
	if (i == 0)
		g_data.exec_check = last_rdr_check(tmp, i);
	else
	{
		free(tmp->content[i]);
		tmp->content[i] = NULL;
	}
	if (tmp->type[i] == OUTPUT_RDR)
		output_rdr(tmp, i);
	else if (tmp->type[i] == DOUBLE_OUTPUT_RDR)
		double_output_rdr(tmp, i);
	else if (tmp->type[i] == INPUT_RDR)
		input_rdr(tmp, i);
	else if (tmp->type[i] == DOUBLE_INPUT_RDR)
		double_input_rdr(tmp, i);
}

void	ft_exec_rdr(t_newlst **list)
{
	t_newlst	*tmp;
	int			i;

	tmp = *list;
	while (tmp != NULL)
	{
		i = -1;
		while (tmp->content[++i] != NULL)
		{
			if (tmp->type[i] != WORD)
			{
				select_rdr_type(tmp, i);
				i++;
			}
		}
		tmp = tmp->next;
	}
}
