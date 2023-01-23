#include "auction.hpp"

void reset_assignement(array<bool> *array_mask)
{
	for (int i = 0; i < array_mask->cols; i++)
	{
		array_mask->data[i] = false;
	}
}