//
//  ofxDatGuiCustomFontSize.h
//  Chromoly
//
//  Created by Shin'ichiro SUZUKI on 2016/11/25.
//
//

#pragma once
#include "ofxDatGuiTheme.h"

class ofxDatGuiCustomFontSize: public ofxDatGuiTheme {
public:
  ofxDatGuiCustomFontSize()
  {
    // WIREFRAME
    stripe.visible = false;
    color.label = hex(0x6E6E6E);
    color.icons = hex(0x6E6E6E);
    color.background = hex(0xFAFAFA);
    color.guiBackground = hex(0xD8D8DB);
    color.inputAreaBackground = hex(0xE9E9E9);
    color.slider.fill = hex(0x6E6E6E);
    color.slider.text = hex(0x6E6E6E);
    color.textInput.text = hex(0x6E6E6E);
    color.textInput.highlight = hex(0xFCFAFD);
    color.colorPicker.border = hex(0xDFDDDF);
    color.textInput.backgroundOnActive = hex(0xD1D1D1);
    color.backgroundOnMouseOver = hex(0xECECEC);
    color.backgroundOnMouseDown = hex(0xDFDDDF);
    color.matrix.normal.button = hex(0xDFDDDF);
    color.matrix.hover.button = hex(0x9C9DA1);
    color.matrix.selected.button = hex(0x6E6E6E);
    color.pad2d.line = hex(0x6E6E6E);
    color.pad2d.ball = hex(0x6E6E6E);
    color.graph.fills = hex(0x6E6E6E);
    
    font.size = 9;
    font.file = "M+2VM+IPAG-circle.ttf";
    layout.upperCaseLabels = false;
    
    init();
  }
};
