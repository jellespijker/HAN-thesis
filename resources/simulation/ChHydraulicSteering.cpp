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

#include "ChHydraulicSteering.h"

namespace chrono {
namespace vehicle {

ChHydraulicSteering::ChHydraulicSteering(const std::string &name) : ChSteering(name) {}

void ChHydraulicSteering::Initialize(std::shared_ptr<ChBodyAuxRef> chassis,
                                     const ChVector<> &location,
                                     const ChQuaternion<> &rotation) {
  m_position = ChCoordsys<>(location, rotation);

  // TODO: workout initialize no visualization needed at this time

}

void ChHydraulicSteering::AddVisualizationAssets(VisualizationType vis) {
  ChPart::AddVisualizationAssets(vis);
}

void ChHydraulicSteering::RemoveVisualizationAssets() {
  ChPart::RemoveVisualizationAssets();
}

void ChHydraulicSteering::Synchronize(double time, double steering) {
  // TODO: implement
}

double ChHydraulicSteering::GetMass() const {
  return 0;
}

ChVector<> ChHydraulicSteering::GetCOMPos() const {
  return ChVector<>();
}

void ChHydraulicSteering::LogConstraintViolations() {
  ChSteering::LogConstraintViolations();
}

} // vehicle
} // chrono