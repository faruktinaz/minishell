/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segurbuz <segurbuz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:50:02 by segurbuz          #+#    #+#             */
/*   Updated: 2023/10/30 02:32:25 by segurbuz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int redirection_check(t_arg *temp)
{
	int	type;

	type = temp->content[0];
	if (temp->content[1] != '\0' && temp->content[1] != type)
		return (parse_error(127), 1);
	else if(temp->next == NULL || temp->next->content[0] == '|')
		return (parse_error(127), 1);
	return (0);
}

int	pipe_check(t_arg *temp)
{
	if (temp->next == NULL || temp->next->content[0] == '|')
		return (parse_error(127), 1);
	return (0);
}

void	*error_check(t_arg *temp)
{
	int	check;

	check = 0;
	if (temp == NULL)
		return (parse_error(127), NULL);
	else if (temp->content[0] == '|')
		return (parse_error(127), NULL);
	while (temp != NULL)
	{
		if (temp->content[0] == '|')
			check = pipe_check(temp);
		else if (temp->content[0] == '>' \
			|| temp->content[0] == '<')
			check = redirection_check(temp);
		if (check == 1)
			break ;
		temp = temp->next;
	}
	return (NULL);
}