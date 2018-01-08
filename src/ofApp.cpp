#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    mFont.loadFont("Courier Bold", 16);
    gettimeofday(&mLastSampledTime, NULL);
    mGame.setup();

    mCurrentFPS = 60;
}

//--------------------------------------------------------------
void ofApp::update(){
    //sample elapsed time:
    struct timeval currtime;
    gettimeofday(&currtime, NULL);
    long seconds = currtime.tv_sec - mLastSampledTime.tv_sec;
    long useconds = currtime.tv_usec - mLastSampledTime.tv_usec;
    mLastSampledTime = currtime;
    float dt = round(seconds * 1000 + useconds / 1000.0) / 1000.0;

    mGame.update(dt);

    if(dt > 0)
        mCurrentFPS = MIN((int)roundf(1.0 / dt), 60);
    else 
        mCurrentFPS = 60;
}

//--------------------------------------------------------------
void ofApp::draw(){
    mGame.draw();

    ofSetColor(255,100,0);

    string txt = "FPS: " + ofToString(mCurrentFPS);
    mFont.drawString(txt, 5, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    mGame.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    mGame.onMouseDown(ofVec2f(x,y));
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