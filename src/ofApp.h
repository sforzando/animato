#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxDatGuiCustomFontSize.h"
#include "ofxPhoto.h"

class ofApp : public ofBaseApp {
public:
  void setup();
  void update();
  void draw();

  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y);
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void mouseEntered(int x, int y);
  void mouseExited(int x, int y);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);
  void exit();
  
  void onButtonCapture(ofxDatGuiButtonEvent e);
  
  void capture();
  
  ofxDatGui * gui;
  ofxDatGuiButton *buttonCapture;
  
  ofFbo fbo;
  
  ofxPhoto photo;
  unsigned char *pixelPicture;
  ofImage imagePicture;
};
