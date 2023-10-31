/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segurbuz <segurbuz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 17:10:55 by segurbuz          #+#    #+#             */
/*   Updated: 2023/10/30 15:59:28 by segurbuz         ###   ########.fr       */
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

int last_rdr_check(t_newlst *tmp, int i, int check)
{
	int count;

	count = 0;
	while (tmp->content[i] != NULL)
	{
		if (tmp->content[i][0] == '>' || tmp->content[i][0] == '<')
		{
			count = i;
			if (check == 1)
				break ; 
		}
		i++;
	}
	if (check == 1)
		return (count);
	else
		return (count + 2);
}

char	**change_newlst(t_newlst *tmp, int count, int check)
{
	int		lenght;
	char	**new_str;
	int		i;

	i = -1;
	lenght = 0;
	new_str = NULL;
	if (check == 1)
	{
		while (tmp->content[lenght])
			lenght++;
		lenght -= count;
		new_str = ft_calloc(sizeof (char *), (lenght + 2));
		while (++i < lenght)
			new_str[i] = ft_strdup(tmp->content[count + i]);
		new_str[lenght + 1] = NULL;
	}
	else if (check == 0)
	{
		new_str = ft_calloc(sizeof (char *), (count + 2));
		while (++i < count)
			new_str[i] = ft_strdup(tmp->content[i]);
		new_str[count + 1] = NULL;
	}
	return (free_commands(tmp->content), new_str);
}

void    select_rdr_type(t_newlst *tmp, int i)
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
		if (tmp->type[0] == WORD)
			tmp->content = change_newlst(tmp, last_rdr_check(tmp, 0, 1), 0);
		else
			tmp->content = change_newlst(tmp, last_rdr_check(tmp, 0, 0), 1);
		tmp = tmp->next;
	}
}
