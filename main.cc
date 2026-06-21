#include <deal.II/base/point.h>
#include <optional>
#include <iostream>
#include <utility>
using namespace dealii;
using namespace std;
class Load {
public:
	Load(const dealii::Point<2>& position, double force_x, double force_y)
		: position(position), force_x(force_x), force_y(force_y) {
	}

	dealii::Point<2> position;
	double force_x;
	double force_y;
};

class StrNode {
public:
	StrNode(int id, const dealii::Point<2>& position)
		: id(id), position(position), loads() {
	}
	int id;
	dealii::Point<2> position;	
	std::vector<std::optional<Load>> loads;
	bool dx_constrained = false;
	bool dy_constrained = false;

	int LoadCount() const {
		return loads.size();
	}

	void NetLoad() {
		double net_load_x = 0.0;
		double net_load_y = 0.0;
		for (const auto& load : loads) {
			if (load.has_value()) {
				net_load_x += load->force_x;
				net_load_y += load->force_y;
			}
		}
		std::cout << "Net Load at Node " << id << ": (" << net_load_x << ", " << net_load_y << ")\n";
	}

	void AddLoad(const Load& load) {
		loads.push_back(load);
	}

	bool operator==(const StrNode& other) const {
		return this->position == other.position;
	}
};

class Line {
	public:
	Point<2> Start;
	Point<2> End;
		Line(const Point<2>& start, const Point<2>& end)
			: Start(start), End(end) {	}

		double Length() const {
			Tensor<1,2> diff = this->End - this->Start;
			double length = sqrt(diff[0]* diff[0] + diff[1] * diff[1]);	
			return length;
		}
};

class StrBar {

public:

	int id;
	StrNode  StartNode;
	StrNode  EndNode;
	double CrossSectionArea;
	double YoungsModulus;


	StrBar(int id, const StrNode& StartNode, const StrNode& EndNode)
			: id(id), StartNode(StartNode), EndNode(EndNode) {		}

	Line Geometry() const {
		return { StartNode.position, EndNode.position };
	}

	double Length() const {
		Line line = this->Geometry();	
		return line.Length();
	}
};

int main() {
	std::cout << "Structural Node Test\n";
	return 0;
}