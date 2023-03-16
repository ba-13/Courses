#define EXP_BITS 32

typedef unsigned int UI;

UI modexp(UI base, UI exponent, UI mod)
{
    int i;
    UI result = 1;
    i = EXP_BITS;
    while (i > 0 && exponent != 0)
    {
        if ((exponent & 1) == 1)
        {
            result = (result * base) % mod;
        }
        exponent >>= 1;
        base = (base * base) % mod;
        i--;
    }
    return result;
}

int main()
{
    modexp(23, 12, 1e7 + 7);
}