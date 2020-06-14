#include<stdio.h>



int main(void)
{
	struct MyPoint
	{
		int x_bdj;
		int y_bdj;
	}point_bdj;

	struct MyProperties
	{
		int quadrant_bdj;
		char axis_location[10];
	}point_properties_bdj;
	printf("\n\n");
	printf("Enter X-Coordinate for x :\n");
	scanf("%d", &point_bdj.x_bdj);

	printf("Enter Y-Coordinte for Y :\n");
	scanf("%d", &point_bdj.y_bdj);
	printf("\n\n");

	printf("X and Y are: x = %d and y = %d\n", point_bdj.x_bdj, point_bdj.y_bdj);

	if (point_bdj.x_bdj && point_bdj.y_bdj == 0)
	{
		printf("Point is Origin (%d , %d)\n", point_bdj.x_bdj, point_bdj.y_bdj);
	}
	else
	{
		if (point_bdj.x_bdj == 0)
		{
			if (point_bdj.y_bdj < 0)
			{
				strcpy(point_properties_bdj.axis_location, "Negative Y");
			}
			if (point_bdj.y_bdj > 0)
			{
				strcpy(point_properties_bdj.axis_location, "Positive Y");
			}
			point_properties_bdj.quadrant_bdj = 0;
			printf("Point Lies on %s Axis..\n", point_properties_bdj.axis_location);
		}
		else if (point_bdj.y_bdj == 0)
		{
			if (point_bdj.x_bdj < 0)
			{
				strcpy(point_properties_bdj.axis_location, "Negative X");
			}
			if (point_bdj.x_bdj > 0)
			{
				strcpy(point_properties_bdj.axis_location, "Positive X");
			}
			point_properties_bdj.quadrant_bdj = 0;
			printf("Point Lies on %s Axis..\n", point_properties_bdj.axis_location);
		}
		else
		{
			point_properties_bdj.axis_location[0] = '\0';

			if (point_bdj.x_bdj > 0 && point_bdj.y_bdj > 0)
			{
				point_properties_bdj.quadrant_bdj = 1;
			}
			else if (point_bdj.x_bdj < 0 && point_bdj.y_bdj > 0)
			{
				point_properties_bdj.quadrant_bdj = 2;
			}
			else if (point_bdj.x_bdj < 0 && point_bdj.y_bdj < 0)
			{
				point_properties_bdj.quadrant_bdj = 3;
			}
			else
			{
				point_properties_bdj.quadrant_bdj = 4;
			}

			printf("Point Lies in %d quadrant\n", point_properties_bdj.quadrant_bdj);
		}
	}
	return 0;
}