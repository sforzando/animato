#include "ofApp.h"

void ofApp::setup()
{
  ofLogToFile(ofGetTimestampString("%Y%m%d") + ".log", true);
  ofSetWindowTitle("");
  ofSetVerticalSync(true);
  ofSetFrameRate(previewFps);
  windowRectangle.setSize(ofGetWidth(), ofGetHeight());
  gifRectangle.setSize(1080, 1080);
  previewRectangle.setSize(ofGetHeight(), ofGetHeight());
  pictureRectangle.setSize(360, 360);

  loadGara();
  loadHamon();

  gui = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
  gui->setTheme(new ofxDatGuiCustomFontSize);
  idFolder = gui->addFolder(": ID :");
  prefixTextInput = idFolder->addTextInput(" - Prefix", "");
  prefixTextInput->setInputType(ofxDatGuiInputType::ALPHA_NUMERIC);
  idTextInput = idFolder->addTextInput(" - ID", "");
  idTextInput->setInputType(ofxDatGuiInputType::NUMERIC);
  idFolder->expand();
  captureFolder = gui->addFolder(": Capture :");
  captureButton = captureFolder->addButton(" - [C]apture");
  captureButton->onButtonEvent([&](ofxDatGuiButtonEvent e) {
    ofLog(OF_LOG_NOTICE, "onCaptureButton()");
    capture();
  });
  loadButton = captureFolder->addButton(" - [L]oad");
  loadButton->onButtonEvent([&](ofxDatGuiButtonEvent e) {
    ofLog(OF_LOG_NOTICE, "onLoadButton()");
    loadPhoto();
  });
  captureFolder->expand();
  garaFolder = gui->addFolder(": Pattern :");
  garaUpperMatrix = garaFolder->addMatrix(" - Upper", garaUpperKinds);
  garaUpperMatrix->setRadioMode(true);
  garaUpperMatrix->onMatrixEvent([&](ofxDatGuiMatrixEvent e) {
    ofLog(OF_LOG_NOTICE, "onLoadButton()");
    garaUpperCurrentKind = e.child;
    ofApp::setStatusMessage("Upper Gara has been changed to " + ofToString(e.child));
  });
  garaLowerMatrix = garaFolder->addMatrix(" - Lower", garaLowerKinds);
  garaLowerMatrix->setRadioMode(true);
  garaLowerMatrix->onMatrixEvent([&](ofxDatGuiMatrixEvent e) {
    ofLog(OF_LOG_NOTICE, "onLoadButton()");
    garaUpperCurrentKind = e.child;
    ofApp::setStatusMessage("Lower Gara has been changed to " + ofToString(e.child));
  });
  garaFolder->expand();
  exportFolder = gui->addFolder(": Export :");
  exportButton = exportFolder->addButton(" - Export");
  exportButton->onButtonEvent([&](ofxDatGuiButtonEvent e) {
    ofLog(OF_LOG_NOTICE, "onExportBUtton()");
  });
  printToggle = exportFolder->addToggle("  - with QR");
  printToggle->setChecked(true);
  exportFolder->expand();
  colorPicker = gui->addColorPicker("Key Color");
  colorPicker->setColor(keyColor);
  colorPicker->onColorPickerEvent([&](ofxDatGuiColorPickerEvent e) {
    ofLog(OF_LOG_NOTICE, "onColorPicker()");
    keyColor              = e.color;
    isBackgroundGenerated = false;
    ofApp::setStatusMessage("The Key Color has been changed.");
  });
  previewFpsSlider = gui->addSlider("Preview FPS", 1.0, 60.0, previewFps);
  previewFpsSlider->bind(previewFps);
  previewFpsSlider->onSliderEvent([&](ofxDatGuiSliderEvent e) {
    ofSetFrameRate(previewFps);
  });
  gui->addFRM();
  statusTextInput = gui->addTextInput("Status");
  setStatusMessage("animato has been launched.");

  fbo.allocate(gifRectangle.width, gifRectangle.height, GL_RGBA32F_ARB);
  backgroundMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);

  photo.init();
}

void ofApp::update()
{
  if (photo.captureSucceeded()) {
    picturePixel = photo.capture();
    pictureImage.clear();
    pictureImage.setFromPixels(picturePixel, photo.getCaptureWidth(), photo.getCaptureHeight(), OF_IMAGE_COLOR, 0);
    pictureImage.resize(pictureRectangle.width, pictureRectangle.height);
    setStatusMessage("Loading of the photo has been completed.");
    isPhotoLoaded = true;
  }
  fbo.begin();
  ofClear(0);
  ofDisableSmoothing();
  ofEnableBlendMode(OF_BLENDMODE_ADD);
  ofEnableAlphaBlending();
  if (isPhotoLoaded) {
    getBackground().drawFaces();
    pictureImage.draw(gifRectangle.width - pictureRectangle.width, gifRectangle.height - pictureRectangle.height, pictureRectangle.width, pictureRectangle.height);
  }
  if (isGaraLoaded) {
    int currentUpperFrame = ofGetFrameNum() % garaUpperVector[0].size();
    int currentLowerFrame = ofGetFrameNum() % garaLowerVector[0].size();
    garaUpperVector[garaUpperCurrentKind][currentUpperFrame].draw(0, 0);
    garaLowerVector[garaLowerCurrentKind][currentLowerFrame].draw(0, 0);
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
  garaUpperKinds = garaUpperDirectory.listDir();
  garaUpperVector.resize(garaUpperKinds);
  for (int i = 0; i < garaUpperKinds; i++) {
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

  garaLowerKinds = garaLowerDirectory.listDir();
  garaLowerVector.resize(garaLowerKinds);
  for (int i = 0; i < garaLowerKinds; i++) {
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
  setStatusMessage("Loading of Japanese patterns has been completed.");
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
  setStatusMessage("Loading of moisture patterns has been completed.");
}

void ofApp::capture()
{
  ofLog(OF_LOG_NOTICE, "capture()");
  isPhotoLoaded         = false;
  isBackgroundGenerated = false;
  photo.exit();
  cameraCheck();
  photo.init();
  if (photo.isBusy()) {
    ofLog(OF_LOG_WARNING, "Camera is busy.");
  } else {
    photo.startCapture();
  }
}

bool ofApp::cameraCheck()
{
  ofSystem("killall PTPCamera");
  ofSystem("/usr/local/bin/gphoto2 --auto-detect");
  sysCommand.callCommand("/usr/local/bin/gphoto2 --debug --summary");

  return true;
}

void ofApp::loadPhoto()
{
  ofLog(OF_LOG_NOTICE, "loadPhoto()");
  ofFileDialogResult loadFileResult = ofSystemLoadDialog("Choose a photo");
  if (loadFileResult.bSuccess) {
    pictureImage = ofImage(loadFileResult.getPath());
    pictureImage.resize(pictureRectangle.width, pictureRectangle.height);
    isPhotoLoaded         = true;
    isBackgroundGenerated = false;
  }
}

void ofApp::setStatusMessage(string s, ofLogLevel level)
{
  ofLog(level, s);
  if (statusTextInput != nil) {
    statusTextInput->setText(s);
  }
  say(s);
}

void ofApp::say(string s)
{
  sysCommand.callCommand("say -v Alex " + s);
}


