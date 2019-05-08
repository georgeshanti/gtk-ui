#include "GuiSettings.hpp"

std::string fin[4] = {"One Finger", "Two Finger", "Three Finger", "Four Finger"};
std::string dir[4] = {"Left Swipe", "Right Swipe", "Up Swipe", "Down Swipe"};
    
std::string Gesture::name() const{
    return fin[fingers-1]+std::string(" ")+dir[direction-1];
}