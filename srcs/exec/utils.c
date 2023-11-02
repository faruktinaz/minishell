/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogenc <ogenc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 21:37:01 by ogenc             #+#    #+#             */
/*   Updated: 2023/11/02 21:38:18 by ogenc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	struct_initilaize(char **envp, int rule)
{
	(void)envp;
	if (rule == 1)
		g_data.error_code = 0;
	g_data.error_flag = 0;
	g_data.quot = 0;
	g_data.in_rdr = 0;
	g_data.quot_type = 1000;
	g_data.fdin = 0;
	g_data.fdout = 0;
	g_data.err_ty = 0;
	g_data.exec_check = 0;
	dup2(g_data.default_in, 0);
	dup2(g_data.default_out, 1);
}
