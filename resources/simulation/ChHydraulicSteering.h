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

#ifndef CHRONO_CHHYDRAULICSTEERING_H
#define CHRONO_CHHYDRAULICSTEERING_H

#include "chrono_vehicle/ChApiVehicle.h"
#include "chrono_vehicle/wheeled_vehicle/ChSteering.h"

namespace chrono {
namespace vehicle {
class CH_VEHICLE_API ChHydraulicSteering : public ChSteering {
 public:
  ChHydraulicSteering(const std::string &name);

  virtual ~ChHydraulicSteering() = default;

  /// Get the name of the vehicle subsystem template.
  virtual std::string GetTemplateName() const override { return "RotaryArm"; }

  /// Initialize this steering subsystem.
  /// The steering subsystem is initialized by attaching it to the specified
  /// chassis body at the specified location (with respect to and expressed in
  /// the reference frame of the chassis) and with specified orientation (with
  /// respect to the chassis reference frame).
  virtual void Initialize(std::shared_ptr<ChBodyAuxRef> chassis,  ///< [in] handle to the chassis body
                          const ChVector<> &location,             ///< [in] location relative to the chassis frame
                          const ChQuaternion<> &rotation          ///< [in] orientation relative to the chassis frame
  ) override;

  /// Add visualization assets for the steering subsystem.
  /// This default implementation uses primitives.
  virtual void AddVisualizationAssets(VisualizationType vis) override;

  /// Remove visualization assets for the steering subsystem.
  virtual void RemoveVisualizationAssets() override;

  /// Update the state of this steering subsystem at the current time.
  /// The steering subsystem is provided the current steering driver input (a
  /// value between -1 and +1).  Positive steering input indicates steering
  /// to the left. This function is called during the vehicle update.
  virtual void Synchronize(double time,     ///< [in] current time
                           double steering  ///< [in] current steering input [-1,+1]
  ) override;

  /// Get the total mass of the steering subsystem.
  virtual double GetMass() const override;

  /// Get the current global COM location of the steering subsystem.
  virtual ChVector<> GetCOMPos() const override;

  /// Log current constraint violations.
  virtual void LogConstraintViolations() override;

}; // ChHydraulicSteering
} // vehicle
} // chrono

#endif //CHRONO_CHHYDRAULICSTEERING_H
