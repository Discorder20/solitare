# Pasjans (Gigathon 3 Edycja, 16-18)

## Podsumowanie projektu

**Pasjans (Gigathon 3 Edycja, 16-18)** to konsolowa gra karciana typu Solitaire napisana w języku C++. Gra umożliwia rozgrywkę w klasycznego pasjansa z obsługą zapisu, wczytywania, cofania ruchów oraz wyborem poziomu trudności. Interfejs oparty jest na sterowaniu klawiaturą, a całość działa w terminalu.

Zarówno kod jak i komentarze są w języku angielskim.
Ze względu na dużą ilość wypisywań do konsoli projekt polecam uruchomić pod powershell zamiast zwykłego CMD, znacząco poprawi to wydajność

### Główne funkcjonalności

- Rozgrywka w klasycznego pasjansa (Solitaire)
- Obsługa zapisu i wczytywania stanu gry
- Cofanie ruchów (undo)
- Tryby trudności (łatwy/trudny)
- Tablica wyników (rekordy)
- Kolorowe oznaczenia kart i kursora
- Intuicyjne sterowanie klawiaturą (WASD, Enter, Esc, itp.)

### Struktura i klasy

- **Engine** – główna klasa zarządzająca stronami (ekranami) gry, obsługuje pętlę gry, przełączanie widoków i globalne skróty klawiszowe.
- **Page** – klasa bazowa dla wszystkich ekranów (menu, gra, rekordy, ustawienia, instrukcja). Każda strona implementuje metody `start()`, `render()`, `handleKey(char key)`.
- **Game** – klasa odpowiadająca za logikę rozgrywki, obsługę ruchów, zaznaczeń, cofania, sprawdzania wygranej, itp.
- **Card, CardStack, CardContainer** – klasy reprezentujące pojedyncze karty, stosy kart oraz kontenery na stosy.
- **JsonHandler** – obsługa zapisu/wczytywania stanu gry i rekordów do plików JSON (wykorzystuje bibliotekę nlohmann/json).

#### Stosy kart (stacki)

- **Regular Stack** – główny obszar gry, 7 kolumn, na których układane są karty w kolejności naprzemiennych kolorów malejąco.
- **Build Stack** – 4 stosy docelowe, na które należy przenieść wszystkie karty danego koloru rosnąco (od Asa do Króla).
- **Extras Stack** – stosy kart do dobierania (stock).

### Sterowanie

- **W, S, A, D** – poruszanie się po interfejsie i planszy (niezależnie od wielkości liter)
- **Enter** – zatwierdzenie wyboru (np. wybór karty, ruch, menu)
- **Esc** – powrót do menu lub wyjście z bieżącego ekranu
- **C** – anulowanie zaznaczenia karty
- **Z** – cofnięcie ostatniego ruchu
- W menu: poruszanie się W/S, wybór Enter
- W grze: wybierz kartę i miejsce docelowe, zatwierdź Enter

Kolory kart:
- Czerwona/biała – zwykła karta
- Niebieska – aktualne położenie kursora
- Żółta – zaznaczona karta

---

## Uruchomienie projektu w Visual Studio Code

1. **Zainstaluj wymagane narzędzia:**
   - Kompilator C++ (np. MinGW lub MSVC)
   - [CMake](https://cmake.org/)
   - [Visual Studio Code](https://code.visualstudio.com/)
   - Rozszerzenia do VS Code: C/C++ (Microsoft), CMake Tools

2. **Otwórz folder projektu w VS Code:**
   - Wybierz `Plik` → `Otwórz folder...` i wskaż katalog z projektem.

3. **Zainicjuj projekt CMake:**
   - Jeśli pojawi się komunikat o wykryciu pliku `CMakeLists.txt`, kliknij „Configure Project”.
   - Wybierz odpowiedni kit (np. MinGW lub MSVC).

4. **Zbuduj projekt:**
   - Użyj polecenia `CMake: Build` z palety poleceń (Ctrl+Shift+P) lub przycisku „Build” na pasku CMake.

5. **Uruchom grę:**
   - Użyj polecenia `CMake: Run` lub uruchom plik wykonywalny `solitare.exe` z katalogu `build`.

### Uwaga dotycząca biblioteki JSON

Projekt korzysta z biblioteki [nlohmann/json](https://github.com/nlohmann/json) do obsługi plików JSON (zapis/wczytywanie stanu gry i rekordów). Biblioteka znajduje się w folderze `lib/json` i jest automatycznie dołączana przez CMake.

---

### Autor

Michał Borzuchowski