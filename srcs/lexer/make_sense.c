/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_sense.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segurbuz <segurbuz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 23:25:10 by segurbuz          #+#    #+#             */
/*   Updated: 2023/10/17 23:48:50 by segurbuz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	initialize_lexer(t_arg *list)
{
	int	len;

	len = ft_strlen(list->content);
	if (len == 1)
	{
		if (*list->content == '<')
			list->type = INPUT_RDR;
		else if (*list->content == '>')
			list->type = OUTPUT_RDR;
		else if (*list->content == '|')
			list->type = PIPE;
	}
	if (len == 2)
	{
		if (*list->content == '<' && *(list->content + 1) == '<')
			list->type = DOUBLE_INPUT_RDR;
		else if (*list->content == '>' && *(list->content + 1) == '>')
			list->type = DOUBLE_OUTPUT_RDR;
	}
}

void	make_sense(t_arg **lst)
{
	t_arg	*list;

	list = *lst;
	while (list != NULL)
	{	
		if (*list->content == '|' || *list->content == '<'
			|| *list->content == '>')
			initialize_lexer(list);
		else
			list->type = WORD;
		list = list->next;
	}
}