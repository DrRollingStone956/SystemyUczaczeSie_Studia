#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>

// Struktura do przechowywania statystyk pojedynczego atrybutu
struct AttributeStats {
    std::map<std::string, int> value_counts; // zliczanie wystąpień każdej wartości
};

// Funkcja pomocnicza do dzielenia wiersza według podanych separatorów (np. spacja, przecinek, średnik)
std::vector<std::string> splitLine(const std::string& line, const std::string& delimiters = " \t,;") {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = line.find_first_of(delimiters);

    while (end != std::string::npos) {
        if (end > start) {
            tokens.push_back(line.substr(start, end - start));
        }
        start = end + 1;
        end = line.find_first_of(delimiters, start);
    }
    if (start < line.length()) {
        tokens.push_back(line.substr(start));
    }
    return tokens;
}

int main() {
    std::string filename = "dane.txt";
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Blad podczas otwierania pliku: " << filename << std::endl;
        return 1;
    }

    std::vector<std::vector<std::string>> decision_table;
    std::string line;

    // 1. Wczytanie danych z pliku tekstowego
    while (std::getline(file, line)) {
        if (line.empty()) continue; // Pomiń puste wiersze
        std::vector<std::string> row = splitLine(line);
        if (!row.empty()) {
            decision_table.push_back(row);
        }
    }
    file.close();

    if (decision_table.empty()) {
        std::cout << "Tabela decyzyjna jest pusta." << std::endl;
        return 0;
    }

    size_t num_rows = decision_table.size();
    size_t num_attributes = decision_table[0].size(); // Ostatni to atrybut decyzyjny

    std::vector<AttributeStats> stats(num_attributes);

    // Przetwarzanie i zbieranie statystyk dla każdego atrybutu
    for (const auto& row : decision_table) {
        for (size_t col = 0; col < num_attributes; ++col) {
            stats[col].value_counts[row[col]]++;
        }
    }

    // Wyświetlenie wczytanej tabeli
    std::cout << "=== Wczytana Tabela Decyzyjna (" << num_rows << " wierszy) ===" << std::endl;
    for (size_t i = 0; i < num_rows; ++i) {
        std::cout << "Obiekt " << i + 1 << ": ";
        for (size_t j = 0; j < num_attributes; ++j) {
            if (j == num_attributes - 1) {
                std::cout << "[Decyzja: " << decision_table[i][j] << "]";
            }
            else {
                std::cout << decision_table[i][j] << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "\n============================================\n" << std::endl;

    // 2 & 3. Obliczenie unikalnych wartości oraz ich wystąpień
    for (size_t col = 0; col < num_attributes; ++col) {
        if (col == num_attributes - 1) {
            std::cout << "--- Atrybut Decyzyjy (d) ---" << std::endl;
        }
        else {
            std::cout << "--- Atrybut a" << col + 1 << " ---" << std::endl;
        }

        // Liczba możliwych (unikalnych) wartości
        std::cout << "Liczba unikalnych wartosci: " << stats[col].value_counts.size() << std::endl;

        // Wystąpienia każdej wartości
        std::cout << "Wystapienia poszczegolnych wartosci:" << std::endl;
        for (const auto& [value, count] : stats[col].value_counts) {
            std::cout << "  Wartosc '" << value << "': " << count << " razy" << std::endl;
        }
        std::cout << std::endl;
    }

    return 0;
}