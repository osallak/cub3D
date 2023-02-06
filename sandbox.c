#include "cub3D.h"
typedef struct s_door
{
	int x;
	int y;	
	double dist;
	struct s_door *prev;
	struct s_door *next;
}	t_door;

double distance(double x, double y, double a, double b)
{
	return (sqrt((a - x) * (a - x) + (b - y) * (b - y)));
}

void	*xalloc(size_t size)
{
	void *ptr = malloc(size);
	if (!ptr)
		exit(1);
	return (ptr);
}

t_door	*create_door(int x, int y, double dist)
{
	t_door	*door;
	door = xalloc(sizeof(t_door));
	door->x = x;
	door->y = y;
	door->dist = dist;
	door->next = 0x0;
	door->prev = 0x0;
	return door;
}

void	add_door(t_door **head, t_door *door, t_door **tail)
{
	t_door *tmp;
	if (head == 0x0 || !door)
		exit(1);
	if (*head == 0x0 || *tail == 0x0)
	{
		*head = door;
		*tail = door;
		return ;
	}
	tmp = *head;
	while (tmp->next != 0x0)
		tmp = tmp->next;
	door->prev = tmp;
	tmp->next = door;
	*tail = door;
}

void delete_door(t_door **head, t_door **door, t_door *tail)
{
	// t_door *prev;
	// t_door *next;
	if (*door == *head)
		*head = (*head)->next;
	else if (*door == tail)
	{
		(*door)->prev->next = 0x0;
	}
	else
	{
		if ((*door)->prev && (*door)->next)
		{
			(*door)->prev->next = (*door)->next;
			(*door)->next->prev = (*door)->prev;
		}
	}
	free(*door);
    *door = 0x0;
}

int main()
{
    t_door *head = 0x0;
    t_door *tail = 0x0;
    add_door(&head, create_door(10, 2, 10.2), &tail);
    add_door(&head, create_door(10, 3, 10.2), &tail);
    add_door(&head, create_door(10, 4, 10.2), &tail);
    add_door(&head, create_door(10, 5, 10.2), &tail);
    add_door(&head, create_door(10, 6, 10.2), &tail);
    add_door(&head, create_door(10, 7, 10.2), &tail);
    // delete_door(&head, head, tail);
    // delete_door(&head, head->next, tail);
    // delete_door(&head, tail, tail);
    t_door *tmp = 0x0;
    t_door *t = head;
    t_door *current = head;
    while(current)
    {
        tmp= current->next;
        if (current->y == 5 || current->y == 6)
            delete_door(&head, &current, tail);
        current = tmp;
    }
    while (t)
    {
        printf("%d\n", t->y);
        t = t->next;
    }
}