//
//  ofxQuadWarp.hpp
//  example
//
//  Created by YOSHIDA Shinya on 2018/09/28.
//

#pragma once
#include "ofMain.h"
//#include "ofConstants.h"
//#include "glm/vec2.hpp"
//#include "glm/vec3.hpp"
//#include "ofColor.h"
//#include "ofMesh.h"

class ofxQuadWarp
{
public:
    void setup(int width, int height, int subdivX=4, int subdivY=4);
    void setupFromFile(string filename);
    void drawMesh(const ofTexture& tex);
    void enableController();
    void disableController();
    void saveToFile(string filename);
    bool isControllerEnabled() { return _controllerEnabled; }
    
    void mousePressed(ofMouseEventArgs &args);
    void mouseReleased(ofMouseEventArgs &args);
    void mouseMoved(ofMouseEventArgs &args);
    void mouseDragged(ofMouseEventArgs &args);
    void mouseScrolled(ofMouseEventArgs &args);
    void mouseEntered(ofMouseEventArgs &args);
    void mouseExited(ofMouseEventArgs &args);
    
private:
    const float POINT_RAD = 10.0f;
    
    ofMesh              _mesh;
    vector<glm::vec3>   _ctrlPoints;
    vector<glm::vec3>   _verts;
    glm::mat4           _curModelView, _curView;
    
    bool    _controllerEnabled = false;
    int     _pressedCtrlIndex = -1;
    int     _width, _height, _subdivX, _subdivY;
    
    bool mouseHitLocalSpace(glm::vec3 pt);
    glm::vec3 calcLocalSpaceMousePos();
    glm::vec3 calcScreenPos(glm::vec3 pt, glm::mat4 modelView);
    void updateControllers();
    void updateMesh();
    void drawController();
};
