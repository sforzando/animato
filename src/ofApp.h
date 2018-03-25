#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxDatGuiCustomFontSize.h"
#include "ofxPhoto.h"

class ofApp : public ofBaseApp {
public:
  void      setup();
  void      update();
  void      draw();

  void      keyPressed(int key);
  void      keyReleased(int key);
  void      mouseMoved(int x, int y);
  void      mouseDragged(int x, int y, int button);
  void      mousePressed(int x, int y, int button);
  void      mouseReleased(int x, int y, int button);
  void      mouseEntered(int x, int y);
  void      mouseExited(int x, int y);
  void      windowResized(int w, int h);
  void      dragEvent(ofDragInfo dragInfo);
  void      gotMessage(ofMessage msg);
  void      exit();

  ofVboMesh getBackground();
  void      onCaptureButton(ofxDatGuiButtonEvent e);

  void      loadGara();
  void      loadHamon();
  void      capture();

  ofRectangle windowRectangle;
  ofRectangle gifRectangle;
  ofRectangle previewRectangle;
  ofRectangle pictureRectangle;

  bool isPhotoLoaded         = false;
  bool isBackgroundGenerated = false;
  bool isGaraLoaded          = false;
  bool isHamonLoaded         = false;

  ofColor   keyColor = ofColor::fromHex(0xffd1cd);
  ofVboMesh backgroundMesh;

  ofxDatGui       *gui;
  ofxDatGuiButton *captureButton;

  ofDirectory                garaUpperDirectory = ofDirectory(ofToDataPath("./gara/upper"));
  ofDirectory                garaLowerDirectory = ofDirectory(ofToDataPath("./gara/lower"));
  vector <vector <ofImage> > garaUpperVector;
  vector <vector <ofImage> > garaLowerVector;
  int                        garaUpperNum;
  int                        garaLowerNum;
  ofDirectory                hamonDirectory = ofDirectory(ofToDataPath("./hamon"));
  vector <ofImage>           hamonImages;
  int                        hamonNum;

  ofFbo fbo;

  ofxPhoto       photo;
  unsigned char *picturePixel;
  ofImage        pictureImage;
};





