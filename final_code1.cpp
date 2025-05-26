#include "al/app/al_App.hpp"
#include "al/app/al_GUIDomain.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/math/al_Random.hpp"
#include "al/math/al_Vec.hpp"

using namespace al;

#include <fstream>
#include <vector>
using namespace std;

Vec3f randomVec3f(float scale) {
  return Vec3f(rnd::uniformS(), rnd::uniformS(), rnd::uniformS()) * scale;
}
struct AlloApp : App {
  Parameter timeStep{"/timeStep", "", 0.1, 0.01, 0.6};
  Parameter boundarySize{"/boundSize", "", 0.1, 0.01, 10.0};
  Light light;
  Material material;  // Necessary for specular highlights
  Mesh mesh;
  Color color;

  std::vector<Nav> agent;
  std::vector<float> size; // (0, 1]
  std::vector<int> interest; // (0, 1]




 /// MESH OBJECTS CREATED:
 /// Red fish (Rf) Blue fish (Bf) Yellow fish (Yf) and Plants(?)
 /// Red fish = small slow plant fish
 /// Blue fish = large faster plant fish
 /// Yellow fish = large really fast meat fish 
 /// Plants = l-system(?) algorithm for feeding  
 /// define each fish as a separate angent?
 /// Now using (i), change what (i) can be?
 


  /// GUI PARAMETERS

  void onInit() override {
    auto GUIdomain = GUIDomain::enableGUI(defaultWindowDomain());
    auto &gui = GUIdomain->newGUI();
    gui.add(timeStep);
    gui.add(boundarySize);
  }
  
  


  
  /// MESH FOR FISH BUILD

  /// 3 different meshes for fishes?
  /// All need different sizes
  void onCreate() override {
    nav().pos(0, 0, 10);
    addSphere(mesh);
    mesh.scale( 1, 1, 1);
    mesh.translate(0, 0, -0.2);
    addCone(mesh);
    mesh.scale(0.2, 0.6, 1.2);
    mesh.scale(0.3);
    mesh.generateNormals();
    light.pos(0, 10, 10);
    addRect(mesh);
    mesh.scale (0.5, 1);
    mesh.translate (-2, 0);
    /// Inserting and positioning 2D objects:
    /// add in another mesh shape to create fins? 
    /// sideways heart for tail?
    /// tear drop shape for side fins?
    /// the top fin isoscoles trapezoid?

    Mesh body;
    ///addSphere(body);
    ///body.scale(0.2, 0.6, 1.2);
  






    
    ///ATTRACTION, REPULSION AND HITBOX

    for (int i = 0; i < 10; ++i) {
      Nav p;
      p.pos() = randomVec3f(5);
      p.quat()
          .set(rnd::uniformS(), rnd::uniformS(), rnd::uniformS(),
               rnd::uniformS())
          .normalize();
      agent.push_back(p);
      size.push_back(rnd::uniform(0.05, 1.0));
      interest.push_back(-1);
    }
  }
/// All fish have hitbox
///Red fish attracted to nearest plant, repelled by nearest yellow fish 
///Blue fish attracted to nearest plant, repelled by nearest yellow fish 
///Yellow fish attracted to nearest plant eating fish (Rf and Bf)
///Eating/being eaten lasts 3 seconds
///After feeding locate nearest food (except object just eaten) 






///FOOD/PLANT

  Vec3f food; //12 Plants ONLY (Evenly spaced in simulation)
  double time = 0;

  bool paused = false;
  void onAnimate(double dt) override {
    if (paused) return;
    if (time > 3) {
      time -= 3;
      //genetation time for plants
      // if temp is between 4.1 -6 double every 20 second
      food = randomVec3f(3);
    }
  /// HEALTHBAR: Lost or gained by eating, starving or being eaten
    /// RED FISH : 0-10 points
    /// if Rf eats 1 plants, health bar up 1
    /// if Rf is eaten, health bar down 5 (Freezes if Yf enters hitbox)
    /// if Rf doesn't eat 20 seconds after last feed, health bar down 1
    /// BLUE FISH : 0-20 points 
    /// if Bf eats 2 plant, health bar up 2
    /// if Bf is eaten, health bar down 5 (Freezes if Yf enters hitbox)
    /// if Bf doesn't eat 15 seconds after last feed, health bar down 2
    /// YELLOW FISH : 0-30 points
    /// if Yf eats 1 fish , health bar up 5 for Rf and 8 for Bf (3)
    /// if Yf doesn't eat 15 seconds after last feed, health bar down 4
    /// HOW TO SIMULATE FLASHING WHEN HEALTH IS <6?
  /// PLANT: if plant cannot regenerate after all stems eaten it will dissapear 

   


   
///TIME/SPEED
   
    time += dt;

    for (int i = 0; i < agent.size(); ++i) {
      if (interest[i] >= 0) {
        continue;
      }
      // search....
      for (int j = i + 1; j < agent.size(); ++j) {
        // i,j
        float difference = size[j] - size[i];
        if (difference > 0 && difference < 0.1) {
          // j is a little bigger than i
          interest[i] = j;
        }
      }
    }

    for (int i = 0; i < agent.size(); ++i) {
      if (interest[i] >= 0) {
        agent[i].faceToward(agent[interest[i]].pos(), 0.1);
        agent[i].nudgeToward(agent[interest[i]].pos(), -0.1);
      }
      else {
        agent[i].faceToward(food, 0.1);
      }
    }

    for (int i = 0; i < agent.size(); ++i) {
      agent[i].moveF(5);
    }


    for (int i = 0; i < agent.size(); ++i) {
      agent[i].step(dt);
    }
  }

  bool onKeyDown(const Keyboard &k) override {
    if (k.key() == ' 1 ') {
      paused = !paused;
    }
  }
/// SPEED FOR EACH FISH
   /// Red fish moves 6 units per second
   /// Blue fish moves 8 units per second
   /// Yellow fish moves 10 units per second
   /// Pauses for 3 seconds once in plant/fish hitbox
/// ClOCK FOR REPRODUCTION 
   /// Healthy temp.: every 60 seconds living fish population doubles
   /// Healthy temp: every 20 seconds remainings branches grow     






/// Lighting and coloring 
  void onDraw(Graphics &g) override {
    g.light(light);
    material.specular(light.diffuse() * 0.2);
    material.shininess(50);
    g.material(material);
    g.clear(0.27);
    g.depthTesting(true);
    g.lighting(true);
    light.globalAmbient(RGB(0.1));
    light.ambient(RGB(0));
    light.diffuse(RGB(1, 1, 0.5));
    //affects color and shade
    g.light(light);
    material.specular(light.diffuse() * 0.2);
    material.shininess(50);
    g.material(material);

    for (int i = 0; i < agent.size(); ++i) {
      g.pushMatrix();
      g.translate(agent[i].pos());
      g.rotate(agent[i].quat());
      g.scale(size[i]);
      g.draw(mesh);
      g.popMatrix();
    }
/// How to ensure each fish and plant recieves right color?
/// Fish and plant colors need to be diffeernt but they get blended?




/// PLANT MESH ( 1 line = 1 unit of food)
    Mesh m;
    addCylinder(m, 0.1);
    ///change to leaf or shape//
    m.generateNormals();
    g.translate(food);
    g.color(0.2, 1, 0.6);
    /// makes plant green//
    g.draw(m);
  }
};
///L-systems are 2-D
///Translation into 3D? 
///Better system for food instead of stems?




/// FINAL 
int main() {
  AlloApp app;
  app.configureAudio(48000, 512, 2, 0);
  app.start();
}
