#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "point.h"
#include "sortedPoints.h"

Point originPoint = {0.0, 0.0};

void sp_shiftPoints(SortedPoints *sp, int position);
void sp_printArray(SortedPoints *sp);

/*
 * Initialize data structures, returning pointer
 * to the object.
 */
SortedPoints *sp_init(SortedPoints *sp){

  sp->size = 0;
  
  int i;

  for (i=0; i < 10; i++)
  {
	(sp->used)[i] = 'n';
  } 

  return sp;
}

/*
 * Allocate a new point and initialize it to x,y. Then
 * add that point to the SortedPointss list. Return
 * 1 on success and 0 on error (e.g., out of memory).
 */

int sp_addNewPoint(SortedPoints *sp, double x, double y)
{
        
	if (sp->size == 10)
	{
		printf("SortedPoint list full\n");
		return 0;
	}
	else if (sp->size > 10)
	{
		printf("List above 10 for some reason, error\n");
		return 0;
	}

	else
	{
		Point *originPointer = &originPoint;

		Point *newPoint = (Point*)malloc(sizeof(Point));
		point_set(newPoint, x, y);

		int position;
		int placePos;
		Point tempPoint;
		Point *tempPointer;

		for (position = 0; position < 10; position++)
		{

			if (sp->used[position] == 'n')
			{

				sp->pointArray[position] = *newPoint;
				sp->used[position] = 'y';
				break;
			}

			else
			{
				tempPoint = sp->pointArray[position];
				tempPointer = &tempPoint;

				if (point_distance(originPointer, newPoint) == point_distance(originPointer, tempPointer))
				{
					if (point_getX(newPoint) == point_getX(tempPointer))
					{
						if (point_getY(newPoint) < point_getY(tempPointer))
						{

							placePos = position;

							//shift everything else over
							sp_shiftPoints(sp, position);

							//this position holds newPoint
							sp->pointArray[placePos] = *newPoint;
							break;
						}
					}

					else if (point_getX(newPoint) < point_getX(tempPointer))
					{

						placePos = position;

						//shift everything else over
						sp_shiftPoints(sp, position);

						//this position holds newPoint
						sp->pointArray[placePos] = *newPoint;
						break;
					}

				}

				else if (point_distance(originPointer, newPoint) < point_distance(originPointer, tempPointer))
				{

					placePos = position;

					//shift everything else over
					sp_shiftPoints(sp, position);

					//this position holds newPoint
					sp->pointArray[placePos] = *newPoint;
					break;
				}

			}
		}		

		free(newPoint);
		//free(originPointer);
		sp->used[sp->size] = 'y';
		sp->size = sp->size + 1;

		return 1;
	}
        
	
}

void sp_shiftPoints(SortedPoints *sp, int position)
{

	int i;

	for (i = 9; i > position; i--)
	{
		sp->pointArray[i] = sp->pointArray[i-1];	
	}

}

void sp_printArray(SortedPoints *sp)
{

	int i;
	Point temp;
	
	for (i = 0; i < 10; i++)
	{
		temp = sp->pointArray[i];
		printf("%d: (%f, %f)\n", i, point_getX(&temp), point_getY(&temp));
	}

	for (i = 0; i < 10; i++)
	{
		printf("[ %c ] ", sp->used[i]);
	}

	printf("\n");

}

/*
  * Remove the first point from the sorted list,
  * storing its value in *ret. Returns 1 on success
  * and 0 on failure (empty list).
  */

int sp_removeFirst(SortedPoints *sp, Point *ret)
{

	if (sp->size == 0 || sp->used[0] == 'n')
	{
		if (NULL != ret)
			point_set(ret, 0.0, 0.0);

		return 0;
	}

	else
	{

		int i;

		sp->size = sp->size - 1;

		if (NULL != ret)
			*ret = sp->pointArray[0];

		sp->used[sp->size] = 'n';

		for (i = 0; i < sp->size; i++)
		{
			sp->pointArray[i] = sp->pointArray[i+1];
		}

		point_set(&(sp->pointArray[sp->size]), 0.0, 0.0);
		//free(&(sp->pointArray)[0]);
		return 1;
	}
}

/*
  * Remove the last point from the sorted list,
  * storing its value in *ret. Returns 1 on success
  * and 0 on failure (empty list).
  */

int sp_removeLast(SortedPoints *sp, Point *ret)
{
	if (sp->size == 0)
	{
		if (NULL != ret)
			point_set(ret, 0.0, 0.0);

		return 0;
	}

	else if (sp->used[sp->size - 1] == 'n')
	{
		if (NULL != ret)
			point_set(ret, 0.0, 0.0);
		return 0;
	}

	else
	{

		sp->size = sp->size - 1;

		if (NULL != ret)
			*ret = sp->pointArray[sp->size];

		point_set(&(sp->pointArray[sp->size]), 0.0, 0.0);
		//free(&(sp->pointArray[sp->size]));
		sp->used[sp->size] = 'n';
		return 1;
	}
}

/*
  * Remove the point that appears in position
  * <index> on the sorted list, storing its 
  * value in *ret. Returns 1 on success
  * and 0 on failure (too short list).
  *
  * The first item on the list is at index 0.
  */
int sp_removeIndex(SortedPoints *sp, int index, Point *ret)
{
	if (sp->size == 0 || index < 0 || index > 10)
	{
		if (NULL != ret)
			point_set(ret, 0.0, 0.0);
		return 0;
	}

	else if (sp->used[index] == 'n')
	{
		if (NULL != ret)
			point_set(ret, 0.0, 0.0);
		return 0;
	}

	else
	{
		int i;

		sp->size = sp->size - 1;

		if (NULL != ret)
			*ret = sp->pointArray[index];

		sp->used[sp->size] = 'n';

		for (i = index; i < sp->size; i++)
		{
			sp->pointArray[i] = sp->pointArray[i+1];
		}

		point_set(&(sp->pointArray[sp->size]), 0.0, 0.0);
		//free(&(sp->pointArray[sp->size]));
		return 1;

	}
}


/*
  * Delete any duplicate records. E.g., if
  * two points on the list have *identical*
  * x and y values, then delete one of them.
  * Return the number of records deleted.
  */
int sp_deleteDuplicates(SortedPoints *sp)
{
  int numRecordsDel = 0;
  int i, j;

  Point temp;
  Point temp2;

  for (i = 0; i < 10; i++)
  {

	temp = sp->pointArray[i];

	if (sp->used[i] == 'y')
	{
		j=i+1;

		if (sp->used[j] == 'y')
		{

			temp2 = sp->pointArray[j];

			while (point_getX(&temp) == point_getX(&temp2) && point_getY(&temp) == point_getY(&temp2))
			{

				sp_removeIndex(sp, j, (Point*)NULL);

				temp2 = sp->pointArray[j];
			}
		}
	}
  }

  return numRecordsDel;

}


