/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   changed_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segurbuz <segurbuz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 21:13:10 by segurbuz          #+#    #+#             */
/*   Updated: 2023/11/02 08:26:13 by segurbuz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	initialize_lexer2(char *commands)
{
	int	len;

	len = ft_strlen(commands);
	if (len == 1)
	{
		if (*commands == '<')
			return (INPUT_RDR);
		else if (*commands == '>')
			return (OUTPUT_RDR);
	}
	if (len == 2)
	{
		if (*commands == '<' && *(commands + 1) == '<')
			return (DOUBLE_INPUT_RDR);
		else if (*commands == '>' && *(commands + 1) == '>')
			return (DOUBLE_OUTPUT_RDR);
	}
	return (1);
}

void	make_sense2(t_newlst **tmp)
{
	t_newlst	*list;
	int			i;

	list = *tmp;
	while (list != NULL)
	{
		i = 0;
		while (list->content[i] != NULL)
		{
			if (*list->content[i] == '<'
				|| *list->content[i] == '>')
			{
				list->type[i] = initialize_lexer2(list->content[i]);
				list->list_type = OUTPUT_RDR;
			}
			else
				list->type[i] = WORD;
			i++;
		}
		list = list->next;
	}
}

int	array_size(t_arg *temp)
{
	int	i;

	i = 0;
	while (temp != NULL)
	{
		if (temp->content[0] == '|' \
			|| (temp->content[0] == '\0' && !temp->next))
			break ;
		i++;
		temp = temp->next;
	}
	return (i);
}

t_arg	*add_list(t_newlst **list, t_arg *tmp, int size)
{
	t_newlst	*arg;
	int			i;
	int			j;

	i = -1;
	j = -1;
	arg = *list;
	arg->content = (char **)ft_calloc(sizeof(char *), (size + 2));
	arg->type = (int *)ft_calloc(sizeof(int), size + 1);
	while (++i < size)
	{
		if (*tmp->content != 0)
		{
			arg->content[++j] = ft_strdup(tmp->content);
		}
		tmp = tmp->next;
	}
	arg->content[size] = NULL;
	return (tmp);
}

void	change_list(t_arg *temp)
{
	t_newlst	*list;
	t_newlst	*headlst;
	int			size;

	list = ft_calloc(sizeof(t_newlst), 1);
	headlst = list;
	while (temp != NULL)
	{
		size = array_size(temp);
		temp = add_list(&list, temp, size);
		if (temp != NULL)
		{
			temp = temp->next;
			if (temp)
			{
				list->next = ft_calloc(sizeof(t_newlst), 1);
				list = list->next;
			}
		}
	}
	list->next = NULL;
	g_data.arg = headlst;
	make_sense2(&g_data.arg);
}
