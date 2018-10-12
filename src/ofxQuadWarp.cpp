//
//  ofxQuadWarp.cpp
//  example
//
//  Created by YOSHIDA Shinya on 2018/09/28.
//

#include "ofxQuadWarp.hpp"
#include "glm/gtx/matrix_decompose.hpp"

using namespace std;
using namespace glm;

void ofxQuadWarp::setup(int width, int height, int subdivX, int subdivY)
{
    _width = width;
    _height = height;
    _subdivX = subdivX;
    _subdivY = subdivY;
    
    // generate mesh
    float dx = (float)_width/(float)_subdivX;
    float dy = (float)_height/(float)_subdivY;
    
    _mesh.clear();
    for(int y=0; y<=_subdivY; ++y) {
        for(int x=0; x<=_subdivX; ++x) {
            _mesh.addVertex(vec3(x*dx, y*dy, 0));
            _mesh.addTexCoord(vec2(x*dx, y*dy));
        }
    }
    
    for(int y=0; y<_subdivY; ++y) {
        for(int x=0; x<_subdivX; ++x) {
            int index = (_subdivX+1)*y + x;
            int pt0 = index;
            int pt1 = pt0 + 1;
            int pt2 = index + _subdivX + 1;
            int pt3 = pt2 + 1;
            _mesh.addIndex(pt0);
            _mesh.addIndex(pt2);
            _mesh.addIndex(pt1);
            _mesh.addIndex(pt1);
            _mesh.addIndex(pt2);
            _mesh.addIndex(pt3);
        }
    }
    
    // add control points
    resetMesh();
}


void ofxQuadWarp::resetMesh()
{
    _ctrlPoints.clear();
    _ctrlPoints.push_back(vec3(0, 0, 0));
    _ctrlPoints.push_back(vec3(_width, 0, 0));
    _ctrlPoints.push_back(vec3(0, _height, 0));
    _ctrlPoints.push_back(vec3(_width, _height, 0));
    updateMesh();
}


void ofxQuadWarp::enableController()
{
    if(_controllerEnabled) return;
    ofRegisterMouseEvents(this);
    _controllerEnabled = true;
}


void ofxQuadWarp::disableController()
{
    if(!_controllerEnabled) return;
    ofRegisterMouseEvents(this);
    _controllerEnabled = false;
}


void ofxQuadWarp::drawMesh(const ofTexture& tex)
{
    tex.bind();
    _mesh.draw();
    tex.unbind();
}


void ofxQuadWarp::drawController()
{
    // store current matrices
    _curModelView = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW);
    _curView = ofGetCurrentViewMatrix();
    
    ofPushStyle();
    ofSetColor(ofColor::white);
    ofDrawLine(_ctrlPoints[0], _ctrlPoints[1]);
    ofDrawLine(_ctrlPoints[1], _ctrlPoints[3]);
    ofDrawLine(_ctrlPoints[0], _ctrlPoints[2]);
    ofDrawLine(_ctrlPoints[2], _ctrlPoints[3]);
    
    ofSetColor(ofColor::green);
    
    for(int i=0; i<_ctrlPoints.size(); ++i) {
        
        // dirty for now. should be cleaned up.
        if(_pressedCtrlIndex == i) {
            _ctrlPoints[i] = calcLocalSpaceMousePos();
            ofFill();
        }
        else if(mouseHitLocalSpace(_ctrlPoints[i]))
            ofFill();
        else ofNoFill();
        
        ofDrawCircle(_ctrlPoints[i], POINT_RAD);
    }
    
    ofPopStyle();
    
    updateMesh();
}


bool ofxQuadWarp::mouseHitLocalSpace(glm::vec3 pt)
{
    return glm::distance(pt, calcLocalSpaceMousePos()) < POINT_RAD;
}

vec3 ofxQuadWarp::calcLocalSpaceMousePos()
{
    vec4 mousePos = vec4(ofGetMouseX(), ofGetMouseY(), 0, 1);
    return inverse(_curModelView) * _curView * mousePos;
}

vec3 ofxQuadWarp::calcScreenPos(glm::vec3 pt, mat4 modelView)
{
    return glm::inverse(_curView) * _curModelView * vec4(pt, 1);
}


void ofxQuadWarp::setupFromFile(string filename)
{
    // Should check if the file exists!
    ofBuffer buff = ofBufferFromFile(filename);
    string inTxt = buff.getText();
    vector<string> data = ofSplitString(buff.getText(), "_");
    _width = stoi(data[0]);
    _height = stoi(data[1]);
    _subdivX = stoi(data[2]);
    _subdivY = stoi(data[3]);
    setup(_width, _height, _subdivX, _subdivY);
    
    int offset=4;
    for(int i=0; i<_ctrlPoints.size(); ++i) {
        _ctrlPoints[i] = vec3(stoi(data[offset]), stoi(data[offset+1]), stoi(data[offset+2]));
        offset+=3;
    }
    updateMesh();
}

void ofxQuadWarp::saveToFile(string filename)
{
    string outTxt = "";
    outTxt += ofToString(_width) + "_";
    outTxt += ofToString(_height) + "_";
    outTxt += ofToString(_subdivX) + "_";
    outTxt += ofToString(_subdivY) + "_";
    for(vec3 pt : _ctrlPoints) {
        outTxt += ofToString(pt.x) + "_";
        outTxt += ofToString(pt.y) + "_";
        outTxt += ofToString(pt.z) + "_";
    }
    outTxt.pop_back();
    ofBuffer buff;
    buff.append(outTxt);
    ofBufferToFile(filename, buff);
}


void ofxQuadWarp::updateMesh()
{
    // solve top edge
    for (int x=0; x<=_subdivX; ++x) {
        int vertIndex = x;
        float ratio = (float)x / (float)_subdivX;
        _mesh.setVertex(vertIndex, lerp(_ctrlPoints[0], _ctrlPoints[1], ratio));
    }
    
    // solve bottom edge
    for (int x=0; x<=_subdivX; ++x) {
        int vertIndex = _subdivY*(_subdivX+1) + x;
        float ratio = (float)x / (float)_subdivX;
        _mesh.setVertex(vertIndex, lerp(_ctrlPoints[2], _ctrlPoints[3], ratio));
    }

    // solve left edge
    for (int y=1; y<_subdivY; ++y) {
        int vertIndex = y*(_subdivX+1);
        float ratio = (float)y / (float)_subdivY;
        _mesh.setVertex(vertIndex, lerp(_ctrlPoints[0], _ctrlPoints[2], ratio));
    }

    // solve right edge
    for (int y=1; y<_subdivY; ++y) {
        int vertIndex = y*(_subdivX+1) + _subdivX;
        float ratio = (float)y / (float)_subdivY;
        _mesh.setVertex(vertIndex, lerp(_ctrlPoints[1], _ctrlPoints[3], ratio));
    }
    
    // solve inside
    for (int y=1; y<_subdivY; ++y) {
        for (int x=1; x<_subdivX; ++x) {
            float ratioY = (float)y / (float)_subdivY;
            int lerpIndexA = x;
            int lerpIndexB = _subdivY*(_subdivX+1) + x;
            int vertIndex = y*(_subdivX+1) + x;
            _mesh.setVertex(vertIndex, lerp(_mesh.getVertices()[lerpIndexA], _mesh.getVertices()[lerpIndexB], ratioY));
        }
    }
}


void ofxQuadWarp::mousePressed(ofMouseEventArgs &args)
{
    for(int i=0; i<_ctrlPoints.size(); ++i) {   
        if(mouseHitLocalSpace(_ctrlPoints[i])) {
            _pressedCtrlIndex = i;
            break;
        }
    }
}

void ofxQuadWarp::mouseReleased(ofMouseEventArgs &args)
{
    _pressedCtrlIndex = -1;
}

void ofxQuadWarp::mouseMoved(ofMouseEventArgs &args){}
void ofxQuadWarp::mouseDragged(ofMouseEventArgs &args){}
void ofxQuadWarp::mouseScrolled(ofMouseEventArgs &args){}
void ofxQuadWarp::mouseEntered(ofMouseEventArgs &args){}
void ofxQuadWarp::mouseExited(ofMouseEventArgs &args){}
