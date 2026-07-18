#include <deal.II/base/point.h>
#include <optional>
#include <iostream>
#include <utility>
#include <ctime>
#include <iomanip>
#include <cantera/core.h>

using namespace dealii;
using namespace std;
using namespace Cantera;
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
void simple_demo()
{
	// Create a new Solution object
	auto sol = newSolution("h2o2.yaml");
	auto gas = sol->thermo();

	// Set the thermodynamic state by specifying T (500 K) P (2 atm) and the mole
	// fractions. Note that the mole fractions do not need to sum to 1.0 - they will
	// be normalized internally. Also, the values for any unspecified species will be
	// set to zero.
	gas->setState_TPX(500.0, 2.0 * OneAtm, "H2O:1.0, H2:8.0, AR:1.0");

	// Print a summary report of the state of the gas.
	std::cout << gas->report() << std::endl;
}

int main() {
	cout << "Value of __cplusplus"<<__cplusplus<<"\n";
	auto now = chrono::system_clock::now();
	time_t currentTime = chrono::system_clock::to_time_t(now);
	tm* localTime = localtime(&currentTime);
	cout<<"Current Date and Time: "
		<<put_time(localTime, "%Y-%m-%d %H:%M:%S %Z")
		<<"\n";

	std::cout << "Structural Node Test\n";


	try {
		simple_demo();
	}
	catch (CanteraError& err) {
		std::cout << err.what() << std::endl;
		return 1;
	}
	return 0;
}