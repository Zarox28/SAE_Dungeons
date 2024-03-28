#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <random>

using namespace std;

// Structure représentant une cellule dans la grille
struct Cell {
    int row, col;
    bool visited;
    Cell(int r, int c) : row(r), col(c), visited(false) {}
};

class MazeGenerator {
private:
    int rows, cols;
    vector<vector<bool>> horizontal_walls;
    vector<vector<bool>> vertical_walls;

public:
    MazeGenerator(int r, int c) : rows(r), cols(c) {
        // Initialisation de la grille avec tous les murs
        horizontal_walls.assign(rows + 1, vector<bool>(cols, true));
        vertical_walls.assign(rows, vector<bool>(cols + 1, true));
    }

    // Fonction pour obtenir les voisins non visités d'une cellule
    vector<Cell> getUnvisitedNeighbors(Cell cell) {
        vector<Cell> neighbors;
        int row = cell.row, col = cell.col;
        if (row > 0 && !getCell(row - 1, col).visited) {
            neighbors.push_back(getCell(row - 1, col));
        }
        if (row < rows - 1 && !getCell(row + 1, col).visited) {
            neighbors.push_back(getCell(row + 1, col));
        }
        if (col > 0 && !getCell(row, col - 1).visited) {
            neighbors.push_back(getCell(row, col - 1));
        }
        if (col < cols - 1 && !getCell(row, col + 1).visited) {
            neighbors.push_back(getCell(row, col + 1));
        }
        return neighbors;
    }

    // Fonction pour mélanger aléatoirement un vecteur
    void shuffle(vector<Cell>& vec) {
        random_shuffle(vec.begin(), vec.end());
    }

    // Fonction pour générer le labyrinthe
    void generateMaze(Cell startCell) {
        stack<Cell> cellStack;
        startCell.visited = true;
        cellStack.push(startCell);

        while (!cellStack.empty()) {
            Cell currentCell = cellStack.top();
            cellStack.pop();

            vector<Cell> neighbors = getUnvisitedNeighbors(currentCell);
            shuffle(neighbors);

            for (auto& neighbor : neighbors) {
                // Supprimer le mur entre la cellule actuelle et la cellule voisine
                if (neighbor.row == currentCell.row - 1) {
                    horizontal_walls[currentCell.row][currentCell.col] = false;
                } else if (neighbor.row == currentCell.row + 1) {
                    horizontal_walls[currentCell.row + 1][currentCell.col] = false;
                } else if (neighbor.col == currentCell.col - 1) {
                    vertical_walls[currentCell.row][currentCell.col] = false;
                } else if (neighbor.col == currentCell.col + 1) {
                    vertical_walls[currentCell.row][currentCell.col + 1] = false;
                }

                neighbor.visited = true;
                cellStack.push(neighbor);
            }
        }
    }

    // Fonction pour récupérer une cellule de la grille
    Cell getCell(int row, int col) {
        return Cell(row, col);
    }

    // Fonction pour afficher le labyrinthe généré
    void printMaze() {
        for (int i = 0; i < rows; ++i) {
            // Afficher les murs horizontaux de la ligne i
            for (int j = 0; j < cols; ++j) {
                cout << "+";
                cout << (horizontal_walls[i][j] ? "---" : "   ");
            }
            cout << "+" << endl;

            // Afficher les murs verticaux et les coins de la ligne i
            for (int j = 0; j < cols; ++j) {
                cout << (vertical_walls[i][j] ? "|" : " ");
                cout << "   ";
            }
            cout << "|" << endl;
        }

        // Afficher la dernière ligne des murs horizontaux et le coin inférieur droit
        for (int j = 0; j < cols; ++j) {
            cout << "+---";
        }
        cout << "+" << endl;
    }
};

int main() {
    int rows, cols;
    cout << "Entrez le nombre de lignes du labyrinthe : ";
    cin >> rows;
    cout << "Entrez le nombre de colonnes du labyrinthe : ";
    cin >> cols;

    MazeGenerator maze(rows, cols);
    maze.generateMaze(maze.getCell(0, 0));
    maze.printMaze();

    return 0;
}
