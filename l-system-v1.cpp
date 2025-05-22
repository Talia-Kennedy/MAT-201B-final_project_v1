#include "al/app/al_App.hpp"
using namespace al;
class MyApp : public App {
  std::vector<std::string> lsystem{{"0"}};
  // (1 → 11), (0 → 1[0]0)
  std::unordered_map<char, std::string> rules = {{'0', "1[0]0"}, {'1', "11"}};



  void onCreate() override {
    // set up the window
    nav().pos(0, 0, 2);
    nav().faceToward(Vec3f(0, 0, 0));
  }

  void process() {
    std::string next;
    for (char c : lsystem.back()) {
      if (rules.find(c) != rules.end()) {
        // if we found a rule for this character....
        next += rules[c];
      } else {
        next += c;
      }
    }
    lsystem.push_back(next);
  }

  // axiom (starting string)
  // constants
  // rules
  // iteration...
  //
  double time = 0;
  void onAnimate(double dt) override {
    time += dt;
    if (time > 1) {
      process();
      time = 0;
      std::cout << lsystem.back() << std::endl;
    }
  }
  void onDraw(Graphics& g) override { g.clear(0.4); 


    struct State {
      Vec3f position;
      double angle;
    };
    std::vector<State> stack{{
        {0, 0, 0}, 0,
    }};

    float t = 1.0 / lsystem.size();
    t = t * t * t;
    // right now we are running constant temperature
    //Branches will double every 20 seconds 
    // fish require 1 stem for food 
    // cycle repeats until there is no food left 



    Mesh m{Mesh::LINES};
    for (char c : lsystem.back()) {
      if (c == '0') {
        auto forward = Vec3f(0, t, 0).rotate(stack.back().angle);
        m.vertex(stack.back().position);
        stack.back().position += forward;
        m.vertex(stack.back().position);
      } else if (c == '1') {
        auto forward = Vec3f(0, t, 0).rotate(stack.back().angle);
        m.vertex(stack.back().position);
        stack.back().position += forward;
        m.vertex(stack.back().position);
      } else if (c == '[') {
        // push position and angle, turn left 45 degrees
        auto& back = stack.back();
        stack.push_back({back.position, back.angle + M_PI / 4});
      } else if (c == ']') {
        // pop position and angle, turn right 45 degrees
        stack.pop_back();
      }
    }
    g.draw(m);

}
};
int main() { MyApp().start(); }