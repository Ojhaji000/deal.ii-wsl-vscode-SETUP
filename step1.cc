#include <iostream>
#include <fstream>
#include <deal.II/grid/tria.h>
#include <deal.II/grid/grid_generator.h>
//#include <deal.II/grid/grid_out.h>
#include <deal.II/dofs/dof_tools.h>
#include <deal.II/fe/fe_q.h>
#include <deal.II/lac/dynamic_sparsity_pattern.h>
#include <deal.II/lac/sparsity_pattern.h>
#include <deal.II/fe/mapping_q1.h>


void make_grid(dealii::Triangulation<2> &triangulation)
{
  const dealii::Point<2> center(1, 0);
  const double           inner_radius = 0.5, outer_radius = 1.0;
  dealii::GridGenerator::hyper_shell(
	triangulation, center, inner_radius, outer_radius, 5);
  for (unsigned int step = 0; step < 3; ++step)
	{
	  for (const auto &cell : triangulation.active_cell_iterators())
		for (const auto v : cell->vertex_indices())
		  {
			const double distance_from_center =
			  center.distance(cell->vertex(v));
			if (std::fabs(distance_from_center - inner_radius) <=
				1e-6 * inner_radius)
			  {
				cell->set_refine_flag();
				break;
			  }
		  }
	  triangulation.execute_coarsening_and_refinement();
	}
}
void write_dof_locations(const dealii::DoFHandler<2>& dof_handler,
	const std::string& filename)
{
	const std::map<dealii::types::global_dof_index, dealii::Point<2>> dof_location_map =
		dealii::DoFTools::map_dofs_to_support_points(dealii::MappingQ1<2>(), dof_handler);
	std::ofstream dof_location_file(filename);
	dealii::DoFTools::write_gnuplot_dof_support_point_info(dof_location_file,
		dof_location_map);
}
void distribute_dofs(dealii::DoFHandler<2>& dof_handler)
{
	const dealii::FE_Q<2> finite_element(1);
	dof_handler.distribute_dofs(finite_element);
	write_dof_locations(dof_handler, "dof-locations-1.gnuplot");
	dealii::DynamicSparsityPattern dynamic_sparsity_pattern(dof_handler.n_dofs(),
		dof_handler.n_dofs());
	dealii::DoFTools::make_sparsity_pattern(dof_handler, dynamic_sparsity_pattern);
	dealii::SparsityPattern sparsity_pattern;
	sparsity_pattern.copy_from(dynamic_sparsity_pattern);
	std::ofstream out("sparsity-pattern-1.svg");
	sparsity_pattern.print_svg(out);
}
int main()
{
    std::cout << "Helloworld!" << std::endl;
	dealii::Triangulation<2> triangulation;
	make_grid(triangulation);

	dealii::DoFHandler<2> dof_handler(triangulation);

	distribute_dofs(dof_handler);
	std::ofstream mesh_file("mesh.gnuplot");
}
