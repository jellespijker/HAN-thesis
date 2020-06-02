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

#ifndef CHRONO_CRAWLER_VEHICLE_H
#define CHRONO_CRAWLER_VEHICLE_H

#include <vector>

#include "chrono/core/ChCoordsys.h"
#include "chrono/physics/ChMaterialSurface.h"
#include "chrono/physics/ChSystem.h"

#include "ChCrawlerVehicle.h"

#include "chrono_models/ChApiModels.h"
#include "chrono_models/vehicle/ChVehicleModelDefs.h"

#include "Crawler_Chassis.h"
#include "Crawler_Driveline.h"

namespace chrono {
namespace vehicle {
namespace crawler {
class CH_MODELS_API Crawler_vehicle : public ChCrawlerVehicle {
 public:
  Crawler_vehicle(const bool fixed = false,
                  ChMaterialSurface::ContactMethod contact_method = ChMaterialSurface::NSC,
                  ChassisCollisionType chassis_collision_type = ChassisCollisionType::NONE);

  Crawler_vehicle(ChSystem *system,
                  const bool fixed = false,
                  ChassisCollisionType chassis_collision_type = ChassisCollisionType::NONE);

  ~Crawler_vehicle();

  virtual int GetNumberAxles() const override { return 2; } //TODO: check if it should be 1

  virtual double GetWheelbase() const override { return 3.0; } //TODO: What is the actual wheel base for the crawler?

  virtual double GetMinTurningRadius() const override { return 2.0; } //TODO: What is the actual min turning radius?

  double GetMaxSteeringAngle() const override { return 25.0 * CH_C_DEG_TO_RAD; } //TODO: check actual value

  void SetInitWheelAngVel(const std::vector<double> &omega) {
    assert(omega.size() == 2);
    m_omega = omega;
  }

  virtual void Initialize(const ChCoordsys<> &chassisPos, double chassisFwdVel = 0) override;

  // Log debugging information
  void LogHardpointLocations(); /// suspension hardpoints at design
  void DebugLog(int what); /// forces and lengths, contsrainets, etc.

 private:
  void Create(bool fixed, ChassisCollisionType chassis_collision_type);

  std::vector<double> m_omega;
}; /// Crawler_vehicle
} /// crawler
} /// vehicle
} /// chrono

#endif //CHRONO_CRAWLER_VEHICLE_H
