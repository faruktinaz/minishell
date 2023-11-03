/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segurbuz <segurbuz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 17:10:55 by segurbuz          #+#    #+#             */
/*   Updated: 2023/11/03 01:44:27 by segurbuz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void	output_rdr(t_newlst *list, int i)
{
	g_data.out_fd = open(list->content[i + 1] \
		, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	g_data.fdout = 1;
}

void	double_output_rdr(t_newlst *list, int i)
{
	g_data.out_fd = open(list->content[i + 1] \
		, O_CREAT | O_WRONLY | O_APPEND, 0644);
	g_data.fdout = 1;
}

void	input_rdr(t_newlst *list, int i)
{
	g_data.input_name = list->content[i + 1];
	g_data.fdin = 1;
}

void	select_rdr_type(t_newlst *tmp, int i)
{
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
	i = -1;
	while (tmp->content[++i] != NULL)
	{
		if (tmp->type[i] != WORD)
		{
			select_rdr_type(tmp, i);
			i++;
		}
	}
	if (tmp->type[0] == WORD)
		tmp->content = change_newlst(tmp, last_rdr_check(tmp, 0, 1), 0);
	else
		tmp->content = change_newlst(tmp, last_rdr_check(tmp, 0, 0), 1);
}
