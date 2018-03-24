#include "ofApp.h"

void ofApp::setup()
{
  ofSetWindowTitle("animato");
  ofSetVerticalSync(true);
  ofEnableAlphaBlending();

  gui = new ofxDatGui(1080, 0);
  gui->setTheme(new ofxDatGuiCustomFontSize);
  buttonCapture = gui->addButton("[C]apture");
  buttonCapture->setLabelAlignment(ofxDatGuiAlignment::CENTER);
  buttonCapture->onButtonEvent(this, &ofApp::onButtonCapture);
  gui->addFRM();

  fbo.allocate(1080, 1080, GL_RGBA32F_ARB);

  photo.init();
  isCameraBusy = false;
}

void ofApp::update()
{
  if (photo.captureSucceeded()) {
    pixelPicture = photo.capture();
    imagePicture.setFromPixels(pixelPicture, photo.getCaptureWidth(), photo.getCaptureHeight(), OF_IMAGE_COLOR, 0);
    ofLog(OF_LOG_NOTICE, "Photo loading finished.");
  }
  fbo.begin();
  imagePicture.draw(0, 0);
  fbo.end();
}

void ofApp::draw()
{
  fbo.draw(0, 0);
}

void ofApp::keyPressed(int key)
{
}

void ofApp::keyReleased(int key)
{
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
}

void ofApp::dragEvent(ofDragInfo dragInfo)
{
}

void ofApp::gotMessage(ofMessage msg)
{
}

void ofApp::onButtonCapture(ofxDatGuiButtonEvent e)
{
  ofLog(OF_LOG_NOTICE, "onButtonCapture()");
  capture();
}

void ofApp::capture()
{
  ofLog(OF_LOG_NOTICE, "capture()");
  if (isCameraBusy) {
    ofLog(OF_LOG_WARNING, "Camera is busy.");
  } else {
    photo.startCapture();
  }
}


