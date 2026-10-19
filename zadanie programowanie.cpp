#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>
 
/* Walidacja liczby; EOF bezpiecznie koñczy program. */
static double read_number(const char *prompt)
{
    char line[256], *end;
    double value;
 
    for (;;) {
        printf("%s", prompt);
        fflush(stdout);
 
        if (!fgets(line, sizeof line, stdin))
            exit(EXIT_SUCCESS);
 
        /* Odrzuæ zbyt d³ug¹ liniê. */
        if (!strchr(line, '\n') && !feof(stdin)) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            puts("Zbyt dlugie dane.");
            continue;
        }
 
        errno = 0;
        value = strtod(line, &end);
 
        if (end != line) {
            while (isspace((unsigned char)*end))
                ++end;
            if (*end == '\0' && errno == 0 && isfinite(value))
                return value;
        }
 
        puts("Niepoprawna liczba. Uzyj kropki dziesietnej.");
    }
}
 
static void hello(void)
{
    puts("Hello!");
}
 
static void number_demo(void)
{
    int Number = 10;
 
    printf("Wartosc poczatkowa: %d\n", Number);
 
    for (int i = 0; i < 3; ++i)
        Number += 5;
 
    printf("Po pierwszej serii: %d\n", Number);
 
    for (int i = 0; i < 2; ++i)
        Number -= 3;
 
    Number += 5;
    printf("Wartosc koncowa: %d\n", Number);
}
 
static void quadratic(void)
{
    double a = read_number("a = ");
    double b = read_number("b = ");
    double c = read_number("c = ");
 
    if (a == 0) {
        if (b == 0)
            puts(c == 0 ? "Nieskonczenie wiele rozwiazan."
                        : "Brak rozwiazan.");
        else {
            double x = -c / b;
            if (isfinite(x))
                printf("Rownanie liniowe: x = %.17g\n", x);
            else
                puts("Wynik poza zakresem.");
        }
        return;
    }
 
    double delta = b * b - 4 * a * c;
 
    if (!isfinite(delta)) {
        puts("Delta poza zakresem obliczen.");
        return;
    }
 
    printf("Delta = %.17g\n", delta);
 
    if (delta < 0) {
        puts("Brak pierwiastkow rzeczywistych.");
        return;
    }
 
    double x1, x2;
    if (delta == 0) {
        x1 = x2 = (-0.5 * b) / a;
    } else {
        /* Stabilniejszy wzor: ogranicza odejmowanie bliskich liczb. */
        double q = -0.5 * b - 0.5 * copysign(sqrt(delta), b);
        x1 = q / a;
        x2 = c / q;
    }
 
    if (!isfinite(x1) || !isfinite(x2)) {
        puts("Pierwiastki poza zakresem obliczen.");
    } else if (delta == 0) {
        printf("Pierwiastek podwojny: x = %.17g\n", x1);
    } else {
        printf("x1 = %.17g\nx2 = %.17g\n", fmin(x1, x2), fmax(x1, x2));
    }
}
 
int main(void)
{
    for (;;) {
        puts("\n1. Hello!\n2. Number\n3. Rownanie kwadratowe\n0. Koniec");
        double choice = read_number("Wybor: ");
 
        if      (choice == 0) return EXIT_SUCCESS;
        else if (choice == 1) hello();
        else if (choice == 2) number_demo();
        else if (choice == 3) quadratic();
        else puts("Wybierz 0, 1, 2 lub 3.");
    }
}
