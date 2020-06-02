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

#ifndef CHRONO_CRAWLER_H
#define CHRONO_CRAWLER_H

#include <array>
#include <string>

#include "chrono_models/ChApiModels.h"
#include "Crawler_vehicle.h"
#include "Crawler_SimpleFluidPowertrain.h"
#include "Crawler_ArchmidesTire.h"

namespace chrono {
namespace vehicle {
namespace crawler {

class CH_MODELS_API Crawler {
 public:
  Crawler();
  Crawler(ChSystem *system);

  ~Crawler();

  void SetContactMethod(ChMaterialSurface::ContactMethod val) { m_contactMethod = val; }

  void SetChassisFixed(bool val) { m_fixed = true; }
  void SetChassisCollisionType(ChassisCollisionType val) { m_chassisCollisionType = val; }

  void SetTireType(TireModelType val) { m_tireType = val; }

  void SetInitPosition(const ChCoordsys<> &pos) { m_initPos = pos; }
  void SetInitFwdVel(double fwdVel) { m_initFwdVel = fwdVel; }
  void SetInitWheelAngVel(const std::vector<double> &omega) { m_initOmega = omega; }

  void SetVehicleStepSize(double step_size) { m_vehicle_step_size = step_size; }
  void SetTireStepSize(double step_size) { m_tire_step_size = step_size; }

  ChSystem *GetSystem() const { return m_vehicle->GetSystem(); }
  ChCrawlerVehicle &GetVehicle() const { return *m_vehicle; }
  std::shared_ptr<ChChassis> GetChassis() const { return m_vehicle->GetChassis(); }
  std::shared_ptr<ChBodyAuxRef> GetChassisBody() const { return m_vehicle->GetChassisBody(); }
  ChPowertrain &GetPowertrain() const { return *m_powertrain; }
  ChTire *GetTire(WheelID which) const { return m_tires[which.id()]; }
  double GetTotalMass() const;

  void Initialize();

//  void LockAxleDifferential(int axle, bool lock) { m_vehicle->LockAxleDifferential(axle, lock); }

  void SetAerodynamicDrag(double Cd, double area, double water_density);

  void SetChassisVisualizationType(VisualizationType vis) { m_vehicle->SetChassisVisualizationType(vis); }
  void SetSuspensionVisualizationType(VisualizationType vis) { m_vehicle->SetSuspensionVisualizationType(vis); }
  void SetSteeringVisualizationType(VisualizationType vis) { m_vehicle->SetSteeringVisualizationType(vis); }
  void SetWheelVisualizationType(VisualizationType vis) { m_vehicle->SetWheelVisualizationType(vis); }
  void SetTireVisualizationType(VisualizationType vis);

  void Synchronize(double time,
                   double steering_input,
                   double throttle_input,
                   const ChTerrain &terrain);

  void Advance(double step);

  void LogHardpointLocations() { m_vehicle->LogHardpointLocations(); }
  void DebugLog(int what) { m_vehicle->DebugLog(what); }

 protected:
  ChMaterialSurface::ContactMethod m_contactMethod;
  ChassisCollisionType m_chassisCollisionType;
  bool m_fixed;

  TireModelType m_tireType;

  double m_vehicle_step_size;
  double m_tire_step_size;

  ChCoordsys<> m_initPos;
  double m_initFwdVel;
  std::vector<double> m_initOmega;

  bool m_apply_drag;
  double m_Cd;
  double m_area;
  double m_water_density;

  ChSystem *m_system;
  Crawler_vehicle *m_vehicle;
  ChPowertrain *m_powertrain;
  std::array<ChTire *, 2> m_tires;

  double m_tire_mass;
};

}
}
}

#endif //CHRONO_CRAWLER_H
