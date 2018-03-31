#include "ofApp.h"

void ofApp::setup()
{
  // Load Settings
  settings.loadFile(settingsXmlPath);
  serverUrl          = settings.getValue("serverUrl", "photo.moisturesurge72.jp");
  garaUpperDirectory = ofDirectory(settings.getValue("garaUpperDirectoryPath", "./materials/gara/upper"));
  garaLowerDirectory = ofDirectory(settings.getValue("garaLowerDirectoryPath", "./materials/gara/lower"));
  hamonDirectory     = ofDirectory(settings.getValue("hamonDirectoryPath", "./materials/hamon"));
  mojiImage          = ofImage(settings.getValue("mojiImagePath", "./materials/moji.png"));
  gifWidth           = settings.getValue("gifWidth", 1080);
  gifHeight          = settings.getValue("gifHeight", 1080);
  pictureWidth       = settings.getValue("pictureWidth", 360);
  pictureHeight      = settings.getValue("pictureHeight", 360);
  outputDirectory    = ofDirectory(settings.getValue("outputDirectoryPath", "./output"));
  archiveDirectory   = ofDirectory(settings.getValue("archiveDirectoryPathPath", "./archive"));
  privateKeyPath     = settings.setValue("privateKeyPath", "./id_rsa");
  resultFrames       = settings.setValue("resultFrames", 8);
  previewFps         = settings.getValue("previewFps", 2);
  keyColor           = ofColor::fromHex(settings.getValue("keyColor", 0xffd1cd));

  if (!archiveDirectory.exists())
  {
    archiveDirectory.create();
  }
  archivePath = archiveDirectory.getAbsolutePath();
  if (!outputDirectory.exists())
  {
    outputDirectory.create();
  }
  outputPath = outputDirectory.getAbsolutePath();
  if (!logDirectory.exists())
  {
    logDirectory.create();
  }
  ofLogToFile(logDirectory.getAbsolutePath() + "/" + ofGetTimestampString("%Y%m%d") + ".log", true);
  ofSetWindowTitle("animato");
  ofSetVerticalSync(true);
  ofSetFrameRate(previewFps);
  windowRectangle.setSize(ofGetWidth(), ofGetHeight());
  gifRectangle.setSize(gifWidth, gifHeight);
  previewRectangle.setSize(ofGetHeight(), ofGetHeight());
  pictureRectangle.setSize(pictureWidth, pictureHeight);

  loadGara();
  loadHamon();

  gui = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
  gui->setTheme(new ofxDatGuiCustomFontSize);
  loadButton = gui->addButton("[L]oad");
  loadButton->onButtonEvent([&](ofxDatGuiButtonEvent e) {
    ofLog(OF_LOG_NOTICE, "onLoadButton()");
    loadPhoto();
  });
  garaUpperMatrix = gui->addMatrix("Upper", garaUpperKinds);
  garaUpperMatrix->setRadioMode(true);
  garaUpperMatrix->onMatrixEvent([&](ofxDatGuiMatrixEvent e) {
    ofLog(OF_LOG_NOTICE, "onGaraUpperMatrix()");
    ofApp::selectGaraUpper(e.child);
  });
  garaLowerMatrix = gui->addMatrix("Lower", garaLowerKinds);
  garaLowerMatrix->setRadioMode(true);
  garaLowerMatrix->onMatrixEvent([&](ofxDatGuiMatrixEvent e) {
    ofLog(OF_LOG_NOTICE, "onGaraLowerMatrix()");
    ofApp::selectGaraLower(e.child);
  });
  generateButton = gui->addButton("[G]enerate");
  generateButton->onButtonEvent([&](ofxDatGuiButtonEvent e) {
    ofLog(OF_LOG_NOTICE, "onGenerateBUtton()");
    ofApp::generateGif();
  });
  printToggle = gui->addToggle("  - with QR");
  printToggle->setChecked(true);
  colorPicker = gui->addColorPicker("Key Color");
  colorPicker->setColor(keyColor);
  colorPicker->onColorPickerEvent([&](ofxDatGuiColorPickerEvent e) {
    ofLog(OF_LOG_NOTICE, "onColorPicker()");
    keyColor = e.color;
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
  setStatusMessage("Application has been launched.");

  fbo.allocate(gifRectangle.width, gifRectangle.height, GL_RGBA32F_ARB);
  backgroundMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
}

void ofApp::update()
{
  fbo.begin();
  ofClear(0);
  ofDisableSmoothing();
  ofEnableAlphaBlending();
  if (isPhotoLoaded)
  {
    getBackground().drawFaces();
    pictureImage.draw(gifRectangle.width - pictureRectangle.width, gifRectangle.height - pictureRectangle.height, pictureRectangle.width, pictureRectangle.height);
  }
  if (isGaraLoaded)
  {
    int currentUpperFrame = ofGetFrameNum() % garaUpperVector[0].size();
    int currentLowerFrame = ofGetFrameNum() % garaLowerVector[0].size();
    garaUpperVector[garaUpperCurrentKind][currentUpperFrame].draw(0, 0);
    garaLowerVector[garaLowerCurrentKind][currentLowerFrame].draw(0, 0);
  }
  if (isHamonLoaded)
  {
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    hamonImages[ofGetFrameNum() % hamonNum].draw(0, 0);
    ofEnableAlphaBlending();
  }
  mojiImage.draw(0, 0);
  ofDisableAlphaBlending();
  ofEnableSmoothing();
  fbo.end();
}

void ofApp::draw()
{
  ofBackgroundHex(0xffffff);
  fbo.draw(0, 0, previewRectangle.width, previewRectangle.height);
  if (isGenerating)
  {
    generateGif();
  }
}

void ofApp::keyPressed(int key)
{
  ofLogNotice() << "keyPressed(): " << ofToString(key);
  switch (key)
  {
    case 'l':
      loadPhoto();
      break;
    case 'g':
      generateGif();
      break;
    case OF_KEY_RETURN:
      generateGif();
      break;
    case '/':
      printQr();
      break;
    case '1':
      selectGaraUpper(0);
      break;
    case '2':
      selectGaraUpper(1);
      break;
    case '3':
      selectGaraUpper(2);
      break;
    case '4':
      selectGaraUpper(3);
      break;
    case '5':
      selectGaraUpper(4);
      break;
    case '6':
      selectGaraUpper(5);
      break;
    case '7':
      selectGaraUpper(6);
      break;
    case '8':
      selectGaraUpper(7);
      break;
    case '9':
      selectGaraUpper(8);
      break;
    case 'q':
      selectGaraLower(0);
      break;
    case 'w':
      selectGaraLower(1);
      break;
    case 'e':
      selectGaraLower(2);
      break;
    case 'r':
      selectGaraLower(3);
      break;
    case 't':
      selectGaraLower(4);
      break;
    case 'y':
      selectGaraLower(5);
      break;
    case 'u':
      selectGaraLower(6);
      break;
    case 'i':
      selectGaraLower(7);
      break;
    case 'o':
      selectGaraLower(8);
      break;
    case 'p':
      selectGaraLower(9);
      break;
    default:
      break;
  }
}

void ofApp::keyReleased(int key)
{
  ofLogNotice() << "keyReleased(): " << ofToString(key);
}

void ofApp::windowResized(int w, int h)
{
  ofLog(OF_LOG_NOTICE, "windowResized()");
  if ((GUI_MIN_WIDTH < ofGetWidth() - ofGetHeight()))
  {
    gui->setWidth(ofGetWidth() - ofGetHeight());
  }
  else
  {
    gui->setWidth(GUI_MIN_WIDTH);
  }
  windowRectangle.setSize(ofGetWidth(), ofGetHeight());
  previewRectangle.setSize(windowRectangle.height, windowRectangle.height);
}

void ofApp::gotMessage(ofMessage msg)
{
  ofLog() << msg.message;
}

void ofApp::exit()
{
  // Save Settings
  settings.setValue("serverUrl", serverUrl);
  settings.setValue("garaUpperDirectoryPath", "./materials/gara/upper");
  settings.setValue("garaLowerDirectoryPath", "./materials/gara/lower");
  settings.setValue("hamonDirectoryPath", "./materials/hamon");
  settings.setValue("mojiImagePath", "./materials/moji.png");
  settings.setValue("gifWidth", gifWidth);
  settings.setValue("gifHeight", gifHeight);
  settings.setValue("pictureWidth", pictureWidth);
  settings.setValue("pictureHeight", pictureHeight);
  settings.setValue("outputDirectoryPath", "./output");
  settings.setValue("archiveDirectoryPathPath", "./archive");
  settings.setValue("privateKeyPath", "./id_rsa");
  settings.setValue("keyColor", keyColor.getHex());
  settings.setValue("previewFps", previewFps);
  settings.setValue("resultFrames", resultFrames);
  settings.saveFile(settingsXmlPath);
}

ofVboMesh ofApp::getBackground()
{
  if (!isBackgroundGenerated)
  {
    backgroundMesh.clear();

    // origin: TOP-LEFT
    backgroundMesh.addVertex(ofPoint(0, 0, 0));
    backgroundMesh.addColor(keyColor);

    // BOTTOM-LEFT
    backgroundMesh.addVertex(ofPoint(0, gifRectangle.height, 0));
    backgroundMesh.addColor(keyColor);

    // PICTURE-LEFT
    for (int y = 0; y < pictureRectangle.height; y++)
    {
      backgroundMesh.addVertex(ofPoint(gifRectangle.width - pictureRectangle.width, gifRectangle.height - y, 0));
      backgroundMesh.addColor(pictureImage.getColor(0, (pictureRectangle.height - 1) - y));
      if (y == 0)
      {
        ofLog() << pictureImage.getColor(0, pictureRectangle.height - y);
        ofLog() << pictureImage.getHeight();
      }
    }

    // PICTURE-TOP
    for (int x = 0; x <= pictureRectangle.width; x++)
    {
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
  garaUpperDirectory.sort();
  for (int i = 0; i < garaUpperKinds; i++)
  {
    ofDirectory      d       = ofDirectory(garaUpperDirectory.getPath(i));
    int              garaNum = d.listDir();
    vector <ofImage> v(garaNum);
    for (int j = 0; j < garaNum; j++)
    {
      string path = d.getPath(j);
      ofLog() << "Upper Gara: " << path;
      v[j] = ofImage(path);
    }
    garaUpperVector[i] = v;
  }

  garaLowerKinds = garaLowerDirectory.listDir();
  garaLowerVector.resize(garaLowerKinds);
  garaLowerDirectory.sort();
  for (int i = 0; i < garaLowerKinds; i++)
  {
    ofDirectory      d       = ofDirectory(garaLowerDirectory.getPath(i));
    int              garaNum = d.listDir();
    vector <ofImage> v(garaNum);
    for (int j = 0; j < garaNum; j++)
    {
      string path = d.getPath(j);
      ofLog() << "Lower Gara: " << path;
      v[j] = ofImage(path);
    }
    garaLowerVector[i] = v;
  }

  isGaraLoaded = true;
  setStatusMessage("Loading of Japanese patterns completed.");
}

void ofApp::loadHamon()
{
  ofLog(OF_LOG_NOTICE, "loadingHamon()");
  hamonNum = hamonDirectory.listDir();
  hamonImages.resize(hamonNum);
  for (int i = 0; i < hamonNum; i++)
  {
    hamonImages[i].load(hamonDirectory.getPath(i));
  }
  isHamonLoaded = true;
  setStatusMessage("Loading of moisture patterns completed.");
}

void ofApp::loadPhoto()
{
  ofLog(OF_LOG_NOTICE, "loadPhoto()");
  ofFileDialogResult loadFileResult = ofSystemLoadDialog("Choose a photo");
  if (loadFileResult.bSuccess)
  {
    pictureImage = ofImage(loadFileResult.getPath());
    pictureImage.resize(pictureRectangle.width, pictureRectangle.height);
    isPhotoLoaded         = true;
    isBackgroundGenerated = false;
  }
}

void ofApp::generateGif()
{
  ofApp::setStatusMessage("Generate process has been started.");

  ofSystem("cp -f " + outputPath + "/* " + archivePath + "/"); // Archive

  isGenerating      = true;
  generateTimestamp = ofGetTimestampString("%d%H%M%s");
  fbo.readToPixels(pixels);
  generatingImage.setFromPixels(pixels);
  generatingImage.save(outputPath + "/" + ofToString(generatingCount, 2, '0') + ".png");
  if (generatingCount < resultFrames)
  {
    generatingCount++;
  }
  else
  {
    ofSystem("/usr/local/bin/ffmpeg -i " + outputPath + "/00.png -vf palettegen -y " + outputPath + "/palette.png"); // Make Palette
    ofSystem("/usr/local/bin/ffmpeg -f image2 -r " + ofToString(previewFps) + " -i " + outputPath + "/%02d.png -i " + outputPath + "/palette.png -filter_complex paletteuse " + outputPath + "/" + generateTimestamp + ".gif");

    if (uploadGif() && printToggle->getChecked())
    {
      printQr();
    }
    isGenerating    = false;
    generatingCount = 0;

    setStatusMessage("Generate completed.");
  }
}

bool ofApp::uploadGif()
{
  ofApp::setStatusMessage("Upload process has been started.");
  ofSystem("scp -i " + ofFile(privateKeyPath).getAbsolutePath() + " " + outputPath + "/*.gif " + "clinique@" + serverUrl + ":/var/www/html/gif/");

  return true;
}

void ofApp::printQr()
{
  ofApp::setStatusMessage("Print process has been started.");
  ofSystem("/usr/local/bin/qrencode -o " + outputPath + "/qr.png -m 2 'http://" + serverUrl + "/index.php/?id=" + generateTimestamp + "'");
  ofSystem("lpr -o media=DC20 -o PageSize=DC20 -o fitplot " + outputPath + "/qr.png");
}

void ofApp::selectGaraUpper(int kind)
{
  for (int i = 0; i < garaUpperKinds; i++)
  {
    if (i == kind)
    {
      garaUpperMatrix->getChildAt(i)->setSelected(true);
    }
    else
    {
      garaUpperMatrix->getChildAt(i)->setSelected(false);
    }
  }
  garaUpperCurrentKind = kind;
  setStatusMessage("Upper Gara has been changed to " + ofToString(kind));
}

void ofApp::selectGaraLower(int kind)
{
  for (int i = 0; i < garaLowerKinds; i++)
  {
    if (i == kind)
    {
      garaLowerMatrix->getChildAt(i)->setSelected(true);
    }
    else
    {
      garaLowerMatrix->getChildAt(i)->setSelected(false);
    }
  }
  garaLowerCurrentKind = kind;
  setStatusMessage("Lower Gara has been changed to " + ofToString(kind));
}

void ofApp::setStatusMessage(string s, ofLogLevel level)
{
  ofLog(level, s);
  if (statusTextInput != nil)
  {
    statusTextInput->setText(s);
  }
  say(s);
}

void ofApp::say(string s)
{
  sysCommand.callCommand("say -v Alex " + s);
}

