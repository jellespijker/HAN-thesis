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

#include "chrono/core/ChStream.h"
#include "chrono/core/ChRealtimeStep.h"
#include "chrono/utils/ChUtilsInputOutput.h"

#include "chrono_vehicle/ChConfigVehicle.h"
#include "chrono_vehicle/ChVehicleModelData.h"
#include "chrono_vehicle/terrain/RigidTerrain.h"
#include "chrono_vehicle/driver/ChIrrGuiDriver.h"
#include "chrono_vehicle/driver/ChDataDriver.h"
#include "chrono_vehicle/wheeled_vehicle/utils/ChWheeledVehicleIrrApp.h"

#include "Crawler.h"

#include "chrono_thirdparty/filesystem/path.h"

using namespace chrono;
using namespace chrono::irrlicht;
using namespace chrono::vehicle;
using namespace chrono::vehicle::crawler;

// =============================================================================

// Initial vehicle location and orientation
ChVector<> initLoc(0, 0, 1.0);
ChQuaternion<> initRot(1, 0, 0, 0);
// ChQuaternion<> initRot(0.866025, 0, 0, 0.5);
// ChQuaternion<> initRot(0.7071068, 0, 0, 0.7071068);
// ChQuaternion<> initRot(0.25882, 0, 0, 0.965926);
// ChQuaternion<> initRot(0, 0, 0, 1);

enum DriverMode { DEFAULT, RECORD, PLAYBACK };
DriverMode driver_mode = DEFAULT;

// Visualization type for vehicle parts (PRIMITIVES, MESH, or NONE)
VisualizationType chassis_vis_type = VisualizationType::MESH;
VisualizationType suspension_vis_type = VisualizationType::PRIMITIVES;
VisualizationType steering_vis_type = VisualizationType::PRIMITIVES;
VisualizationType wheel_vis_type = VisualizationType::MESH;

// Collision type for chassis (PRIMITIVES, MESH, or NONE)
ChassisCollisionType chassis_collision_type = ChassisCollisionType::NONE;

// Type of powertrain model (SHAFTS, SIMPLE)
// PowertrainModelType powertrain_model = PowertrainModelType::SHAFTS;

// Drive type (FWD)
// DrivelineType drive_type = DrivelineType::FWD;

// Type of tire model (RIGID, TMEASY)
TireModelType tire_model = TireModelType::RIGID;

// Rigid terrain
RigidTerrain::Type terrain_model = RigidTerrain::BOX;
double terrainHeight = 0;      // terrain height (FLAT terrain only)
double terrainLength = 100.0;  // size in X direction
double terrainWidth = 100.0;   // size in Y direction

// Point on chassis tracked by the camera
ChVector<> trackPoint(0.0, 0.0, 1.75);

// Contact method
ChMaterialSurface::ContactMethod contact_method = ChMaterialSurface::SMC;
bool contact_vis = false;

// Simulation step sizes
double step_size = 1e-3;
double tire_step_size = step_size;

// Simulation end time
double t_end = 1000;

// Time interval between two render frames
double render_step_size = 1.0 / 50;  // FPS = 50

// Output directories
const std::string out_dir = GetChronoOutputPath() + "Sedan";
const std::string pov_dir = out_dir + "/POVRAY";

// Debug logging
bool debug_output = false;
double debug_step_size = 1.0 / 1;  // FPS = 1

// POV-Ray output
bool povray_output = false;

// =============================================================================

int main(int argc, char* argv[]) {
  GetLog() << "Copyright (c) 2019 Jelle Spijker, using Project Chrono version: " << CHRONO_VERSION << "\n\n";

  // --------------
  // Create systems
  // --------------

  // Create the Crawler vehicle, set parameters, and initialize
  Crawler ihc_crawler;
  ihc_crawler.SetContactMethod(contact_method);
  ihc_crawler.SetChassisCollisionType(chassis_collision_type);
  ihc_crawler.SetChassisFixed(false);
  ihc_crawler.SetInitPosition(ChCoordsys<>(initLoc, initRot));
  //ihc_crawler.SetPowertrainType(powertrain_model);
  //ihc_crawler.SetDriveType(drive_type);
  ihc_crawler.SetTireType(tire_model);
  ihc_crawler.SetTireStepSize(tire_step_size);
  ihc_crawler.SetVehicleStepSize(step_size);
  ihc_crawler.Initialize();

  VisualizationType tire_vis_type = VisualizationType::MESH;// : VisualizationType::PRIMITIVES;

  ihc_crawler.SetChassisVisualizationType(chassis_vis_type);
  ihc_crawler.SetSuspensionVisualizationType(suspension_vis_type);
  ihc_crawler.SetSteeringVisualizationType(steering_vis_type);
  ihc_crawler.SetWheelVisualizationType(wheel_vis_type);
  ihc_crawler.SetTireVisualizationType(tire_vis_type);

  // Create the terrain
  RigidTerrain terrain(ihc_crawler.GetSystem());

  std::shared_ptr<RigidTerrain::Patch> patch;
  switch (terrain_model) {
    case RigidTerrain::BOX:
      patch = terrain.AddPatch(ChCoordsys<>(ChVector<>(0, 0, terrainHeight - 5), QUNIT),
                               ChVector<>(terrainLength, terrainWidth, 10));
      patch->SetTexture(vehicle::GetDataFile("terrain/textures/tile4.jpg"), 200, 200);
      break;
    case RigidTerrain::HEIGHT_MAP:
      patch = terrain.AddPatch(CSYSNORM, vehicle::GetDataFile("terrain/height_maps/test64.bmp"), "test64", 128,
                               128, 0, 4);
      patch->SetTexture(vehicle::GetDataFile("terrain/textures/grass.jpg"), 16, 16);
      break;
    case RigidTerrain::MESH:
      patch = terrain.AddPatch(CSYSNORM, vehicle::GetDataFile("terrain/meshes/test.obj"), "test_mesh");
      patch->SetTexture(vehicle::GetDataFile("terrain/textures/grass.jpg"), 100, 100);
      break;
  }
  patch->SetContactFrictionCoefficient(0.9f);
  patch->SetContactRestitutionCoefficient(0.01f);
  patch->SetContactMaterialProperties(2e7f, 0.3f);
  patch->SetColor(ChColor(0.8f, 0.8f, 0.5f));
  terrain.Initialize();

  // Create the vehicle Irrlicht interface
  ChWheeledVehicleIrrApp app(&ihc_crawler.GetVehicle(), &ihc_crawler.GetPowertrain(), L"Crawler Demo");
  app.SetSkyBox();
  app.AddTypicalLights(irr::core::vector3df(30.f, -30.f, 100.f), irr::core::vector3df(30.f, 50.f, 100.f), 250, 130);
  app.SetChaseCamera(trackPoint, 6.0, 0.5);
  app.SetTimestep(step_size);
  app.AssetBindAll();
  app.AssetUpdateAll();

  // -----------------
  // Initialize output
  // -----------------

  if (!filesystem::create_directory(filesystem::path(out_dir))) {
    std::cout << "Error creating directory " << out_dir << std::endl;
    return 1;
  }
  if (povray_output) {
    if (!filesystem::create_directory(filesystem::path(pov_dir))) {
      std::cout << "Error creating directory " << pov_dir << std::endl;
      return 1;
    }
    terrain.ExportMeshPovray(out_dir);
  }

  std::string driver_file = out_dir + "/driver_inputs.txt";
  utils::CSV_writer driver_csv(" ");

  // ------------------------
  // Create the driver system
  // ------------------------

  // Create the interactive driver system
  ChIrrGuiDriver driver(app);

  // Set the time response for steering and throttle keyboard inputs.
  double steering_time = 1.0;  // time to go from 0 to +1 (or from 0 to -1)
  double throttle_time = 1.0;  // time to go from 0 to +1
  double braking_time = 0.3;   // time to go from 0 to +1
  driver.SetSteeringDelta(render_step_size / steering_time);
  driver.SetThrottleDelta(render_step_size / throttle_time);
  driver.SetBrakingDelta(render_step_size / braking_time);

  // If in playback mode, attach the data file to the driver system and
  // force it to playback the driver inputs.
  if (driver_mode == PLAYBACK) {
    driver.SetInputDataFile(driver_file);
    driver.SetInputMode(ChIrrGuiDriver::DATAFILE);
  }

  driver.Initialize();

  // ---------------
  // Simulation loop
  // ---------------

  if (debug_output) {
    GetLog() << "\n\n============ System Configuration ============\n";
    ihc_crawler.LogHardpointLocations();
  }

  //output vehicle mass
  std::cout<<"VEHICLE MASS: "<<my_sedan.GetVehicle().GetVehicleMass()<<std::endl;

  // Number of simulation steps between miscellaneous events
  int render_steps = (int)std::ceil(render_step_size / step_size);
  int debug_steps = (int)std::ceil(debug_step_size / step_size);

  // Initialize simulation frame counter and simulation time
  ChRealtimeStepTimer realtime_timer;
  int step_number = 0;
  int render_frame = 0;
  double time = 0;

  if (contact_vis) {
    app.SetSymbolscale(1e-4);
    app.SetContactsDrawMode(ChIrrTools::eCh_ContactsDrawMode::CONTACT_FORCES);
  }

  while (app.GetDevice()->run()) {
    time = ihc_crawler.GetSystem()->GetChTime();

    // End simulation
    if (time >= t_end)
      break;

    // Render scene and output POV-Ray data
    if (step_number % render_steps == 0) {
      app.BeginScene(true, true, irr::video::SColor(255, 140, 161, 192));
      app.DrawAll();
      app.EndScene();

      if (povray_output) {
        char filename[100];
        sprintf(filename, "%s/data_%03d.dat", pov_dir.c_str(), render_frame + 1);
        utils::WriteShapesPovray(ihc_crawler.GetSystem(), filename);
      }

      render_frame++;
    }

    // Debug logging
    if (debug_output && step_number % debug_steps == 0) {
      GetLog() << "\n\n============ System Information ============\n";
      GetLog() << "Time = " << time << "\n\n";
      ihc_crawler.DebugLog(OUT_SPRINGS | OUT_SHOCKS | OUT_CONSTRAINTS);
    }

    // Collect output data from modules (for inter-module communication)
    double throttle_input = driver.GetThrottle();
    double steering_input = driver.GetSteering();
    double braking_input = driver.GetBraking();

    // Driver output
    if (driver_mode == RECORD) {
      driver_csv << time << steering_input << throttle_input << braking_input << std::endl;
    }

    // Update modules (process inputs from other modules)
    driver.Synchronize(time);
    terrain.Synchronize(time);
    ihc_crawler.Synchronize(time, steering_input, braking_input, throttle_input, terrain);
    app.Synchronize(driver.GetInputModeAsString(), steering_input, throttle_input, braking_input);

    // Advance simulation for one timestep for all modules
    double step = realtime_timer.SuggestSimulationStep(step_size);
    driver.Advance(step);
    terrain.Advance(step);
    ihc_crawler.Advance(step);
    app.Advance(step);

    // Increment frame number
    step_number++;
  }

  if (driver_mode == RECORD) {
    driver_csv.write_to_file(driver_file);
  }

  return 0;
}

