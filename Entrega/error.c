/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armgonza <armgonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 19:45:39 by armgonza          #+#    #+#             */
/*   Updated: 2025/06/20 19:49:35 by armgonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	destroy_mutexes(t_data *data)
{
	int	i;

	if (!data->forks)
		return ;
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->data_mutex);
}

int	cleanup(t_data *data, char *msg)
{
	if (msg)
		fprintf(stderr, "Error: %s\n", msg);
	if (data)
	{
		destroy_mutexes(data);
		if (data->philos)
			free(data->philos);
		if (data->forks)
			free(data->forks);
	}
	return (1);
}
