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

#ifndef CHRONO_CHCRAWLERVEHICLE_H
#define CHRONO_CHCRAWLERVEHICLE_H

#include <vector>

#include "chrono_vehicle/ChVehicle.h"
#include "chrono_vehicle/wheeled_vehicle/ChWheel.h"
#include "chrono_vehicle/wheeled_vehicle/ChDriveline.h"
#include "chrono_vehicle/wheeled_vehicle/ChSteering.h"
#include "chrono_vehicle/wheeled_vehicle/ChSuspension.h"

namespace chrono {
namespace vehicle {

/// @addtogroup vehicle_crawler
/// @{

/// Base class for chrono crawler vehicle systems.
/// This class provides the interface between the vehicle system and other
/// systems (tires, driver, etc.).
/// The reference frame for a vehicle follows the ISO standard: Z-axis up, X-axis
/// pointing forward, and Y-axis towards the left of the vehicle.
class CH_VEHICLE_API ChCrawlerVehicle : public ChVehicle {
 public:
  /// Construct a vehicle system with a default ChSystem.
  ChCrawlerVehicle(const std::string &name,                                                 ///< [in] vehicle name
                   ChMaterialSurface::ContactMethod contact_method = ChMaterialSurface::NSC ///< [in] contact method
  );

  /// Construct a vehicle system using the specified ChSystem.
  ChCrawlerVehicle(const std::string &name, ///< [in] vehicle name
                   ChSystem *system         ///< [in] containing mechanical system
  );

  /// Destructor.
  virtual ~ChCrawlerVehicle() = default;

  /// Get the name of the vehicle system template.
  virtual std::string GetTemplateName() const override { return "CrawlerVehicle"; }

  /// Get the specified suspension subsystem.
  std::shared_ptr<ChSuspension> GetSuspension(int id) const { return m_suspensions[id]; }

  /// Get the specified steering subsystem.
  std::shared_ptr<ChSteering> GetSteering(int id) const { return m_steerings[id]; }

  /// Get a handle to the specified vehicle wheel subsystem.
  std::shared_ptr<ChWheel> GetWheel(const WheelID &wheel_id) const { return m_wheels[wheel_id.id()]; }

  /// Get a handle to the vehicle's driveline subsystem.
  std::shared_ptr<ChDriveline> GetDriveline() const { return m_driveline; }

  /// Get the vehicle total mass.
  /// This includes the mass of the chassis and all vehicle subsystems, but not the mass of tires.
  virtual double GetVehicleMass() const override;

  /// Get the current global vehicle COM location.
  virtual ChVector<> GetVehicleCOMPos() const override;

  /// Get a handle to the vehicle's driveshaft body.
  virtual std::shared_ptr<ChShaft> GetDriveshaft() const override { return m_driveline->GetDriveshaft(); }

  /// Get the angular speed of the driveshaft.
  /// This function provides the interface between a vehicle system and a
  /// powertrain system.
  virtual double GetDriveshaftSpeed() const override;

  /// Return the number of axles for this vehicle.
  virtual int GetNumberAxles() const = 0;

  /// Get a handle to the specified wheel body.
  std::shared_ptr<ChBody> GetWheelBody(const WheelID &wheel_id) const;

  /// Get the global location of the specified wheel.
  const ChVector<> &GetWheelPos(const WheelID &wheel_id) const;

  /// Get the orientation of the specified wheel.
  /// The wheel orientation is returned as a quaternion representing a rotation
  /// with respect to the global reference frame.
  const ChQuaternion<> &GetWheelRot(const WheelID &wheel_id) const;

  /// Get the linear velocity of the specified wheel.
  /// Return the linear velocity of the wheel center, expressed in the global
  /// reference frame.
  const ChVector<> &GetWheelLinVel(const WheelID &wheel_id) const;

  /// Get the angular velocity of the specified wheel.
  /// Return the angular velocity of the wheel frame, expressed in the global
  /// reference frame.
  ChVector<> GetWheelAngVel(const WheelID &wheel_id) const;

  /// Get the angular speed of the specified wheel.
  /// This is the angular speed of the wheel axle.
  double GetWheelOmega(const WheelID &wheel_id) const;

  /// Get the complete state for the specified wheel.
  /// This includes the location, orientation, linear and angular velocities,
  /// all expressed in the global reference frame, as well as the wheel angular
  /// speed about its rotation axis.
  WheelState GetWheelState(const WheelID &wheel_id) const;

  /// Return the vehicle wheelbase.
  virtual double GetWheelbase() const = 0;

  /// Return the vehicle wheel track of the specified suspension subsystem.
  double GetWheeltrack(int id) const { return m_suspensions[id]->GetTrack(); }

  /// Return the minimum turning radius.
  /// A concrete wheeled vehicle class should override the default value (20 m).
  virtual double GetMinTurningRadius() const { return 20; } //TODO: determine min turning radius

  /// Return the maximum steering angle.
  /// This default implementation estimates the maximum steering angle based on a bicycle model
  /// and the vehicle minimum turning radius.
  virtual double GetMaxSteeringAngle() const; //TODO: determine max turning radius

  /// Set visualization type for the suspension subsystems.
  /// This function should be called only after vehicle initialization.
  void SetSuspensionVisualizationType(VisualizationType vis);

  /// Set visualization type for the steering subsystems.
  /// This function should be called only after vehicle initialization.
  void SetSteeringVisualizationType(VisualizationType vis);

  /// Set visualization type for the wheel subsystems.
  /// This function should be called only after vehicle initialization.
  void SetWheelVisualizationType(VisualizationType vis);

  /// Enable/disable collision between the chassis and all other vehicle subsystems.
  /// This only controls collisions between the chassis and the tire systems.
  virtual void SetChassisVehicleCollide(bool state = False) override;

  /// Enable/disable output from the suspension subsystems.
  void SetSuspensionOutput(int id, bool state);

  /// Enable/disable output from the steering subsystems.
  void SetSteeringOutput(int id, bool state);

  /// Enable/disable output from the driveline subsystem.
  void SetDrivelineOutput(bool state);

  /// Initialize this vehicle at the specified global location and orientation.
  /// This base class implementation only initializes the chassis subsystem.
  /// Derived classes must extend this function to initialize all other wheeled
  /// vehicle subsystems (steering, suspensions, wheels and driveline).
  virtual void Initialize(const ChCoordsys<> &chassisPos,  ///< [in] initial global position and orientation
                          double chassisFwdVel = 0         ///< [in] initial chassis forward velocity
  ) override;

  /// Update the state of this vehicle at the current time.
  /// The vehicle system is provided the current driver inputs (throttle between
  /// 0 and 1, steering between -1 and +1, braking between 0 and 1), the torque
  /// from the powertrain, and tire forces (expressed in the global reference
  /// frame).
  virtual void Synchronize(double time,                      ///< [in] current time
                           double steering,                  ///< [in] current steering input [-1,+1]
                           double powertrain_torque,         ///< [in] input torque from powertrain
                           const TerrainForces &tire_forces  ///< [in] vector of tire force structures
  );

  /// Log current constraint violations.
  virtual void LogConstraintViolations() override;

  /// Return a JSON string with information on all modeling components in the vehicle system.
  /// These include bodies, shafts, joints, spring-damper elements, markers, etc.
  virtual std::string ExportComponentList() const override;

  /// Write a JSON-format file with information on all modeling components in the vehicle system.
  /// These include bodies, shafts, joints, spring-damper elements, markers, etc.
  virtual void ExportComponentList(const std::string &filename) const override;

  /// Output data for all modeling components in the vehicle system.
  virtual void Output(int frame, ChVehicleOutput &database) const override;

 protected:
  ChSuspensionList m_suspensions;            ///< list of handles to suspension subsystems
  std::shared_ptr<ChDriveline> m_driveline;  ///< handle to the driveline subsystem
  ChSteeringList m_steerings;                ///< list of handles to steering subsystems
  ChWheelList m_wheels;                      ///< list of handles to wheel subsystems
}; /// ChCrawlerVehicle
} /// vehicle
} /// chrono

#endif //CHRONO_CHCRAWLERVEHICLE_H
