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

#include "chrono/ChConfig.h"

#include "chrono_vehicle/ChVehicleModelData.h"

#include "Crawler.h"

namespace chrono {
namespace vehicle {
namespace crawler {

// -----------------------------------------------------------------------------
Crawler::Crawler()
    : m_system(NULL),
      m_vehicle(NULL),
      m_powertrain(NULL),
      m_tires({{NULL, NULL}}),
      m_contactMethod(ChMaterialSurface::NSC),
      m_chassisCollisionType(ChassisCollisionType::NONE),
      m_fixed(false),
      m_tireType(TireModelType::RIGID),
      m_vehicle_step_size(-1),
      m_tire_step_size(-1),
      m_initFwdVel(0),
      m_initPos(ChCoordsys<>(ChVector<>(0, 0, 1), QUNIT)),
      m_initOmega({0, 0, 0, 0}),
      m_apply_drag(false) {}

Crawler::Crawler(ChSystem *system)
    : m_system(system),
      m_vehicle(NULL),
      m_powertrain(NULL),
      m_tires({{NULL, NULL}}),
      m_contactMethod(ChMaterialSurface::NSC),
      m_chassisCollisionType(ChassisCollisionType::NONE),
      m_fixed(false),
      m_tireType(TireModelType::RIGID),
      m_vehicle_step_size(-1),
      m_tire_step_size(-1),
      m_initFwdVel(0),
      m_initPos(ChCoordsys<>(ChVector<>(0, 0, 1), QUNIT)),
      m_initOmega({0, 0, 0, 0}),
      m_apply_drag(false) {}

Crawler::~Crawler() {
  delete m_vehicle;
  delete m_powertrain;
  delete m_tires[0];
  delete m_tires[1];
}

void Crawler::SetAerodynamicDrag(double Cd, double area, double water_density) {
  m_Cd = Cd;
  m_area = area;
  m_water_density = water_density;

  m_apply_drag = true;
}

void Crawler::Initialize() {
  // Create and initialize the crawler vehicle
  m_vehicle = m_system ? new Crawler_vehicle(m_system, m_fixed, m_chassisCollisionType)
                       : new Crawler_vehicle(m_fixed, m_contactMethod, m_chassisCollisionType);

  m_vehicle->SetInitWheelAngVel(m_initOmega);
  m_vehicle->Initialize(m_initPos, m_initFwdVel);

  if (m_vehicle_step_size > 0) {
    m_vehicle->SetStepsize(m_vehicle_step_size);
  }

  // If specified, enable aerodynamic drag
  if (m_apply_drag) {
    m_vehicle->GetChassis()->SetAerodynamicDrag(m_Cd, m_area, m_water_density);
  }

  // Create and initialize the powertrain system
  m_powertrain
  new Crawler_SimpleFluidPowertrain('Powertrain');
  m_powertrain->Initialize(GetChassisBody(), m_vehicle->GetDriveshaft());

  // Create the Archimedes tires and set parameters depending on type
  switch (m_tireType) {
    case TireModelType::RIGID: {
      GetLog() << "Init RIGID" << "\n";
      bool use_mesh = (m_tireType == TireModelType::RIGID_MESH);
      Crawler_ArchmidesTire *tire_L = new Crawler_ArchmidesTire("L", use_mesh);
      Crawler_ArchmidesTire *tire_R = new Crawler_ArchmidesTire("R", use_mesh);

      m_tires[0] = tire_L;
      m_tires[1] = tire_R;

      break;
    }
    default: {
      break;
    }
  }

  // Initialize the tires
  m_tires[0]->Initialize(m_vehicle->GetWheelBody(FRONT_LEFT), LEFT);
  m_tires[1]->Initialize(m_vehicle->GetWheelBody(FRONT_RIGHT), RIGHT);

  m_tire_mass = m_tires[0]->ReportMass();
}

void Crawler::SetTireVisualizationType(VisualizationType vis) {
  for (auto Tire : m_tires) {
    Tire->SetVisualizationType(vis);
  }
}

void Crawler::Synchronize(double time,
                          double steering_input,
                          double throttle_input,
                          const ChTerrain &terrain) {
  TerrainForces tire_forces(2);
  WheelState wheel_states[2];

  tire_forces[0] = m_tires[0]->GetTireForce();
  tire_forces[1] = m_tires[1]->GetTireForce();

  wheel_states[0] = m_vehicle->GetWheelState(FRONT_LEFT);
  wheel_states[1] = m_vehicle->GetWheelState(FRONT_RIGHT);

  double powertrain_torque = m_powertrain->GetOutputTorque();

  double driveshaft_speed = m_vehicle->GetDriveshaftSpeed();

  m_tires[0]->Synchronize(time, wheel_states[0], terrain);
  m_tires[1]->Synchronize(time, wheel_states[1], terrain);

  m_powertrain->Synchronize(time, throttle_input, driveshaft_speed);

  m_vehicle->Synchronize(time, steering_input, powertrain_torque, tire_forces);
}

void Crawler::Advance(double step) {
  for (auto Tire : m_tires) {
    Tire->Advance(step);
  }

  m_powertrain->Advance(step);

  m_vehicle->Advance(step);
}

double Crawler::GetTotalMass() const {
  return m_vehicle->GetVehicleMass() + 2 * m_tire_mass;
}

} /// crawler
} /// vehicle
} /// chrono