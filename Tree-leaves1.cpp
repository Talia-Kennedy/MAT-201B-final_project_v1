#include "al/app/al_App.hpp"
using namespace al;
class MyApp : public App {
    Struct Tree; {
  std::vector<Tree:> branch; {
    Tree tree;
    tree.branch.resize(4);
    tree.branch[0].branch.resize(2);
    tree.branch.erase(tree.branch.begin());
  }
}
}

double time = 0;
  void onAnimate(double dt) override {
    time += dt;
    if (time > 1) {
      process();
      time = 0;
      std::cout << tree.branch() << std::endl;
        }
  }
  void onDraw(Graphics& g) override g.clear(0.4) { 

    struct State {
      Vec3f position;
      double angle;
    };
    std::vector<State> stack{{
        {0, 0, 0}, 0,
    }};

    float t = 1.0 / lsystem.size();
    t = t * t * t;
};
    





