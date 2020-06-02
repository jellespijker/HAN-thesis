// MIT License
//
// Copyright (c) 2019 Jelle Spijker
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "chrono/assets/ChTriangleMeshShape.h"
#include "chrono/utils/ChUtilsInputOutput.h"

#include "chrono_vehicle/ChVehicleModelData.h"

#include "Crawler_Chassis.h"

namespace chrono {
namespace vehicle {
namespace crawler {

// -----------------------------------------------------------------------------
// Static variables
// -----------------------------------------------------------------------------
const double Crawler_Chassis::m_mass = 1250; //Todo: actual mass of chassis
const ChVector<> Crawler_Chassis::m_inertiaXX(222.8, 944.1, 1053.5); // Todo: actual inertia XX
const ChVector<> Crawler_Chassis::m_inertiaXY(0, 0, 0); // Todo: actual inertia XY
const ChVector<> Crawler_Chassis::m_COM_loc(0, 0, 0.2); // Todo: actual Center of Mass

Crawler_Chassis::Crawler_Chassis(const std::string &name, bool fixed, ChassisCollisionType chassis_collision_type)
    : ChRigidChassis(name, fixed) {
  m_inertia.SetElement(0, 0, m_inertiaXX.x());
  m_inertia.SetElement(1, 1, m_inertiaXX.y());
  m_inertia.SetElement(2, 2, m_inertiaXX.z());

  m_inertia.SetElement(0, 1, m_inertiaXY.x());
  m_inertia.SetElement(0, 2, m_inertiaXY.y());
  m_inertia.SetElement(1, 2, m_inertiaXY.z());
  m_inertia.SetElement(1, 0, m_inertiaXY.x());
  m_inertia.SetElement(2, 0, m_inertiaXY.y());
  m_inertia.SetElement(2, 1, m_inertiaXY.z());

  //// TODO: A more appropriate contact shape from primitives
  BoxShape box1(ChVector<>(0.0, 0.0, 0.1), ChQuaternion<>(1, 0, 0, 0), ChVector<>(1.0, 0.5, 0.2));

  m_has_primitives = true;
  m_vis_boxes.push_back(box1);

  m_has_mesh = true;
  m_vis_mesh_name = "crawler_chassis_POV_geom";
  m_vis_mesh_file = "sedan/sedan_chassis_vis.obj"; // Todo: add correct mesh

  m_has_collision = (chassis_collision_type != ChassisCollisionType::NONE);
  switch (chassis_collision_type) {
    case ChassisCollisionType::PRIMITIVES:
      m_coll_boxes.push_back(box1);
      break;
    case ChassisCollisionType::MESH:
      m_coll_mesh_names.push_back("sedan/sedan_chassis_col.obj"); // Todo: add mesh of crawler chassis
      break;
    default:
      break;
  }
}

} /// crawler
} /// vehicle
} /// chrono
