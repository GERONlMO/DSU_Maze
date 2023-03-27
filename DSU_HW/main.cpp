#include <iostream>
#include <vector>
#include <random>

class DSU {
private:
	int* data;
	int* rank;
	int size;
public:
	DSU(int size) {
		data = new int[size];
		rank = new int[size];
		this->size = size;
		for (; size > 0; size--, make(size));
	}

	void make(int x) {
		data[x] = x;
		rank[x] = 0;
	}

	int find(int x) { 
		if (data[x] == x) {
			return x;
		}
		else {
			return find(data[x]);
		}
	}

	void unite(int x, int y) {
		int parentX = find(x);
		int parentY = find(y);
		if (rank[x] > rank[y]) {
			data[parentY] = parentX;
		}
		else {
			data[parentX] = parentY;
			if (rank[x] == rank[y]) { 
				rank[y]++;
			}
		}
	}

	void print() {
		for (int i = 0; i < size; i++) { std::cout << '(' << data[i] << ')'; }
	}

	~DSU() {
		delete[] data;
		delete[] rank;
	}

};

class Maze {
private:
	DSU dsu;
	bool* right;
	bool* bottom;
public:
	Maze(int size) : dsu(size) {
		right = new bool[size];
		bottom = new bool[size];
	}

	bool notInOnePath(int x_wall, int y_wall) { return dsu.find(x_wall) != dsu.find(y_wall); }
	void continuePath(int x, int y) { dsu.unite(x, y); }
	int startPosition(int x) { return dsu.find(x); }

	bool rightWall(int x) { return right[x]; }
	bool bottomWall(int x) { return bottom[x]; }
	void destroyRightWall(int x) { right[x] = false; }
	void destroyBottomWall(int x) { bottom[x] = false; }

	~Maze() {
		delete[] right;
		delete[] bottom;
	}
};

int main() {
	srand(time(NULL));

	DSU dsu(7);

	dsu.print();
	std::cout << "Step: create" << std::endl;

	dsu.unite(3, 6);

	dsu.print();
	std::cout << "Step: unite(3, 6)" << std::endl;

	dsu.unite(5, 6);

	dsu.print();
	std::cout << "Step: unite(5, 6)" << std::endl;

	dsu.unite(3, 4);

	dsu.print();
	std::cout << "Step: unite(3, 4)" << std::endl;

	int mazeSize = 10;

	DSU mazeDSU(mazeSize * mazeSize);
	Maze maze(mazeSize * mazeSize);

	std::vector<int> walls;

	for (int i = 0; i <= mazeSize * mazeSize * 2 - 2; i++) {
		if ((i % (2 * mazeSize) == 0) || (i > mazeSize * (mazeSize - 1) * 2)) continue;
		walls.push_back(i);
	}

	while (walls.size() > 0) {

		int size = walls.size();
		int j = rand() % size;
		int x = (walls[j] - 1) / 2;

		if (walls[j] % 2 == 0) {
			if (maze.notInOnePath(x, x + 1)) {
				maze.continuePath(maze.startPosition(x), maze.startPosition(x + 1));
				maze.destroyRightWall(x);
			}
		}
		else {
			if (maze.notInOnePath(x, x + mazeSize)) {
				maze.continuePath(maze.startPosition(x), maze.startPosition(x + mazeSize));
				maze.destroyBottomWall(x);
			}
		}
		walls.erase(walls.begin() + j);
	}

	std::cout << "   ";
	for (int i = 1; i < mazeSize; i++) {
		std::cout << "___";
	}

	std::cout << '\n';
	for (int i = 0; i < mazeSize; i++) {
		std::cout << "|";
		for (int j = mazeSize * i; j < mazeSize + mazeSize * i; j++) {
			if (j == mazeSize * mazeSize - 1) {
				std::cout << "  |";
			}
			else if (maze.rightWall(j) && maze.bottomWall(j)) {
				std::cout << "__|";
			}
			else if (maze.rightWall(j)) {
				std::cout << "  |";
			}
			else if (maze.bottomWall(j)) {
				std::cout << "__ ";
			}
			else {
				std::cout << "   ";
			}
		}
		std::cout << "\n";
	}

	return 0;
}