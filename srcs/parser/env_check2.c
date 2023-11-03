/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_check2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segurbuz <segurbuz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 01:04:12 by segurbuz          #+#    #+#             */
/*   Updated: 2023/11/03 01:17:18 by segurbuz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	env_check(char const *str, char c, int rule)
{
	int	i;

	i = -1;
	if (rule == 1)
	{
		if (ft_isalnum(c) || c == '_' \
			|| c == '?')
			return (true);
		return (false);
	}
	else
	{
		while (str[++i] != '\0')
			if (str[i] == '$' || str[i] == '~')
				return (true);
		return (false);
	}
}

int	env_size(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '=')
			break ;
		i++;
	}
	return (i);
}

char	*env_find(char *path)
{
	int	i;

	i = -1;
	if (ft_strlen(path + 1) == 1)
	{
		if (path[1] == '?')
		{
			errno = g_data.error_code;
			return (ft_itoa(errno / 256));
		}
	}
	while (g_data.envp[++i])
	{
		if (ft_strncmp(g_data.envp[i], path + 1, ft_strlen(path + 1)) == 0
			&& env_size(g_data.envp[i]) <= (int)ft_strlen(path + 1))
			break ;
	}
	if (g_data.envp[i] == NULL)
		return (ft_strdup("\0"));
	else
		return (ft_strdup(g_data.envp[i] + env_size(path)));
}

char	*env_add_dollars3(char *str, char *tmp_path, char *path, int i)
{
	while (str[++i] != '\0')
	{
		if (g_data.quot_type != '\'' && str[i] == '$')
		{
			tmp_path = env_find(path);
			g_data.error_code = 0;
			break ;
		}
		else if (str[i] == '~')
			tmp_path = ft_strdup(getenv("HOME"));
	}
	return (tmp_path);
}
