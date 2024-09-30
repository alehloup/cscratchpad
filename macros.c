#include "aleh/prettyc.h"

struct stest {int x; float y;};

static inline void teste_equal(void) {
  float x = 1.0f, y = 1.0f;
    double m = 2.0, n = 2.0;
    char hello[6] = {'h', 'e', 'l', 'l', 'o', '\0'};
    char *str1 = hello, *str2 = hello;
    const char *lit1 = "world", *lit2 = "world";
    int a = 5, b = 5;
    size_t sz1 = 42, sz2 = 42;
    int xv[5] = {1, 5, 3};
    int yv[5] = {1, 5, 3};
    struct stest k = {5, 5.2f}, l = {5, 5.2f};

    if (equal(x, y)) {
        printf("Floats are equal\n");
    }

    if (equal(m, n)) {
        printf("Doubles are equal\n");
    }

    if (equal(str1, str2)) {
        printf("Strings are equal\n");
    }

    if (equal(lit1, lit2)) {
        printf("String literals are equal\n");
    }

    if (equal(a, b)) {
        printf("Integers are equal\n");
    }

    if (equal(sz1, sz2)) {
        printf("Sizes are equal\n");
    }

    if (equal_array(xv, yv)) {
        printf("equal arrays\n");
    }

    if (equal_pstruct(&k, &l)) {
        printf("equal structs\n");
    }
}

static inline void farray(int arr[atleast 10]) {
  int i = 0;

  foreach(el, int, 10, arr)
    *el = i++;

  foreach(el, int, 10, arr)
    printf("%d ", *el);
  printf("\n");
}

int main(void) {
  int x[10] = ZEROINIT;
  int t[3] = {1, 2, 3};

  fortimes(y, 5)
    printf("%d ", y);
  printf("\n");

  printf("%zu %zu \n", len(x), len("alessandro"));

  foreach(var, int, len(t), t)
    printf("%d ", *var);
  printf("\n");

  forascby(var, 0, 9, 2)
    printf("%d ", var);
  printf("\n");

  fordescby(var, 9, 0, 2)
    printf("%d ", var);
  printf("\n");

  withfile (file, "hello.txt", "w")
    fprintf(file, "Hello world!");

  teste_equal();

  farray(x);

  printf("\nSuccess\n");

  return 0;
}
