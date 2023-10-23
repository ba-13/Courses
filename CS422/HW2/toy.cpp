int main()
{
    int a = 5;
    for (int i = 0; i < 1000'000; i++)
        if (i % 2)
            a++;
        else
            a--;
    return 0;
}