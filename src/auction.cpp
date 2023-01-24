#include "auction.hpp"

void reset_assignement(array<bool> *array_mask)
{
	for (int i = 0; i < array_mask->cols; i++)
	{
		array_mask->data[i] = false;
	}
}

bool assignement_found(array<bool> *agents_mask)
{
	bool rval = agents_mask->data[0];
	for (int i = 1; i < agents_mask->cols; i++)
	{
		rval = rval && agents_mask->data[i];
	}
	return rval;
}