/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim_quot.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segurbuz <segurbuz@student.42istanbul.co>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 06:14:21 by segurbuz          #+#    #+#             */
/*   Updated: 2023/09/18 06:14:22 by segurbuz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	quot_check(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

int	new_str_lenght(char *str)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (str[i])
	{
		is_check(str[i]);
		if (str[i] == g_data.quot_type)
			while (str[++i] != g_data.quot_type)
				len++;
		else
			len++;
		struct_initilaize(NULL, 0);
		i++;
	}
	return (len);
}

char	*trim_quot(char *str)
{
	int		len;
	char	*new_str;
	int		i;
	int		j;

	len = new_str_lenght(str);
	struct_initilaize(NULL, 0);
	new_str = malloc(sizeof(char) * (len + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		is_check(str[i]);
		if (str[i] == g_data.quot_type)
			while (str[++i] != g_data.quot_type)
				new_str[j++] = str[i];
		else
			new_str[j++] = str[i];
		struct_initilaize(NULL, 0);
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

void	check_quot_list(t_arg *temp)
{
	t_arg	*tmp;
	char	*s;
	int		i;

	i = 0;
	tmp = temp;
	while (tmp != NULL)
	{
		if (quot_check(tmp->content))
		{
			s = tmp->content;
			tmp->content = trim_quot(tmp->content);
			struct_initilaize(NULL, 0);
			free(s);
		}
		tmp = tmp->next;
	}
	g_data.quot = 0;
}
