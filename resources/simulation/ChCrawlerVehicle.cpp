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
#include <fstream>

#include "ChCrawlerVehicle.h"

#include "chrono_thirdparty/rapidjson/document.h"
#include "chrono_thirdparty/rapidjson/prettywriter.h"
#include "chrono_thirdparty/rapidjson/stringbuffer.h"

namespace chrono {
namespace vehicle {

ChCrawlerVehicle::ChCrawlerVehicle(const std::string &name, ChMaterialSurface::ContactMethod contact_method)
    : ChVehicle(name, contact_method) {}

ChCrawlerVehicle::ChCrawlerVehicle(const std::string &name, ChSystem *system) : ChVehicle(name, system) {}

// -----------------------------------------------------------------------------
// Initialize this vehicle at the specified global location and orientation.
// This base class implementation only initializes the chassis subsystem.
// Derived classes must extend this function to initialize all other wheeled
// vehicle subsystems (steering, suspensions, wheels, and driveline).
// -----------------------------------------------------------------------------
void ChCrawlerVehicle::Initialize(const ChCoordsys<> &chassisPos, double chassisFwdVel) {
  m_chassis->Initialize(m_system, chassisPos, chassisFwdVel, WheeledCollisionFamily::CHASSIS);
}

// -----------------------------------------------------------------------------
// Update the state of this vehicle at the current time.
// The vehicle system is provided the current driver inputs (throttle between
// 0 and 1, steering between -1 and +1), the torque
// from the powertrain, and tire forces (expressed in the global reference
// frame).
// The default implementation of this function invokes the update functions for
// all vehicle subsystems.
// -----------------------------------------------------------------------------
void ChCrawlerVehicle::Synchronize(double time,
                                   double steering,
                                   double powertrain_torque,
                                   const TerrainForces &tire_forces) {
  // Apply powertrain torque to the driveline's input shaft.
  m_driveline->Synchronize(powertrain_torque);

  // Let the steering subsystems process the steering input.
  for (unsigned int i = 0; i < m_steerings.size(); i++) {
    m_steerings[i]->Synchronize(time, steering);
  }

  // Apply tire forces to spindle bodies.
  for (unsigned int i = 0; i < m_suspensions.size(); i++) {
    m_suspensions[i]->Synchronize(LEFT, tire_forces[2 * i]);
    m_suspensions[i]->Synchronize(RIGHT, tire_forces[2 * i + 1]);
  }

  m_chassis->Synchronize(time);
}

// -----------------------------------------------------------------------------
// Set visualization type for the various subsystems
// -----------------------------------------------------------------------------
void ChCrawlerVehicle::SetSuspensionVisualizationType(VisualizationType vis) {
  for (size_t i = 0; i < m_suspensions.size(); ++i) {
    m_suspensions[i]->SetVisualizationType(vis);
  }
}

void ChCrawlerVehicle::SetSteeringVisualizationType(VisualizationType vis) {
  for (size_t i = 0; i < m_steerings.size(); ++i) {
    m_steerings[i]->SetVisualizationType(vis);
  }
}

void ChCrawlerVehicle::SetWheelVisualizationType(VisualizationType vis) {
  for (size_t i = 0; i < m_wheels.size(); ++i) {
    m_wheels[i]->SetVisualizationType(vis);
  }
}

// -----------------------------------------------------------------------------
// Enable/disable collision between the chassis and all other vehicle subsystems
// This only controls collisions between the chassis and the tire systems.
// -----------------------------------------------------------------------------
void ChCrawlerVehicle::SetChassisVehicleCollide(bool state) {
  if (state) {
    // Chassis collides with tires
    m_chassis->GetBody()->GetCollisionModel()->SetFamilyMaskDoCollisionWithFamily(WheeledCollisionFamily::TIRES);
  } else {
    // Chassis does not collide with tires
    m_chassis->GetBody()->GetCollisionModel()->SetFamilyMaskNoCollisionWithFamily(WheeledCollisionFamily::TIRES);
  }
}

// -----------------------------------------------------------------------------
// Enable/disable output from the various subsystems
// -----------------------------------------------------------------------------
void ChCrawlerVehicle::SetSuspensionOutput(int id, bool state) {
  m_suspensions[id]->SetOutput(state);
}

void ChCrawlerVehicle::SetSteeringOutput(int id, bool state) {
  m_steerings[id]->SetOutput(state);
}

void ChCrawlerVehicle::SetDrivelineOutput(bool state) {
  m_driveline->SetOutput(state);
}

// -----------------------------------------------------------------------------
// Calculate and return the total vehicle mass
// -----------------------------------------------------------------------------
double ChCrawlerVehicle::GetVehicleMass() const {
  double mass = m_chassis->GetMass();

  for (auto wheel : m_wheels) {
    mass += wheel->GetMass();
  }

  for (auto steering : m_steerings) {
    mass += steering->GetMass();
  }

  for (auto suspension : m_suspensions) {
    mass += suspension->GetMass();
  }

  return mass;
}

// -----------------------------------------------------------------------------
// Calculate and return the current vehicle COM location
// -----------------------------------------------------------------------------
ChVector<> ChCrawlerVehicle::GetVehicleCOMPos() const {
  ChVector<> com(0, 0, 0);
  com += m_chassis->GetMass() * m_chassis->GetCOMPos();

  for (auto wheel: m_wheels) {
    com += wheel->GetMass() * wheel->GetCOMPos();
  }

  for (auto steering : m_steerings) {
    com += steering->GetMass() * steering->GetCOMPos();
  }

  for (auto suspension : m_suspensions) {
    com += suspension->GetMass() * suspension->GetCOMPos();
  }

  return com / GetVehicleMass();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
std::shared_ptr<ChBody> ChCrawlerVehicle::GetWheelBody(const WheelID &wheel_id) const {
  return m_suspensions[wheel_id.axle()]->GetSpindle(wheel_id.side());
}

const ChVector<> &ChCrawlerVehicle::GetWheelPos(const WheelID &wheel_id) const {
  return m_suspensions[wheel_id.axle()]->GetSpindlePos(wheel_id.side());
}

const ChQuaternion<> &ChCrawlerVehicle::GetWheelRot(const WheelID &wheel_id) const {
  return m_suspensions[wheel_id.axle()]->GetSpindleRot(wheel_id.side());
}

const ChVector<> &ChCrawlerVehicle::GetWheelLinVel(const WheelID &wheel_id) const {
  return m_suspensions[wheel_id.axle()]->GetSpindleLinVel(wheel_id.side());
}

ChVector<> ChCrawlerVehicle::GetWheelAngVel(const WheelID &wheel_id) const {
  return m_suspensions[wheel_id.axle()]->GetSpindleAngVel(wheel_id.side());
}

double ChCrawlerVehicle::GetWheelOmega(const WheelID &wheel_id) const {
  return m_suspensions[wheel_id.axle()]->GetAxleSpeed(wheel_id.side());
}

// -----------------------------------------------------------------------------
// Return the complete state (expressed in the global frame) for the specified
// wheel body.
// -----------------------------------------------------------------------------
WheelState ChCrawlerVehicle::GetWheelState(const WheelID &wheel_id) const {
  WheelState state;

  state.pos = GetWheelPos(wheel_id);
  state.rot = GetWheelRot(wheel_id);
  state.lin_vel = GetWheelLinVel(wheel_id);
  state.ang_vel = GetWheelAngVel(wheel_id);

  ChVector<> ang_vel_loc = state.rot.RotateBack(state.ang_vel);
  state.omega = ang_vel_loc.y();

  return state;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
double ChCrawlerVehicle::GetDriveshaftSpeed() const {
  return m_driveline->GetDriveshaftSpeed();
}

// -----------------------------------------------------------------------------
// Estimate the maximum steering angle based on a bicycle model, from the vehicle
// minimum turning radius, the wheelbase, and the track of the front suspension.
// TODO: determine accurate max steering angle
// -----------------------------------------------------------------------------
double ChCrawlerVehicle::GetMaxSteeringAngle() const {
  return std::asin(GetWheelbase() / (GetMinTurningRadius() - 0.5 * GetWheeltrack(0)));
}

// -----------------------------------------------------------------------------
// Log constraint violations
// -----------------------------------------------------------------------------
void ChCrawlerVehicle::LogConstraintViolations() {
  GetLog().SetNumFormat("%16.4e");

  // Report constraint violations for the suspension joints
  for (size_t i = 0; i < m_suspensions.size(); i++) {
    GetLog() << "\n---- AXLE " << i << " LEFT side suspension constraint violations\n\n";
    m_suspensions[i]->LogConstraintViolations(LEFT);
    GetLog() << "\n---- AXLE " << i << " RIGHT side suspension constraint violations\n\n";
    m_suspensions[i]->LogConstraintViolations(RIGHT);
  }

  // Report constraint violations for the steering joints
  for (size_t i = 0; i < m_steerings.size(); i++) {
    GetLog() << "\n---- STEERING subsystem " << i << " constraint violations\n\n";
    m_steerings[i]->LogConstraintViolations();
  }

  GetLog().SetNumFormat("%g");
}

std::string ChCrawlerVehicle::ExportComponentList() const {
  rapidjson::Document jsonDocument;
  jsonDocument.SetObject();

  std::string template_name = GetTemplateName();
  jsonDocument.AddMember("name", rapidjson::StringRef(m_name.c_str()), jsonDocument.GetAllocator());
  jsonDocument.AddMember("template", rapidjson::Value(template_name.c_str(), jsonDocument.GetAllocator()).Move(),
                         jsonDocument.GetAllocator());

  {
    rapidjson::Document jsonSubDocument(&jsonDocument.GetAllocator());
    jsonSubDocument.SetObject();
    m_chassis->ExportComponentList(jsonSubDocument);
    jsonDocument.AddMember("chassis", jsonSubDocument, jsonDocument.GetAllocator());
  }

  rapidjson::Value suspArray(rapidjson::kArrayType);
  for (auto suspension : m_suspensions) {
    rapidjson::Document jsonSubDocument(&jsonDocument.GetAllocator());
    jsonSubDocument.SetObject();
    suspension->ExportComponentList(jsonSubDocument);
    suspArray.PushBack(jsonSubDocument, jsonDocument.GetAllocator());
  }
  jsonDocument.AddMember("suspension", suspArray, jsonDocument.GetAllocator());

  rapidjson::Value sterringArray(rapidjson::kArrayType);
  for (auto steering : m_steerings) {
    rapidjson::Document jsonSubDocument(&jsonDocument.GetAllocator());
    jsonSubDocument.SetObject();
    steering->ExportComponentList(jsonSubDocument);
    sterringArray.PushBack(jsonSubDocument, jsonDocument.GetAllocator());
  }
  jsonDocument.AddMember("steering", sterringArray, jsonDocument.GetAllocator());

  rapidjson::StringBuffer jsonBuffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> jsonWriter(jsonBuffer);
  jsonDocument.Accept(jsonWriter);

  return jsonBuffer.GetString();
}

void ChCrawlerVehicle::ExportComponentList(const std::string &filename) const {
  std::ofstream of(filename);
  of << ExportComponentList();
  of.close();
}

void ChCrawlerVehicle::Output(int frame, ChVehicleOutput &database) const {
  database.WriteTime(frame, m_system->GetChTime());

  if (m_chassis->OutputEnabled()) {
    database.WriteSection(m_chassis->GetName());
    m_chassis->Output(database);
  }

  for (auto suspension : m_suspensions) {
    if (suspension->OutputEnabled()) {
      database.WriteSection(suspension->GetName());
      suspension->Output(database);
    }
  }

  for (auto steering : m_steerings) {
    if (steering->OutputEnabled()) {
      database.WriteSection(steering->GetName());
      steering->Output(database);
    }
  }
}

} /// vehicle
} /// chrono