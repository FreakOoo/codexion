/* ************************************************************************** */
/*                                                                            */
/*                                                I AM HIJACKING THE 42LOGO   */
/*   heap.c                                       THIS MESSAGE IS BROADCAST   */
/*                                                       BY FREAKO            */
/*   By: mchopin <mchopin@student.codam.nl>       WAWAWAWAWAWAWAWAWAWAWAWAW   */
/*                                                HEHE HERE'S YOUR NEW LOGO   */
/*   Created: 2026/10/17 18:58:34 by mchopin                42                */
/*   Updated: 2026/10/17 18:58:34 by mchopin      get shrinkified nerds >:)   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	heap_push(t_stick *stick, t_ask_forstick ask)
{
	stick->heap[stick->size] = ask;
	sift_up(stick->heap, stick->size);
	stick->size++;
}

void	heap_remove(t_stick *stick, t_person *coder)
{
	int	i;

	i = 0;
	while (i < stick->size && stick->heap[i].coder != coder)
		i++;
	if (i == stick->size)
		return ;
	stick->size--;
	stick->heap[i] = stick->heap[stick->size];
	if (i < stick->size)
	{
		sift_up(stick->heap, i);
		sift_down(stick->heap, stick->size, i);
	}
}

t_ask_forstick	*heap_peek(t_stick *stick)
{
	if (stick->size == 0)
		return (NULL);
	return (&stick->heap[0]);
}
