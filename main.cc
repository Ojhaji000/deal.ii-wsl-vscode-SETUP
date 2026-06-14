#include <deal.II/base/point.h>
#include <optional>

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

int main() {
	std::cout << "Structural Node Test\n";
	return 0;
}