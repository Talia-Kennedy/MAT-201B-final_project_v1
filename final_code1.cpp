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

  
  // Parameter size{"/size","", 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  // Parameter speed{"/speed","", 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  Light light;
  Material material; // Necessary for specular highlights
  Mesh mesh;
  Color color;

  std::vector<Nav> agent;
  std::vector<float> size;     // (0, 1]
  std::vector<int> interest;   // (0, 1]
  std::vector<float> quantity; // number of agents
  std::vector <float> speed;
  std::vector <int> i; 
  std::vector <float> b; // blue fish (medium)
  std::vector <float> r; // red fish (largest)
  std::vector <float> y; // yellow fish (smallest)
  



  /// GUI PARAMETERS

  void onInit() override {
    auto GUIdomain = GUIDomain::enableGUI(defaultWindowDomain());
    auto &gui = GUIdomain->newGUI();
    gui.add(timeStep);
    gui.add(boundarySize);
  }

  /// MESH FOR FISH BUILD

  void onCreate() override { 
    nav().pos(0, 0, 10);
    addSphere(mesh);
    mesh.scale(1, 1, 1);
    mesh.translate(0, 0, -0.2);
    addCone(mesh);
    mesh.scale(0.2, 0.6, 1.2);
    mesh.scale(0.3);
    mesh.generateNormals();
    light.pos(0, 10, 10);
    addRect(mesh);
    mesh.scale(0.5, 1);
    mesh.translate(-2, 0);


    nav().faceToward(Vec3f(0, 0, 0));

    // POSITIONING FISH
    // PRIMARY BUILD int i (int i = yellow fish)
    // int b = medium blue fish (i + 2)
    // int r = big red fish (i + 3)
    

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
      speed.push_back(10); // speed of fish  
    }



    /// TRYING TO CREATE VARIOUS INT VALUES TO PAIR WITH FISH BUILDS

    // for (int i = 0; i < agent.size(); ++i) { /// yellow (y = i)
    //   for (int c = i + 2; c < agent.size(); ++c) { /// blue (b = c)
    //     for (int d = i + 3; d < agent.size(); ++d) { /// red (r = d)
    //     }

    //   } 
    // } 


    /// VECTORS TO INT VALUES
    
    // Vec3f y = int [i];
    // Vec3f b = int [c];
    // Vec3f r = int [d];


    /// MULTIPLYING STANDARD MESH SIZES FOR EACH FISH

    // for (int i = 0; i < agent.size(); ++i) {
    //   // auto agent = agent.size()[i];
    //   Vec3f y = (agent.size()[i]); // Yellow fish
    //   Vec3f b = (agent.size()[i * 1.5]); // Blue fish 1.5 times as big
    //   Vec3f r = (agent.size()[i * 2]); // Red fish 2 times as big
    // };


    /// CONVERTING EACH VALUE INTO A NEW COLOR

    // for (int i = 0; agent.color(); ++i) {
    //   Vec3f y = agent.color(RGB(1, 1, 0.5)); // Yellow color
    //   Vec3f b = agent.color(RGB(0.1, 0.2, 1)); // Blue color
    //   Vec3f r = agent.color(RGB(1, 0, 0.1)); // Red color
    // }

    /// BASING A NEW SPEED OFF OF THE STANDARD FOR i (YEllOW FISH)

    // for (int i = 0; i < agent.speed(); ++i) {
    //   // auto agent = agent.size()[i];
    //   Vec3f y = (agent.speed()[i]); // Yellow fish
    //   Vec3f b = (agent.speed()[i * 1.5]); // Blue fish 1.5 times as big
    //   Vec3f r = (agent.speed()[i * 2]); // Red fish 2 times as big
    // };

  

    /// REPELLING FOOD ANF 
    food.push_back(randomVec3f(4));
    fish.push_back(randomVec3f(4));
  }

  /// FOOD/PLANT VECTORS
  std::vector<Vec3f> fish; 

  std::vector<Vec3f> food; // 12 Plants ONLY (Evenly spaced in simulation)
  
  

  // TIMING
  
  double time = 0;

  double fish_double_timer = 0;

  double food_double_timer = 0;

  bool paused = false;
  void onAnimate(double dt) override {
    if (paused)
      return;

      

    time += dt;
    fish_double_timer += dt;
    food_double_timer += dt;


    if (food_double_timer > 20) {
        food_double_timer -= 20;

        food.push_back(randomVec3f(5));

        
    }; //// food doubling (make into )



    if (fish_double_timer > 60) {
      fish_double_timer -= 60; /// How fast fish population doubles (will be 60 seconds)
      
      // QUANTIFYING EACH FISH AT BEGINNING OF SIM
      // if (fish_double_timer -= 1) {
      //   Vec3f y = 5 // YELLOW START NUMBER
      //   Vec3f b = 3 // BLUE START NUMBER
      //   Vec3f r = 2 // RED START NUMBER
      // }
        

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


   // THESE SECTIONS WILL REPLACE i WITH THE OTHER VALUES
   // DEPENDING ON FISH TYPE BEHAVIOR THEY WILL ACT DIFFERENT

    for (int i = 0; i < agent.size(); ++i) {
      if (interest[i] >= 0) {
        continue;
      }
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
      } else {
        int nearest = -1;
        float distance = 999999;
        for (int k = 0; k < food.size(); ++k) {
          float d2 = (food[k] - agent[i].pos()).magSqr();
          if (d2 < distance) {
            nearest = k;
            distance = d2;
          }
        } // we have the nearest
        

        if (nearest > -1) {
          agent[i].faceToward(food[nearest], 0.1);
        }
      }
    }

    for (int i = 0; i < agent.size(); ++i) {
      agent[i].moveF(5);
    }

    for (int i = 0; i < agent.size(); ++i) {
      agent[i].step(dt);
    }

  }

  
  /// LIGHTING AND COLORING

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
    light.diffuse(RGB(1, 1, 0.5)); // yellow color
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
    /// change to leaf or shape//
    m.generateNormals();
    
    g.color(0.2, 1, 0.6); /// makes plant green//

    for (auto& f : food) {
      g.translate(f);
      g.draw(m);
    }


  }
};


/// FINAL
int main() {
  AlloApp app;
  app.configureAudio(48000, 512, 2, 0);
  app.start();
};

