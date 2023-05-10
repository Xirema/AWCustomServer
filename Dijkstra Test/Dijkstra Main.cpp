#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<random>
#include<algorithm>
#include<numeric>
#include<boost/functional/hash.hpp>

#include<iostream>
#include<iomanip>
#include<fstream>

using Point = std::pair<int, int>;

template<>
struct std::hash<Point> {
	size_t operator()(Point const& p) const {
		size_t seed = 0;
		boost::hash_combine(seed, p.first);
		boost::hash_combine(seed, p.second);
		return seed;
	}
};

std::string to_string(std::u8string const& string) {
	std::string out;
	for (auto chr : string) {
		out.append(1, static_cast<char>(chr));
	}
	return out;
}

int main() {
	std::unordered_map<Point, float> grid;
	std::minstd_rand engine{ std::random_device{}()};
	std::uniform_real_distribution<float> dist{ 0, 100 };
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 50; j++) {
			Point p{ i, j };
			grid.emplace(p, dist(engine));
		}
	}

	for (int i : {2, 5, 8, 15, 24, 33, 42}) {
		int jstart, jend;
		if (i % 2 == 0) {
			jstart = 3; jend = 49;
		}
		else {
			jstart = 0; jend = 46;
		}
		for (int j = jstart; j <= jend; j++) {
			Point p{ i, j };
			grid[p] = std::numeric_limits<float>::infinity();
		}
	}

	Point start{ 0,0 };
	Point end{ 45,45 };

	std::unordered_set<Point> unvisited{};
	for (auto const& [p, _] : grid) {
		unvisited.insert(p);
	}

	std::unordered_map<Point, float> distances;
	for (auto const& [p, _] : grid) {
		if (p == start)
			distances.emplace(p, 0.f);
		else
			distances.emplace(p, std::numeric_limits<float>::infinity());
	}
	bool finished = false;
	Point current = start;
	while (!finished) {
		//if (current == end) {
		//	finished = true;
		//	continue;
		//}
		std::vector<Point> neighbors{
			Point{current.first + 1, current.second},
			Point{current.first - 1, current.second},
			Point{current.first, current.second + 1},
			Point{current.first, current.second - 1}
		};
		std::erase_if(neighbors,
			[&](Point const& p) {
				return
					grid.find(p) == grid.end() ||
					unvisited.find(p) == unvisited.end()
					;
			}
		);
		for (auto const& neighbor : neighbors) {
			distances.at(neighbor) = std::min(distances.at(neighbor), grid.at(neighbor) + distances.at(current));
		}
		unvisited.erase(current);
		if (unvisited.size() == 0) {
			finished = true;
			continue;
		}
		Point lowestDistance = *unvisited.begin();
		for (Point p : unvisited) {
			auto currentDistance = distances.at(lowestDistance);
			auto candidateDistance = distances.at(p);
			if (candidateDistance < currentDistance) {
				lowestDistance = p;
			}
		}
		current = lowestDistance;
		if (distances.at(current) == std::numeric_limits<float>::infinity()) {
			finished = true;
			continue;
		}
	}

	current = end;
	std::vector<Point> path{ current };
	while (current != start) {
		std::vector<Point> neighbors{
			Point{current.first + 1, current.second},
			Point{current.first - 1, current.second},
			Point{current.first, current.second + 1},
			Point{current.first, current.second - 1}
		};
		std::erase_if(neighbors,
			[&](Point const& p) {
				return
					grid.find(p) == grid.end() ||
					distances.at(p) == std::numeric_limits<float>::infinity() ||
					std::find(path.begin(), path.end(), p) != path.end();
					;
			}
		);
		if (neighbors.size() == 0) {
			std::cout << "There is no path between <" << start.first << ',' << start.second << "> and <" << end.first << ',' << end.second << ">.\n";
			break;
		}
		std::sort(neighbors.begin(), neighbors.end(), 
			[&distances](Point const& a, Point const& b) {
				return distances.at(a) < distances.at(b);
			}
		);
		current = neighbors.front();
		path.push_back(current);
	}

	auto asCharacter = [](float f) {
		std::u8string renderset = u8" .,-+=#@";
		std::u8string infinity = u8"█";
		if (f == std::numeric_limits<float>::infinity()) {
			return infinity;
		}
		f = std::round(f * (renderset.size() - 1) / 100);
		return std::u8string{ renderset[static_cast<size_t>(f)] };
	};
	std::ofstream out{ "out.txt" };
	out << "Grid: " << std::endl;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			out << to_string(asCharacter(grid.at(Point{ i, j })));
		}
		out << "\n";
	}
	out << "\nPath: " << std::endl;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (std::find(path.begin(), path.end(), Point{ i, j }) == path.end()) {
				out << ' ';
			}
			else {
				out << '+';
			}
		}
		out << "\n";
	}

	auto asString = [](float f) {
		if (f == std::numeric_limits<float>::infinity()) {
			return std::string("INF");
		}
		else {
			std::stringstream ss;
			ss << std::setprecision(0) << std::fixed << f;
			return ss.str();
		}
	};

	std::cout << "Costs: " << std::endl;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			std::cout << std::setw(5) << std::right << asString(grid.at(Point{ i, j }));
		}
		std::cout << "\n";
	}

	std::cout << "Distances: " << std::endl;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			std::cout << std::setw(5) << std::right << asString(distances.at(Point{ i, j }));
		}
		std::cout << "\n";
	}
}