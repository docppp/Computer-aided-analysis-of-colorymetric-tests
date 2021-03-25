# Computer-aided-analysis-of-colorymetric-tests
Qt app

## Cel

Celem pracy jest stworzenie desktopowej aplikacji działającej na systemie operacyjnym Windows, wspomagającej analizę przeprowadzonych testów kolorymetrycznych w warunkach pozalaboratoryjnych i próbie automatycznego określenia składu badanej próbki. Dzięki porównaniu zmiany zabarwienia odczynnika z bazą wzorcowych reakcji ponad 500 substancji, będzie można je uporządkować od najbardziej do najmniej prawdopodobnych. Użytkownik dostanie do wyboru 13 różnych testów.

Aby wyeliminować problem z różnym formatem danych, pobór barwy danej reakcji będzie możliwy z każdego piksela na ekranie. Dodatkowo do dyspozycji będzie dostępny próbnik kolorów RGB, z możliwością wyboru przez użytkownika dowolnego koloru.

Analizowany gradient będzie składał się z trzech części - barwy samego odczynnika, barwy końcowej i ewentualnie barwy przejściowej. W przypadku, gdy użytkownik nie wybierze barwy przejściowej, brana będzie średnia z pierwszej i ostatniej.

Dodatkowymi funkcjonalnościami będą podgląd znanych reakcji dla każdego odczynnika oraz możliwość edycji bazy danych. Dzięki temu będzie można dodawać własne, wcześniej nie określone wzorcowe zmiany barwy dla danej substancji i danego odczynnika.

## Baza

Baza danych zawiera ustandaryzowane kolory reakcji ponad 500 substancji z 13 odczynnikami w formacie CSV (comma separated values). Każdy gradient składa się z dwóch barw - przejściowej i finalnej, co w połączeniu ze startowym kolorem odczynnika daje trzy wartości do porównywania. Większość danych pochodzi z europejskiego projektu Response\cite{response} Nie są one jednak kompletne.

## Implementacja

Podstawową klasą programu jest klasa modelująca kolor, zawierającą trzy składowe barwy w przestrzeni RGB, a także odpowiadający im napis HEX.\\
```\begin{lstlisting}[language=C++]
class Color
{
public:
    std::string name;
    int R, G, B;
};
\end{lstlisting}
```

Klasa modelująca substancję składa się z nazwy, tablicy 26 kolorów opisującej wzorcowe reakcje z odczynnikami kolorymetrycznymi, po dwie na każdy (barwa końcowa i pośrednia) oraz wartości sumarycznego błędu.\\
 ```\begin{lstlisting}[language=C++]
 >class Drug
 >{
 >public:
 >   std::string name;
 >   Color test[NUMBER_OF_TESTS * 2]; // NUMBER_OF_TESTS = 13
>    double error;
>};
>\end{lstlisting}
```

Po każdorazowym zatwierdzeniu gradientu przeprowadzonej reakcji z odpowiednim odczynnikiem, program porównuje go z wzorcowym gradientem każdej substancji w dwóch punktach - końcowym i środkowym. Następnie z wektora porównywanych związków, usuwane są te, których błąd przekroczył wartość krytyczną.
```>\begin{lstlisting}[language=C++]
>void CheckErrors(const Color &sample1, const Color &sample2, int testFlag)
>{
>    std::vector<Drug *>::iterator it = DrugSorted.begin();
>    for (; it != DrugSorted.end(); ++it)
>    {
>        Color ref1 = (*it) -> test[testFlag * 2];
>        Color ref2 = (*it) -> test[testFlag * 2 + 1];
>        (*it) -> error += CopmareTests(ref1, ref2, sample1, sample2, testFlag);
>    }
>    std::sort(DrugSorted.begin(), DrugSorted.end(), DrugCompare);
>    while (DrugSorted[DrugSorted.size() - 1] -> error > ERROR_THRESHOLD)
>        DrugSorted.pop_back();
>}
>\end{lstlisting}
```
### Interfejs
Główne okno aplikacji podzielone jest na dwie części. Pierwsza odpowiedzialna jest za badanie nieznanych substancji. Druga umożliwia podgląd wzorcowych reakcji. Jest to przydatne w celu weryfikacji otrzymanych wyników, a także przy konieczności ewentualnego porównania gradientów różnych związków.\\
```>\begin{figure}[H]
>    \centering
>    \includegraphics[width=\textwidth]{test.jpg}
>    \caption{Pierwsza część głównego okna aplikacji}
>\end{figure}
>\begin{figure}[h]
>    \centering
>    \includegraphics[width=\textwidth]{picker.jpg}
>    \caption{Próbnik kolorów}
>\end{figure}
```
Wybór koloru możliwy jest na dwa sposoby - z piksela na ekranie oraz z wykorzystaniem próbnika kolorów, w którym poza możliwością wyboru dowolnej barwy, zapisane są te najczęściej występujące. Dzięki temu doświadczony użytkownik będzie mógł zdecydować czy zdjęcie w zadowalający sposób oddaje przebieg reakcji, czy też lepiej jest skorzystać z próbnika.\\

Druga część aplikacji to zbiór wszystkich znanych substancji wraz z kolorowym zapisem reakcji z każdym z testów. W przypadku gdy gradient nie jest znany, czarny prostokąt zasłania odpowiednie pole.\\

Dodatkowe okno nad bazą umożliwia wyszukiwanie substancji po nazwie. Z racji tego, że nazwy niektórych są skomplikowane, zdecydowano się na zastosowanie dwóch sposobów szukania. Pierwszy to proste sprawdzanie, czy nazwa danego związku zawiera w sobie szukaną frazę. Drugi to algorytm Damerau-Levenshteina, który polega na określaniu odległości dwóch napisów. Obliczanie tej wielkości polega na sprawdzeniu, ile operacji prostych należy wykonać na jednym ciągu znaków, aby otrzymać drugi. Operacje proste to wstawianie znaku, usuwanie znaku, zamiana znaku na inny oraz przestawienie kolejnością dwóch sąsiednich znaków. Jeżeli odległość Damerau-Levenshteina szukanej frazy i nazwy związku  $\leq 2$, substancja będzie dodana do listy.\\
