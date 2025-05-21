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
  Parameter boundarySize{"/boundSize", "", .1, 0.01, 10.0};
  Light light;
  Material material;  // Necessary for specular highlights
  Mesh mesh;

  // size, color, species, sex, age, etc.
  std::vector<Nav> agent;
  std::vector<float> size; // (0, 1]
  std::vector<int> interest; // (0, 1]


  void onInit() override {
    auto GUIdomain = GUIDomain::enableGUI(defaultWindowDomain());
    auto &gui = GUIdomain->newGUI();
    gui.add(timeStep);
    gui.add(boundarySize);
  }

  void onCreate() override {
    nav().pos(0, 0, 10);
    addSphere(mesh);
    mesh.scale( 1, 1, 1);
    mesh.translate(0, 0, -0.2);
    addCone(mesh);
    mesh.scale(1, 1, 1.3);
    mesh.scale(0.3);

    mesh.generateNormals();
    light.pos(10, 0, 10);

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

  Vec3f food;
  double time = 0;

  bool paused = false;
  void onAnimate(double dt) override {
    if (paused) return;
    if (time > 3) {
      time -= 3;

      food = randomVec3f(3);
      // position must be within 100, 1000, 1000 area
    }
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

  void onDraw(Graphics &g) override {
    g.clear(0.27);
    g.depthTesting(true);
    g.lighting(true);
    light.globalAmbient(RGB(0.1));
    light.ambient(RGB(0));
    light.diffuse(RGB(1, 1, 0.5));
    //affects color shade/
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

    Mesh m;
    addSphere(m, 0.1);
    ///change to leaf or shape//
    m.generateNormals();
    g.translate(food);
    g.color(0.2, 1, 0.6);
    /// makes plant green//
    g.draw(m);
  }
};

int main() {
  AlloApp app;
  app.configureAudio(48000, 512, 2, 0);
  app.start();
}
