#include "Sakura.h"

using namespace std;

int main() {
    Sakura * sakura = new Sakura("./ionic.png");
    SakuraPicture * pic = sakura->Scale(64, 64, SakuraScaleFilter::LANCZOS);
    Sakura::OutputPng("./ionic2.png", pic);
    return 0;
}
