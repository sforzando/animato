#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxDatGuiCustomFontSize.h"
#include "ofxXmlSettings.h"
#include "SysCommand.h"

class ofApp : public ofBaseApp
{
public:
  const int GUI_MIN_WIDTH = 360;

  ofxXmlSettings settings;

  void      setup();
  void      update();
  void      draw();

  void      keyPressed(int key);
  void      keyReleased(int key);
  void      windowResized(int w, int h);
  void      gotMessage(ofMessage msg);
  void      exit();

  ofVboMesh getBackground_STRIP();
  ofVboMesh getBackground_FAN();
  void      loadPhoto();
  void      loadGara();
  void      loadHamon();
  void      capture();
  void      generateGif();
  bool      uploadGif();
  void      printQr();

  void      selectGaraUpper(int kind);
  void      selectGaraLower(int kind);

  void      setStatusMessage(string s, ofLogLevel level=OF_LOG_NOTICE);
  void      say(string s, bool async=true);

  SysCommand sysCommand;

  ofRectangle windowRectangle;
  ofRectangle gifRectangle;
  int         gifWidth;
  int         gifHeight;
  ofRectangle previewRectangle;
  ofRectangle pictureRectangle;
  int         pictureWidth;
  int         pictureHeight;

  bool   isPhotoLoaded         = false;
  bool   isBackgroundGenerated = false;
  bool   isGaraLoaded          = false;
  bool   isHamonLoaded         = false;
  bool   isGenerating          = false;
  int    generatingCount       = 0;
  string generateTimestamp     = "";

  ofxDatGui            *gui;
  ofxDatGuiButton      *loadButton;
  ofxDatGuiMatrix      *garaUpperMatrix;
  ofxDatGuiMatrix      *garaLowerMatrix;
  ofxDatGuiButton      *generateButton;
  ofxDatGuiToggle      *printToggle;
  ofxDatGuiColorPicker *colorPicker;
  ofxDatGuiSlider      *previewFpsSlider;
  ofxDatGuiTextInput   *statusTextInput;

  string                     settingsXmlPath = ofFilePath::getUserHomeDir() + "/Desktop/settings.xml";
  ofDirectory                logDirectory    = ofDirectory(ofFilePath::getUserHomeDir() + "/Desktop/log");
  string                     serverUrl;
  ofDirectory                garaUpperDirectory;
  ofDirectory                garaLowerDirectory;
  vector <vector <ofImage> > garaUpperVector;
  vector <vector <ofImage> > garaLowerVector;
  int                        garaUpperKinds;
  int                        garaLowerKinds;
  int                        garaUpperCurrentKind = 0;
  int                        garaLowerCurrentKind = 0;
  ofDirectory                hamonDirectory;
  vector <ofImage>           hamonImages;
  int                        hamonNum;
  ofImage                    mojiImage;
  ofDirectory                outputDirectory;
  string                     outputPath;
  ofDirectory                archiveDirectory;
  string                     archivePath;
  string                     privateKeyPath;

  int            previewFps;
  int            resultFrames;
  ofFbo          fbo;
  unsigned char *picturePixel;
  ofImage        pictureImage;
  ofColor        keyColor;
  ofVboMesh      backgroundMesh;
  ofPixels       pixels;
  ofImage        generatingImage;
};

