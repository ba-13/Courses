#include "shared.h"

int partition_x(point *points, int p, int r)
{
    int pivotIdx = r;
    int pivot = points[pivotIdx].x;
    int i = p - 1;
    for (int j = p; j < r; j++)
    {
        if (points[j].x < pivot)
        {
            i += 1;
            swap(&points[i], &points[j]);
        }
    }
    swap(&points[i + 1], &points[pivotIdx]);
    return i + 1;
}

int partition_y(point *points, int p, int r)
{
    int pivotIdx = r;
    int pivot = points[pivotIdx].y;
    int i = p - 1;
    for (int j = p; j < r; j++)
    {
        if (points[j].y < pivot)
        {
            i += 1;
            swap(&points[i], &points[j]);
        }
    }
    swap(&points[i + 1], &points[pivotIdx]);
    return i + 1;
}

void quickSort_x(point *points, int b, int a)
{
    if (b >= a)
        return;
    int pivotIdx = partition_x(points, b, a);
    quickSort_x(points, b, pivotIdx - 1);
    quickSort_x(points, pivotIdx + 1, a);
}

void quickSort_y(point *points, int b, int a)
{
    if (b >= a)
        return;
    int pivotIdx = partition_y(points, b, a);
    quickSort_y(points, b, pivotIdx - 1);
    quickSort_y(points, pivotIdx + 1, a);
}

pair_points *closest_pair(point *points, int b, int a)
{
    pair_points *min_pair = (pair_points *)malloc(sizeof(pair_points));
    // you need to store a copy of points inside min_pair instead of pointing
    // to the indices because they swap
    point *p1 = (point *)malloc(sizeof(point));
    point *p2 = (point *)malloc(sizeof(point));

    min_pair->p1 = p1, min_pair->p2 = p2;
    if (b >= a)
    {
        // allot minimum pair such that they have huge distance
        // signifying infinity
        p1->x = -99999, p2->x = 99999;
        p1->y = -99999, p2->y = 99999;
        min_pair->p1 = p1, min_pair->p2 = p2;
        return min_pair;
    }
    if (b + 1 == a)
    {
        // allot minimum pair the given pair
        p1->x = points[b].x, p1->y = points[b].y;
        p2->x = points[a].x, p2->y = points[a].y;
        min_pair->p1 = p1, min_pair->p2 = p2;
        return min_pair;
    }

    int mid = (b + a) / 2;
    pair_points *below = closest_pair(points, b, mid); // find closest_pair between b and mid inclusive
    pair_points *above = closest_pair(points, mid, a); // find closest_pair between mid and a inclusive
    double delta1_sq = euclid_distance_sq(below->p1, below->p2);
    double delta2_sq = euclid_distance_sq(above->p1, above->p2);
    double delta_sq;
    if (delta1_sq < delta2_sq)
    {
        min_pair = below;
        free(above);
        delta_sq = delta1_sq;
    }
    else
    {
        min_pair = above;
        free(below);
        delta_sq = delta2_sq;
    }

    // check by removing this part
    int lower_bound_idx = lower_bound(points, b, mid - 1, delta_sq, points[mid].x);
    int upper_bound_idx = upper_bound(points, mid + 1, a, delta_sq, points[mid].x);

    quickSort_y(points, lower_bound_idx, upper_bound_idx); // this is sorting within delta strip
    /*
    printf("Lowest distance: %f\n", delta_sq);
    printf("Closest Pair till now: ");
    print_point(min_pair->p1);
    print_point(min_pair->p2);
    printf("\n");
    */

    int num_points_within_boundaries = upper_bound_idx - lower_bound_idx + 1;
    for (int i = lower_bound_idx; i < num_points_within_boundaries; i++)
    {
        for (int j = i + 1; j < minimum_int(num_points_within_boundaries, i + 8); j++)
        {
            double tmp_sq = euclid_distance_sq(&points[i], &points[j]); // obtain distance
            if (tmp_sq < delta_sq)
            {

                p1->x = points[i].x, p2->x = points[j].x;
                p1->y = points[i].y, p2->y = points[j].y;
                // min_pair already points to p1 and p2
                delta_sq = tmp_sq;
                min_pair->p1 = p1, min_pair->p2 = p2;
            }
        }
    }

    return min_pair;
}

int main(int argc, char **argv)
{
    int tmp;
    int num_points;
    char *fileName = "_random_numbers.txt";

    if (argc < 3)
    {
        fprintf(stderr, "Arguments needed: [fileName] [n]\n");
        fprintf(stderr, "fileName - file path containing numbers to be sorted\n");
        fprintf(stderr, "n - #numbers in file\n");
        num_points = 3;
    }
    else
    {
        num_points = atoi(argv[2]);
        fileName = argv[1];
    }

    point points[num_points];
    FILE *filePointer;
    filePointer = fopen(fileName, "r");
    for (int i = 0; i < num_points; i++)
    {
        tmp = fscanf(filePointer, "%lf ", &(points[i].x));
        tmp = fscanf(filePointer, "%lf ", &(points[i].y));
    }
    fclose(filePointer);

    if (num_points <= 100)
        print_points(points, num_points);

    gettimeofday(&t1, NULL);
    quickSort_x(points, 0, num_points - 1);
    pair_points *result = closest_pair(points, 0, num_points - 1);
    gettimeofday(&t2, NULL);

    printf("Time: %ldus\n", (t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec));

    printf("Closest: ");
    print_point(result->p1);
    print_point(result->p2);
    printf(" Distance: %f\n", euclid_distance_sq(result->p1, result->p2));

    return 0;
}
