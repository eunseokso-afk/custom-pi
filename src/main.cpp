#include <Geode/Geode.hpp>
#include <Geode/modify/CCNode.hpp>
#include <numbers>

using namespace geode::prelude;

// Cached settings so we don't do a settings lookup on every single setRotation call
static double g_pi = std::numbers::pi;
static bool g_enabled = true;

// cocos2d turns degrees into radians with (deg * pi / 180).
// If pi were X instead, the angle actually drawn would be deg * (X / realPi).
static float piFactor() {
    if (!g_enabled) return 1.f;
    return static_cast<float>(g_pi / std::numbers::pi);
}

$on_mod(Loaded) {
    g_pi = Mod::get()->getSettingValue<double>("pi-value");
    g_enabled = Mod::get()->getSettingValue<bool>("enabled");

    listenForSettingChanges<double>("pi-value", [](double value) { g_pi = value; });
    listenForSettingChanges<bool>("enabled", [](bool value) { g_enabled = value; });
}

class $modify(PiNode, CCNode) {
    // Store the "wrong pi" angle for rendering...
    void setRotation(float degrees) {
        CCNode::setRotation(degrees * piFactor());
    }

    // ...but hand the game back the angle it thinks it set.
    // Without this, stuff like CCRotateBy reads the scaled value back and
    // compounds it every jump, and your cube becomes a ceiling fan.
    float getRotation() {
        return CCNode::getRotation() / piFactor();
    }
};
