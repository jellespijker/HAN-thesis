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

#ifndef CHRONO_CRAWLER_CHASSIS_H
#define CHRONO_CRAWLER_CHASSIS_H

#include <string>

#include "chrono_vehicle/chassis/ChRigidChassis.h"

#include "chrono_models/ChApiModels.h"
#include "chrono_models/vehicle/ChVehicleModelDefs.h"

namespace chrono {
namespace vehicle {
namespace crawler {
class CH_MODELS_API Crawler_Chassis : public ChRigidChassis {
 public:
  Crawler_Chassis(const std::string &name,
                  bool fixed = false,
                  ChassisCollisionType chassis_collision_type = ChassisCollisionType::NONE);

  ~Crawler_Chassis() = default;

  /// Return the mass of the chassis body.
  virtual double GetMass() const override { return m_mass; }

  /// Return the inertia tensor of the chassis body.
  virtual const ChMatrix33<> &GetInertia() const override { return m_inertia; }

  /// Get the location of the center of mass in the chassis frame.
  virtual const ChVector<> &GetLocalPosCOM() const override { return m_COM_loc; }

 protected:
  ChMatrix33<> m_inertia;

  static const double m_mass;
  static const ChVector<> m_inertiaXX;
  static const ChVector<> m_inertiaXY;
  static const ChVector<> m_COM_loc;
}; /// Crawler_Chassis
} /// crawler
} /// vehicle
} /// chrono

#endif //CHRONO_CRAWLER_CHASSIS_H
