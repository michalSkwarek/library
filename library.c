#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Struktura stosu
struct stack {
    char *key; //wskaznik na poczatek lancucha znakow
    struct stack *next;
};

//Struktura wierzcholkow stosow
struct stacks_of_books {
    struct stack *books_on_table; //dla stosu ksiazek na stole
    struct stack *books_under_table; //dla stosu ksiazek zdjetych ze stolu
};

/*Funkcja dodajaca dany tytul na stos ksiazek na stole. Jako parametry wejsciowe przyjmuje strukture wierzcholkow stosow
  oraz wskaznik na poczatek danego tytulu. Zwraca strukture wierzcholkow stosow.*/
struct stacks_of_books add_book(struct stacks_of_books S, char *book_title) {
    struct stack *temp = (struct stack *)malloc(sizeof(struct stack));

    temp->key = strdup(book_title);
    free(book_title);
    temp->next = S.books_on_table;
    S.books_on_table = temp;
    return S;
}

/*Funkcja usuwajaca dane tytuly ksiazek ze stosu na stole oraz okladajaca te usuniete tytuly na nowym stosie.
  Jako parametr wejsciowy przyjmuje strukture wierzcholkow stosow. Zwraca strukture wierzcholkow stosow.*/
struct stacks_of_books remove_book(struct stacks_of_books S) {
    struct stack *temp = (struct stack *)malloc(sizeof(struct stack));
    struct stack *p = S.books_on_table; //wskaznik na wierzcholek stosu ksiazek na stole
    if(S.books_on_table == NULL)
        printf("Brak ksiazek na stole.\n");
    else {
        temp->key = S.books_on_table->key;
        temp->next = S.books_under_table;
        S.books_under_table = temp;
        S.books_on_table = S.books_on_table->next;
        free(p);
    }
    return S;
}

/*Funkcja czytajaca plik wejsciowy i umieszczajaca przeczytane tytuly ksiazek na stosie ksiazek na stole.
  Jako parametry wejsciowe przyjmuje strukture wierzcholkow stosow i wskaznik do poczatku pliku wejsciowego.
  Zwraca srukture wierzcholkow stosow.*/
struct stacks_of_books build_stack_from_file(struct stacks_of_books S, char *filename) {
    char buffer[100]; //bufor do przechowywania znakow z odczytanej linii
    char *book_title; //wskaznik do poczatku lancucha znakow z odczytanej linii
    FILE *file = fopen(filename, "r"); //otwarcie pliku
    //jesli nie udalo sie otworzyc pliku zostaje wypisany komunikat o bledzie
    if(file == NULL) {
        printf("Blad w trakcie otwierania pliku z danymi\n");
        exit(EXIT_FAILURE);
	}
	//jesli udalo sie otworzyc plik
	else {
        /*dopoki nie natrafimy na koniec pliku odczytujemy kolejne znaki z danej linii do bufora
          oraz poczatek odczytanego lancucha znakow z bufora umieszczamy we wskazniku "book_title"*/
        while (!feof(file)) {
            fgets(buffer, 100, file);
            /*Wzgledy estetyczne. Z racji dzialania funkcji fgets dla ostatniej odczytanej linii z pliku
              nie zostanie dodany znak konca linii. Wobec tego na koncu odczytanej linii dodajemy ten znak.*/
            if (feof(file)) {
                int end_of_line = (int) strlen(buffer);
                buffer[end_of_line] = '\n';
            }
            book_title = buffer;
            S = add_book(S, book_title); //dodanie odczytanej linii na stos ksiazek na stole
            for (int i = 0; i < 100; ++i) //czyszczenie bufora
                buffer[i] = '\0';
        }
    }
    fclose(file); //zamkniecie pliku
    return S;
}

/*Funkcja wyswietlajaca stos ksiazek. Jako parametr wejsciowy przyjmuje wskaznik na wierzchole stosu.*/
void print_stack(struct stack *stack) {
    if(stack == NULL)
        printf("Brak ksiazek.\n");
    else {
        int i = 1;
        while(stack != NULL) {
            printf("%3.d. ", i);
            fputs(stack->key, stdout);
            stack = stack->next;
            ++i;
        }
    }
}

int main(int argc, char *argv[]) {
    struct stacks_of_books S; //deklaracja struktury wierzcholkow stosow
    S.books_on_table = NULL; S.books_under_table = NULL;
    char *book_title; //wskaznik na poczatek danego tytulu ksiazki
    char buffer[100]; //bufor do przechowywania znakow z odczytanej linii
    int user_command; //zmienna oznaczajaca dana opcje wyboru
    int counter_remove_books = 0; //zmienna oznaczjaca ilosc ksiazek do usuniecia ze stosu
    char *filename = (char *) "libraryFile.txt"; //wskaznik do poczatku danego pliku wejsciowego

    S = build_stack_from_file(S, filename); //odczytanie tytulow ksiazek z pliku i umieszczenie ich na stosie

    while(1) {
        printf("\nOpcja wyboru (wprowadz odpowiednia cyfre): "
                       "\n 0-wyjscie\n 1-dodaj ksiazke na stol"
                       "\n 2-usun ksiazke ze stolu"
                       "\n 3-wyswietl ksiazki na stole"
                       "\n 4-print ksiazki zdjete ze stolu\n");

        printf("Wybor = ");
        scanf("%d", &user_command); //odczytanie danej opcji wyboru

        if (user_command == 0) {
            break;
        }

        system("cls"); //czyszczenie ekranu

        switch(user_command) {
            //odczytanie danego tytulu ksiazki podanego przez uzytkownika i umieszczenie go na stosie
            case 1: {
                printf("Podaj tytul ksiazki (bez polskich znakow):\n");
                fflush(stdin); //czyszczenie bufora
                book_title = fgets(buffer, 100, stdin);
                S = add_book(S, book_title);
            } break;

            //usuniecie wybranej liczby tytulow ze stosu ksiazek na stole oraz przelozenie ich na books_on_table ksiezek zdjetych ze stolu
            case 2: {
                printf("Podaj ile chcesz zdjac ksiazek:\n");
                scanf("%d", &counter_remove_books);
                while(counter_remove_books--)
                    S = remove_book(S);
            } break;

            //wyswietlenie tytulow ksiazek na stole
            case 3: {
                printf("Ksiazki na stole:\n");
                print_stack(S.books_on_table);
            } break;

            //wyswietlenie tytulow ksiazek zdjetych ze stolu
            case 4: {
                printf("Ksiazki zdjete ze stolu:\n");
                print_stack(S.books_under_table);
            } break;

            default:
                printf("Zla opcja wyboru. Wprowadz poprawna.\n");
            break;
        }
    }

    return 0;
}
