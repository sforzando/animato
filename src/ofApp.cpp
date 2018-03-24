#include "ofApp.h"

void ofApp::setup()
{
  ofSetWindowTitle("animato");
  ofSetVerticalSync(true);
  ofEnableAlphaBlending();
  windowRectangle.setSize(ofGetWidth(), ofGetHeight());
  gifRectangle.setSize(1080, 1080);
  previewRectangle.setSize(ofGetHeight(), ofGetHeight());
  photoRectangle.setSize(360, 360);

  gui = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
  gui->setTheme(new ofxDatGuiCustomFontSize);
  buttonCapture = gui->addButton("[C]apture");
  buttonCapture->setLabelAlignment(ofxDatGuiAlignment::CENTER);
  buttonCapture->onButtonEvent(this, &ofApp::onButtonCapture);
  gui->addFRM();

  fbo.allocate(gifRectangle.width, gifRectangle.height, GL_RGBA32F_ARB);

  photo.init();
}

void ofApp::update()
{
  if (photo.captureSucceeded()) {
    pixelPicture = photo.capture();
    imagePicture.clear();
    imagePicture.setFromPixels(pixelPicture, photo.getCaptureWidth(), photo.getCaptureHeight(), OF_IMAGE_COLOR, 0);
    ofLog(OF_LOG_NOTICE, "Photo loading finished.");

    fbo.begin();
    imagePicture.draw(gifRectangle.width - photoRectangle.width, gifRectangle.height - photoRectangle.height, photoRectangle.width, photoRectangle.height);
    fbo.end();
  } else { }
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

void ofApp::onButtonCapture(ofxDatGuiButtonEvent e)
{
  ofLog(OF_LOG_NOTICE, "onButtonCapture()");
  capture();
}

void ofApp::capture()
{
  ofLog(OF_LOG_NOTICE, "capture()");
  photo.exit();
  photo.init();
  if (photo.isBusy()) {
    ofLog(OF_LOG_WARNING, "Camera is busy.");
  } else {
    photo.startCapture();
  }
}


