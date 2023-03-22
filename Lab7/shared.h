#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

struct timeval t1, t2;

typedef struct point
{
    double x;
    double y;
} point;

typedef struct pair_points
{
    point *p1, *p2;
} pair_points;

typedef struct tuple_idx
{
    int idx1, idx2;
} tuple_idx;

double euclid_distance_sq(point *x, point *y)
{
    return (x->x - y->x) * (x->x - y->x) + (x->y - y->y) * (x->y - y->y);
}

void print_point(point *pt)
{
    printf("(%0.1lf, %0.1lf) ", pt->x, pt->y);
}

void print_points(point *points, int num_points)
{
    for (int i = 0; i < num_points; i++)
    {
        print_point(&points[i]);
    }
    printf("\n");
}

void swap(point *a, point *b)
{
    point tmp = *a;
    *a = *b;
    *b = tmp;
}

double minimum_double(double a, double b)
{
    return a > b ? b : a;
}

int minimum_int(int a, int b)
{
    return a > b ? b : a;
}

int lower_bound(point *points, int b, int a, double thresh_dist_sq, double x)
{
    int mid;
    int low = b, high = a;
    double dist;
    while (low < high)
    {
        mid = (low + high) / 2;
        dist = x - points[mid].x;
        if (dist * dist >= thresh_dist_sq)
        {
            high = mid;
        }
        else
        {
            low = mid + 1;
        }
    }
    dist = points[low].x - x;
    if (low <= a && dist * dist < thresh_dist_sq)
        low++;
    return low;
}

int upper_bound(point *points, int b, int a, double thresh_dist_sq, double x)
{
    int mid;
    int low = b, high = a;
    double dist;
    while (low < high)
    {
        mid = (low + high) / 2;
        dist = x - points[mid].x;
        if (dist * dist > thresh_dist_sq)
        {
            high = mid;
        }
        else
        {
            low = mid + 1;
        }
    }
    dist = points[low].x - x;
    if (low < a && dist * dist <= thresh_dist_sq)
        low++;
    return low;
}
