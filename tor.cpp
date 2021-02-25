#include <set>
#include <gmsh.h>
#include <math.h>
#include <stdio.h>
#include <vector>

double lc = 1e-2;
double pi = 3.1415926535897932384626433832795;


int cnt_points = 1;
int cnt_lines = 1;
int cnt_surfs = 1;
int cnt_vols = 1;

int cnt_loops = 1;

int tor(double R, double r, double x, double y, double z) {
  gmsh::model::geo::addPoint(x, y, z, lc, cnt_points++);  // center
  int c_id = cnt_points - 1;
  
  
  
  int n = 8;
  
  for (int i = 0; i < n - 1; i++) {
    double c = cos(2 * pi * i / n);
    double s = sin(2 * pi * i / n);
    
    if (!i) {
      gmsh::model::geo::addPoint(x + R * c, y + R * s, z, lc, cnt_points++);
      gmsh::model::geo::addPoint(x + (R - r) * c, y + (R - r) * s, z, lc, cnt_points++);
      gmsh::model::geo::addPoint(x + (R + r) * c, y + (R + r) * s, z, lc, cnt_points++);
  
      gmsh::model::geo::addCircleArc(cnt_points - 2, cnt_points - 3, cnt_points - 1, cnt_lines++, s, -c, 0);  // 1
      gmsh::model::geo::addCircleArc(cnt_points - 2, cnt_points - 3, cnt_points - 1, cnt_lines++, -s, c, 0);  // 2
    }
    
    double c1 = cos(2 * pi * (i + 1) / n);
    double s1 = sin(2 * pi * (i + 1) / n);
    
       
    gmsh::model::geo::addPoint(x + R * c1, y + R * s1, z, lc, cnt_points++);
    gmsh::model::geo::addPoint(x + (R - r) * c1, y + (R - r) * s1, z, lc, cnt_points++);
    gmsh::model::geo::addPoint(x + (R + r) * c1, y + (R + r) * s1, z, lc, cnt_points++);
  
    gmsh::model::geo::addCircleArc(cnt_points - 2, cnt_points - 3, cnt_points - 1, cnt_lines++, s1, -c1, 0);  // 3
    gmsh::model::geo::addCircleArc(cnt_points - 2, cnt_points - 3, cnt_points - 1, cnt_lines++, -s1, c1, 0);  // 4
    
    
    gmsh::model::geo::addCircleArc(cnt_points - 5, c_id, cnt_points - 2, cnt_lines++, 0, 0, 1);  // 5
    gmsh::model::geo::addCircleArc(cnt_points - 4, c_id, cnt_points - 1, cnt_lines++, 0, 0, 1);  // 6
    
    
     if (!i) {
       gmsh::model::geo::addCurveLoop({cnt_lines - 6, cnt_lines - 1, -(cnt_lines - 4), -(cnt_lines - 2)}, cnt_loops++);
       gmsh::model::geo::addSurfaceFilling({cnt_loops - 1}, cnt_surfs++);
       
       gmsh::model::geo::addCurveLoop({cnt_lines - 5, cnt_lines - 1, -(cnt_lines - 3), -(cnt_lines - 2)}, cnt_loops++);
       gmsh::model::geo::addSurfaceFilling({cnt_loops - 1}, cnt_surfs++);
     }
     
     else {
       gmsh::model::geo::addCurveLoop({cnt_lines - 8, cnt_lines - 1, -(cnt_lines - 4), -(cnt_lines - 2)}, cnt_loops++);
       gmsh::model::geo::addSurfaceFilling({cnt_loops - 1}, cnt_surfs++);
       
       gmsh::model::geo::addCurveLoop({cnt_lines - 7, cnt_lines - 1, -(cnt_lines - 3), -(cnt_lines - 2)}, cnt_loops++);
       gmsh::model::geo::addSurfaceFilling({cnt_loops - 1}, cnt_surfs++);
       
       
     }
    
  }
  
  gmsh::model::geo::addCircleArc(cnt_points - 2, c_id, cnt_points - 3 * n + 1, cnt_lines++, 0, 0, 1);
  gmsh::model::geo::addCircleArc(cnt_points - 1, c_id, cnt_points - 3 * n + 2, cnt_lines++, 0, 0, 1);
  
  
  
  gmsh::model::geo::addCurveLoop({cnt_lines - 6, cnt_lines - 1, -(cnt_lines - 4 * n), -(cnt_lines - 2)}, cnt_loops++);
  gmsh::model::geo::addSurfaceFilling({cnt_loops - 1}, cnt_surfs++);
  
  gmsh::model::geo::addCurveLoop({cnt_lines - 5, cnt_lines - 1, -(cnt_lines - 4 * n + 1), -(cnt_lines - 2)}, cnt_loops++);
  gmsh::model::geo::addSurfaceFilling({cnt_loops - 1}, cnt_surfs++);
  
  
  std::vector<int> surs;
  
  for (int i = cnt_surfs - 2 * n; i < cnt_surfs; i++) {
    surs.push_back(i);
  }
  
  gmsh::model::geo::addSurfaceLoop(surs, cnt_loops++);
  
  
  return cnt_loops - 1;
}

int main() {
  gmsh::initialize();

  gmsh::model::add("t2");

  
  int t1 = tor(0.2, 0.07, 0, 0, 0);
 
  int t3 = tor(0.2, 0.05, 0, 0, 0);
  
  gmsh::model::geo::addVolume({t1, -t3}, cnt_vols++);
 
  gmsh::model::geo::synchronize();

  gmsh::model::mesh::generate(3);

  gmsh::write("t2.msh");

  gmsh::fltk::run();

  gmsh::finalize();

  return 0;
}

