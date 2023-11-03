/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogenc <ogenc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 19:42:22 by segurbuz          #+#    #+#             */
/*   Updated: 2023/11/03 03:40:03 by ogenc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_arg	*save_for_after(t_arg *list, int i)
{
	t_arg	*newlst;
	t_arg	*tmp;
	t_arg	*tmp_newlst;

	tmp = list;
	newlst = ft_calloc(sizeof(t_arg), 1);
	if (!tmp->next)
		return (NULL);
	while (tmp != NULL)
	{
		if (tmp->type != i)
			ms_lstadd_back(&newlst, ms_lstnew(tmp->type, \
						ft_strdup(tmp->content)));
		tmp = tmp->next;
	}
	tmp_newlst = newlst;
	newlst = newlst->next;
	free(tmp_newlst);
	return (newlst);
}

void	ft_unset2(char **content)
{
	t_arg	*tmp;
	t_arg	*newlst;
	int		i;

	tmp = g_data.exp_p;
	i = find_env_dir2(g_data.exp_p, content[1]);
	if (i != -1)
	{
		if (ms_lstsize(g_data.exp_p) == 1)
			freelizer(&g_data.exp_p, NULL);
		else
		{
			newlst = save_for_after(tmp, i);
			freelizer(&g_data.exp_p, NULL);
			g_data.exp_p = newlst;
		}
	}
}
