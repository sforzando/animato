#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxDatGuiCustomFontSize.h"
#include "ofxPhoto.h"
#include "SysCommand.h"

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

  void      onCaptureButton(ofxDatGuiButtonEvent e);
  void      onLoadButton(ofxDatGuiButtonEvent e);
  void      onColorPicker(ofxDatGuiColorPickerEvent e);

  ofVboMesh getBackground();
  void      loadPhoto();
  void      loadGara();
  void      loadHamon();
  void      capture();
  bool      cameraCheck();

  void      setStatusMessage(string s, ofLogLevel level=OF_LOG_NOTICE);
  void      say(string s);

  SysCommand sysCommand;

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

  ofxDatGui            *gui;
  ofxDatGuiButton      *captureButton;
  ofxDatGuiButton      *loadButton;
  ofxDatGuiColorPicker *colorPicker;
  ofxDatGuiTextInput   *statusTextInput;

  ofDirectory                garaUpperDirectory = ofDirectory(ofToDataPath("./materials/gara/upper"));
  ofDirectory                garaLowerDirectory = ofDirectory(ofToDataPath("./materials/gara/lower"));
  vector <vector <ofImage> > garaUpperVector;
  vector <vector <ofImage> > garaLowerVector;
  int                        garaUpperKinds;
  int                        garaLowerKinds;
  int                        garaUpperCurrentKind = 0;
  int                        garaLowerCurrentKind = 0;
  ofDirectory                hamonDirectory       = ofDirectory(ofToDataPath("./materials/hamon"));
  vector <ofImage>           hamonImages;
  int                        hamonNum;
  ofImage                    mojiImage = ofImage("./materials/moji.png");

  ofFbo fbo;

  ofxPhoto       photo;
  unsigned char *picturePixel;
  ofImage        pictureImage;
};


