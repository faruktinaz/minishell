/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segurbuz <segurbuz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 01:41:53 by segurbuz          #+#    #+#             */
/*   Updated: 2023/11/03 01:45:15 by segurbuz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

int	last_rdr_check(t_newlst *tmp, int i, int check)
{
	int	count;

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

void	change_newlst_rdr_start(t_newlst *tmp \
	, char ***new_str, int count)
{
	int	i;

	i = -1;
	(*new_str) = ft_calloc(sizeof (char *), (count + 2 + g_data.fdin));
	while (++i < count)
		(*new_str)[i] = ft_strdup(tmp->content[i]);
	if (g_data.fdin == 1)
		(*new_str)[i] = ft_strdup(g_data.input_name);
	(*new_str)[count + 1 + g_data.fdin] = NULL;
}

void	change_newlst_rdr_not_start(t_newlst *tmp \
	, char ***new_str, int lenght, int count)
{
	int	i;

	i = -1;
	while (tmp->content[lenght])
		lenght++;
	lenght -= count;
	(*new_str) = ft_calloc(sizeof (char *), (lenght + 2 + g_data.fdin));
	while (++i < lenght)
		(*new_str)[i] = ft_strdup(tmp->content[count + i]);
	if (g_data.fdin == 1)
		(*new_str)[i] = ft_strdup(g_data.input_name);
	(*new_str)[lenght + 1 + g_data.fdin] = NULL;
}

char	**change_newlst(t_newlst *tmp, int count, int check)
{
	int		lenght;
	char	**new_str;

	lenght = 0;
	new_str = NULL;
	if (check == 1)
		change_newlst_rdr_not_start(tmp, &new_str, lenght, count);
	else if (check == 0)
		change_newlst_rdr_start(tmp, &new_str, count);
	g_data.fdin = 0;
	return (free_commands(tmp->content), new_str);
}
