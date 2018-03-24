#include "ofApp.h"

// --------------------------------------------------------------
void ofApp::setup()
{
  gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
  gui->setTheme(new ofxDatGuiThemeWireframe());
  gui->addHeader(":: animato ::");
  gui->addButton("Capture");
  gui->addMatrix("UPPER", 9);
  gui->addMatrix("LOWER", 9);
  gui->addFRM();
}

// --------------------------------------------------------------
void ofApp::update()
{
}

// --------------------------------------------------------------
void ofApp::draw()
{
}

// --------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}

// --------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

// --------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

// --------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

// --------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

// --------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

// --------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

// --------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

// --------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

// --------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

// --------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}


