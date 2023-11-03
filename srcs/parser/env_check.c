/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segurbuz <segurbuz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 18:38:56 by segurbuz          #+#    #+#             */
/*   Updated: 2023/11/03 01:17:24 by segurbuz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*env_add_dollars2(char *str_start, char *str_end, char *tmp_path)
{
	char	*new_str;

	new_str = ms_strjoin(str_start, tmp_path);
	free(tmp_path);
	new_str = ms_strjoin(new_str, str_end);
	return (free(str_end), new_str);
}

char	*env_add_dollars(char *str, char *path, int check)
{
	char	*str_start;
	char	*str_end;
	char	*tmp_path;
	char	*new_str;
	int		i;

	i = 0;
	tmp_path = NULL;
	struct_initilaize(NULL, 0);
	i = env_control(str, i);
	str_start = ft_substr(str, 0, i);
	if (check == 1)
		str_end = ft_substr(str, i + 1, ft_strlen(str));
	else
		str_end = ft_substr(str, (ft_strlen(path + 1) + i) + 1 \
			, ft_strlen(str) - i);
	i = -1;
	tmp_path = env_add_dollars3(str, tmp_path, path, i);
	new_str = env_add_dollars2(str_start, str_end, tmp_path);
	free(str);
	return (new_str);
}

void	find_env_name3(t_arg *temp, char *path, int check)
{
	if (check == 1)
	{
		if (ft_strncmp(temp->content, "~/", 2) == 0 \
			|| ft_strcmp(temp->content, "~") == 0)
		{
			path = ft_strdup(temp->content);
			temp->content = env_add_dollars(temp->content, path, 1);
			free(path);
		}
		return ;
	}
}

void	find_env_name2(t_arg *temp, char *path, int i, int len)
{
	is_check(temp->content[i]);
	if (g_data.quot_type != '\'' && i > 0 && temp->content[i - 1] == '$')
	{
		len = 1;
		while (len++ && env_check(NULL, temp->content[i], 1))
		{
			if (!env_check(NULL, temp->content[i], 1)
				|| temp->content[i - 1] == '?')
				break ;
			i++;
		}
		path = ft_substr(temp->content, (i - len) + 1, len - 1);
		if ((path[ft_strlen(path) - 1] == '?')
			&& ft_strlen(path) >= 3)
			path[ft_strlen(path) - 1] = '\0';
		temp->content = env_add_dollars(temp->content, path, 0);
		g_data.quot = 0;
		i = -1;
		free(path);
	}
	else if (temp->content[i] == '~')
		find_env_name3(temp, path, 1);
}

void	find_env_name(t_arg *temp)
{
	int		i;
	int		len;
	char	*path;

	path = NULL;
	len = 0;
	while (temp != NULL)
	{
		if (env_check(temp->content, '\0', 0))
		{
			i = -1;
			while (temp->content[++i] != '\0')
				find_env_name2(temp, path, i, len);
		}
		temp = temp->next;
	}
}
