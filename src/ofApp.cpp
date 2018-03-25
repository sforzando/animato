#include "ofApp.h"

void ofApp::setup()
{
  ofSetWindowTitle("");
  ofSetVerticalSync(true);
  windowRectangle.setSize(ofGetWidth(), ofGetHeight());
  gifRectangle.setSize(1080, 1080);
  previewRectangle.setSize(ofGetHeight(), ofGetHeight());
  pictureRectangle.setSize(360, 360);

  gui = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
  gui->setTheme(new ofxDatGuiCustomFontSize);
  captureButton = gui->addButton("[C]apture");
  //  captureButton->setLabelAlignment(ofxDatGuiAlignment::CENTER);
  captureButton->onButtonEvent(this, &ofApp::onCaptureButton);
  loadButton = gui->addButton("[L]oad");
  //  loadButton->setLabelAlignment(ofxDatGuiAlignment::CENTER);
  loadButton->onButtonEvent(this, &ofApp::onLoadButton);
  gui->addFRM();

  fbo.allocate(gifRectangle.width, gifRectangle.height, GL_RGBA32F_ARB);
  backgroundMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);

  loadGara();
  loadHamon();

  photo.init();
}

void ofApp::update()
{
  if (photo.captureSucceeded()) {
    picturePixel = photo.capture();
    pictureImage.clear();
    pictureImage.setFromPixels(picturePixel, photo.getCaptureWidth(), photo.getCaptureHeight(), OF_IMAGE_COLOR, 0);
    pictureImage.resize(pictureRectangle.width, pictureRectangle.height);
    ofLog(OF_LOG_NOTICE, "Photo loading finished.");
    isPhotoLoaded = true;
  }
  fbo.begin();
  ofDisableSmoothing();
  ofEnableBlendMode(OF_BLENDMODE_ADD);
  ofEnableAlphaBlending();
  if (isPhotoLoaded) {
    getBackground().drawFaces();
    pictureImage.draw(gifRectangle.width - pictureRectangle.width, gifRectangle.height - pictureRectangle.height, pictureRectangle.width, pictureRectangle.height);
  }
  if (isGaraLoaded) {
    garaUpperVector[0][0].draw(0, 0);
    garaLowerVector[0][0].draw(0, 0);
  }
  if (isHamonLoaded) {
    hamonImages[ofGetFrameNum() % hamonNum].draw(0, 0);
  }
  mojiImage.draw(0, 0);
  ofDisableAlphaBlending();
  ofEnableSmoothing();
  fbo.end();
}

void ofApp::draw()
{
  ofBackgroundHex(0x000000);
  fbo.draw(0, 0, previewRectangle.width, previewRectangle.height);
}

void ofApp::keyPressed(int key)
{
  ofLogNotice() << "keyPressed(): " << ofToString(key);
  switch (key) {
    case 'c':
      capture();
      break;
    case 'l':
      loadPhoto();
      break;
    default:
      break;
  }
}

void ofApp::keyReleased(int key)
{
  ofLogNotice() << "keyReleased(): " << ofToString(key);
}

void ofApp::mouseMoved(int x, int y)
{
}

void ofApp::mouseDragged(int x, int y, int button)
{
}

void ofApp::mousePressed(int x, int y, int button)
{
}

void ofApp::mouseReleased(int x, int y, int button)
{
}

void ofApp::mouseEntered(int x, int y)
{
}

void ofApp::mouseExited(int x, int y)
{
}

void ofApp::windowResized(int w, int h)
{
  ofLog(OF_LOG_NOTICE, "windowResized()");
  gui->setWidth(ofGetWidth() - ofGetHeight());
  windowRectangle.setSize(ofGetWidth(), ofGetHeight());
  previewRectangle.setSize(windowRectangle.height, windowRectangle.height);
}

void ofApp::dragEvent(ofDragInfo dragInfo)
{
}

void ofApp::gotMessage(ofMessage msg)
{
  ofLog() << msg.message;
}

void ofApp::exit()
{
  photo.exit();
}

ofVboMesh ofApp::getBackground()
{
  if (!isBackgroundGenerated) {
    backgroundMesh.clear();

    // origin: TOP-LEFT
    backgroundMesh.addVertex(ofPoint(0, 0, 0));
    backgroundMesh.addColor(keyColor);


    // BOTTOM-LEFT
    backgroundMesh.addVertex(ofPoint(0, gifRectangle.height, 0));
    backgroundMesh.addColor(keyColor);

    // PICTURE-LEFT
    for (int y = 0; y < pictureRectangle.height; y++) {
      backgroundMesh.addVertex(ofPoint(gifRectangle.width - pictureRectangle.width, gifRectangle.height - y, 0));
      backgroundMesh.addColor(pictureImage.getColor(0, (pictureRectangle.height - 1) - y));
      if (y == 0) {
        ofLog() << pictureImage.getColor(0, pictureRectangle.height - y);
        ofLog() << pictureImage.getHeight();
      }
    }

    // PICTURE-TOP
    for (int x = 0; x <= pictureRectangle.width; x++) {
      backgroundMesh.addVertex(ofPoint(gifRectangle.width - pictureRectangle.width + x, gifRectangle.height - pictureRectangle.height, 0));
      backgroundMesh.addColor(pictureImage.getColor(x, 0));
    }

    // TOP-RIGHT
    backgroundMesh.addVertex(ofPoint(gifRectangle.width, 0, 0));
    backgroundMesh.addColor(keyColor);

    isBackgroundGenerated = true;
  }

  return backgroundMesh;
}

void ofApp::loadGara()
{
  ofLog(OF_LOG_NOTICE, "loadGara()");
  int upperNum = garaUpperDirectory.listDir();
  garaUpperVector.resize(upperNum);
  for (int i = 0; i < upperNum; i++) {
    ofDirectory      d       = ofDirectory(garaUpperDirectory.getPath(i));
    int              garaNum = d.listDir();
    vector <ofImage> v(garaNum);
    for (int j = 0; j < garaNum; j++) {
      string path = d.getPath(j);
      ofLog() << "upper: " << path;
      v[j] = ofImage(path);
    }
    garaUpperVector[i] = v;
  }

  int lowerNum = garaLowerDirectory.listDir();
  garaLowerVector.resize(lowerNum);
  for (int i = 0; i < lowerNum; i++) {
    ofDirectory      d       = ofDirectory(garaLowerDirectory.getPath(i));
    int              garaNum = d.listDir();
    vector <ofImage> v(garaNum);
    for (int j = 0; j < garaNum; j++) {
      string path = d.getPath(j);
      ofLog() << "lower: " << path;
      v[j] = ofImage(path);
    }
    garaLowerVector[i] = v;
  }

  isGaraLoaded = true;
}

void ofApp::loadHamon()
{
  ofLog(OF_LOG_NOTICE, "loadingHamon()");
  hamonNum = hamonDirectory.listDir();
  hamonImages.resize(hamonNum);
  for (int i = 0; i < hamonNum; i++) {
    hamonImages[i].load(hamonDirectory.getPath(i));
  }
  isHamonLoaded = true;
}

void ofApp::onCaptureButton(ofxDatGuiButtonEvent e)
{
  ofLog(OF_LOG_NOTICE, "onCaptureButton()");
  capture();
}

void ofApp::capture()
{
  ofLog(OF_LOG_NOTICE, "capture()");
  isPhotoLoaded         = false;
  isBackgroundGenerated = false;
  photo.exit();
  ofLog(OF_LOG_NOTICE, ofSystem("killall PTPCamera"));
  photo.init();
  ofLog(OF_LOG_NOTICE, ofSystem("/usr/local/bin/gphoto2 --auto-detect"));
  ofLog(OF_LOG_VERBOSE, ofSystem("/usr/local/bin/gphoto2 --debug --summary"));
  if (photo.isBusy()) {
    ofLog(OF_LOG_WARNING, "Camera is busy.");
  } else {
    photo.startCapture();
  }
}

void ofApp::onLoadButton(ofxDatGuiButtonEvent e)
{
  ofLog(OF_LOG_NOTICE, "onLoadButton()");
  loadPhoto();
}

void ofApp::loadPhoto()
{
  ofLog(OF_LOG_NOTICE, "loadPhoto()");
  ofFileDialogResult loadFileResult = ofSystemLoadDialog("");
  if (loadFileResult.bSuccess) {
    pictureImage = ofImage(loadFileResult.getPath());
    pictureImage.resize(pictureRectangle.width, pictureRectangle.height);
    isPhotoLoaded         = true;
    isBackgroundGenerated = false;
  }
}


