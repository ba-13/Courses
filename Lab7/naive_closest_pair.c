#include "shared.h"

tuple_idx closest_pair(point *points, int num_points)
{
    point *tmp = NULL, *tmp2 = NULL;
    int m1, m2;
    double min_dist_sq = 999999999;
    for (int curr = 0; curr < num_points; curr++)
    {
        tmp = (points + curr);
        for (int i = curr + 1; i < num_points; i++)
        {
            tmp2 = (points + i);
            double dist_sq = euclid_distance_sq(tmp, tmp2);
            if (min_dist_sq > dist_sq)
            {
                m1 = curr;
                m2 = i;
                min_dist_sq = dist_sq;
            }
        }
    }
    tuple_idx p;
    p.idx1 = m1;
    p.idx2 = m2;
    return p;
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Arguments needed: [fileName] [n]\n");
        fprintf(stderr, "fileName - file path containing numbers to be sorted\n");
        fprintf(stderr, "n - #numbers in file\n");
        return -1;
    }

    int tmp;
    int num_points = atoi(argv[2]);
    point points[num_points];

    FILE *filePointer;
    filePointer = fopen(argv[1], "r");
    for (int i = 0; i < num_points; i++)
    {
        tmp = fscanf(filePointer, "%lf ", &(points[i].x));
        tmp = fscanf(filePointer, "%lf ", &(points[i].y));
    }
    fclose(filePointer);

    if (num_points <= 100)
      print_points(points, num_points);

    gettimeofday(&t1, NULL);
    tuple_idx closest = closest_pair(points, num_points);
    gettimeofday(&t2, NULL);

    printf("Time: %ldus\n", (t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec));

    printf("Closest: ");
    print_point(&points[closest.idx1]);
    print_point(&points[closest.idx2]);
    printf(" Distance: %f\n", euclid_distance_sq(&points[closest.idx1], &points[closest.idx2]));

    return 0;
}
