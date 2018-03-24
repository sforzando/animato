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

  ofMesh getBackground();
  void onCaptureButton(ofxDatGuiButtonEvent e);

  void capture();

  ofRectangle windowRectangle;
  ofRectangle gifRectangle;
  ofRectangle previewRectangle;
  ofRectangle photoRectangle;

  bool isPhotoLoaded = false;
  bool isBackgroundGenerated = false;

  ofColor keyColor = ofColor::fromHex(0xffd1cd);
  ofMesh backgroundMesh;

  ofxDatGui * gui;
  ofxDatGuiButton *captureButton;

  ofFbo fbo;

  ofxPhoto photo;
  unsigned char *picturePixel;
  ofImage pictureImage;
};
