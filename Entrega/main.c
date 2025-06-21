/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armgonza <armgonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 19:45:51 by armgonza          #+#    #+#             */
/*   Updated: 2025/06/20 19:49:48 by armgonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	create_threads(t_data *data)
{
	int	i;

	data->start_time = timestamp();
	i = 0;
	while (i < data->nb_philo)
	{
		data->philos[i].last_meal = data->start_time;
		if (pthread_create(&data->philos[i].thread, NULL, &routine,
				&data->philos[i]))
			return (cleanup(data, "pthread_create error"));
		i++;
	}
	return (0);
}

static int	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_join(data->philos[i].thread, NULL))
			return (cleanup(data, "pthread_join error"));
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	monitor_th;

	if (init_data(&data, argc, argv))
		return (1);
	if (create_threads(&data))
		return (1);
	if (pthread_create(&monitor_th, NULL, &monitor, &data))
		return (cleanup(&data, "monitor thread error"));
	if (pthread_join(monitor_th, NULL))
		return (cleanup(&data, "monitor join error"));
	join_threads(&data);
	cleanup(&data, NULL);
	return (0);
}
