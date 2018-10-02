#include "ofApp.h"

using namespace glm;

//--------------------------------------------------------------
void ofApp::setup(){
    _image.load("check256.png");
    _quadWarp.setup(_image.getWidth(),_image.getHeight(), 8, 8);
//    _quadWarp.setupFromFile("mesh.txt")
    _quadWarp.enableController();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(0);
    
    ofPushMatrix();
    ofTranslate(100, 100, 0);
    ofScale(0.5f, 0.5f);
    _quadWarp.drawMesh(_image.getTexture());
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == 'c') {
        if(!_quadWarp.isControllerEnabled())
            _quadWarp.enableController();
        else
            _quadWarp.disableController();
    }
    else if(key == 's') {
        _quadWarp.saveToFile("mesh.txt");
        _quadWarp.setupFromFile("mesh.txt");
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
