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

#ifndef CHRONO_HYDRAULICSTEERING_H
#define CHRONO_HYDRAULICSTEERING_H

#include "chrono_vehicle/ChApiVehicle.h"
#include "ChHydraulicSteering.h"

#include "chrono_thirdparty/rapidjson/document.h"

namespace chrono {
namespace vehicle {
class CH_VEHICLE_API HydraulicSteering : public ChHydraulicSteering {
 public:
  HydraulicSteering(const std::string &filename);
  HydraulicSteering(const rapidjson::Document &d);
  ~HydraulicSteering() {};

 private:
  virtual void Create(const rapidjson::Document &d) override;
}; // HydraulicSteering
} /// vehicle
} /// chrono



#endif //CHRONO_HYDRAULICSTEERING_H
