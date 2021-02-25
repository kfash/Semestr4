#include <set>
#include <cmath>
#include <gmsh.h>

int main()
{
  gmsh::initialize();

  gmsh::model::add("t13");

  try {
    gmsh::merge("H4-016.STL");
  } catch(...) {
    gmsh::logger::write("Could not load STL mesh: bye!");
    gmsh::finalize();
    return 0;
  }


  double angle = 40;

  bool forceParametrizablePatches = false;

  bool includeBoundary = true;

  double curveAngle = 180;

  gmsh::model::mesh::classifySurfaces(angle * M_PI / 180., includeBoundary, forceParametrizablePatches);

  gmsh::model::mesh::createGeometry();

  gmsh::model::geo::synchronize();

  int f = gmsh::model::mesh::field::add("MathEval");

  gmsh::model::mesh::field::setString(f, "F", "4");
  gmsh::model::mesh::field::setAsBackgroundMesh(f);

  gmsh::model::mesh::generate(3);

  gmsh::fltk::run();

  gmsh::finalize();
  return 0;
}
