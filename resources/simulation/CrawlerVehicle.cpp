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

#include "chrono/assets/ChSphereShape.h"
#include "chrono/assets/ChTriangleMeshShape.h"
#include "chrono/utils/ChUtilsInputOutput.h"

#include "chrono_vehicle/ChVehicleModelData.h"

#include "Crawler_vehicle.h"

namespace chrono {
namespace vehicle {
namespace crawler {
Crawler_vehicle::Crawler_vehicle(const bool fixed,
                                 ChMaterialSurface::ContactMethod contact_method,
                                 ChassisCollisionType chassis_collision_type)
    : ChCrawlerVehicle("Crawler", contact_method), m_omega({0, 0}) {
  Create(fixed, chassis_collision_type);
}

Crawler_vehicle::Crawler_vehicle(ChSystem *system, const bool fixed, ChassisCollisionType chassis_collision_type)
    : ChCrawlerVehicle("Crawler", system), m_omega({0, 0}) {
  Create(fixed, chassis_collision_type);
}

void Crawler_vehicle::Create(bool fixed, ChassisCollisionType chassis_collision_type) {
  // -------------------------------------------
  // Create the chassis subsystem
  // -------------------------------------------
  m_chassis = std::make_shared<Crawler_Chassis>("Chassis", fixed, chassis_collision_type);

  // -------------------------------------------
  // Create the suspension subsystems
  // -------------------------------------------
  m_suspensions.resize(2);
  m_suspensions[0] = std::make_shared<Sedan_DoubleWishbone>("FrontSusp");
  m_suspensions[1] = std::make_shared<Sedan_MultiLink>("RearSusp");

  // -----------------------------
  // Create the steering subsystem
  // -----------------------------
  m_steerings.resize(1);
  m_steerings[0] = std::make_shared<Sedan_RackPinion>("Steering");

  // -----------------
  // Create the wheels
  // -----------------
  m_wheels.resize(4);
  m_wheels[0] = std::make_shared<Sedan_WheelLeft>("Wheel_FL");
  m_wheels[1] = std::make_shared<Sedan_WheelRight>("Wheel_FR");
  m_wheels[2] = std::make_shared<Sedan_WheelLeft>("Wheel_RL");
  m_wheels[3] = std::make_shared<Sedan_WheelRight>("Wheel_RR");

  // --------------------
  // Create the driveline
  // --------------------
  m_driveline = std::make_shared<Sedan_Driveline2WD>("Driveline");
}

} /// crawler
} /// vehicle
} /// chrono