#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    vidWidth = 320;  // try to grab at this size.
    vidHeight = 240;

    //get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();

    for (size_t i = 0; i < devices.size(); i++) {
        if (devices[i].bAvailable) {
            //log the device
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }
        else {
            //log the device and note it as unavailable
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }

    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.initGrabber(vidWidth, vidHeight);
    frameData.allocate(vidWidth, vidHeight, OF_PIXELS_GRAY);
    data1.allocate(frameData);
    data2.allocate(frameData);
    data3.allocate(frameData);
    ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(100, 100, 100);
    vidGrabber.update();

    ofPixelsRef pixelsRef = vidGrabber.getPixels();
    
    /* create training example input and output vectors here */

    if (isRecording) {
        for (int i = 0; i < vidWidth; i++) {
            for (int j = 0; j < vidHeight; j++) {
                // get the pixel and its lightness (lightness is the average of its RGB values)
                float lightness = pixelsRef.getColor(i, j).getLightness(); 
                //RAPIDMIX
                /* push input training data on to vector */

                frameData[i + (j * vidWidth)] = lightness;
            }
        }

        /* specify the training label,
        put the input data and label together,
        push the current training example onto the training data vector*/
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetHexColor(0xffffff);
    vidGrabber.draw(0, 20);

    ofSetColor(ofColor::beige);
    ofDrawBitmapString("Current label: " + ofToString((int)label), 20, 40 + vidGrabber.getHeight());
    ofDrawBitmapString("Number of training examples: " + ofToString(data.size()), 20, 68 + vidGrabber.getHeight());
    ofSetColor(ofColor::cyan);
    ofRectangle trainingExamplesBar;
    trainingExamplesBar.setPosition(20, 80 + vidGrabber.getHeight());
    trainingExamplesBar.setHeight(50);
    trainingExamplesBar.setWidth(data.size());
    ofDrawRectangle(trainingExamplesBar);

    if (label == 1.0f) {
        ofSetColor(255);
        data1.loadData(frameData);
    }
    else if (label == 2.0f) {
        ofSetColor(255);
        data2.loadData(frameData);
    }
    else if (label == 3.0f) {
        ofSetColor(255);
        data3.loadData(frameData);
    }
    data1.draw(vidGrabber.getWidth(), 20);
    data2.draw(2 * vidGrabber.getWidth(), 20);
    data3.draw(3 * vidGrabber.getWidth(), 20);

    ofSetColor(ofColor::beige);
    ofDrawBitmapString("Data Label 1", vidGrabber.getWidth() + 20, 40 + vidGrabber.getHeight());
    ofDrawBitmapString("Data Label 2", (2 * vidGrabber.getWidth()) + 20, 40 + vidGrabber.getHeight());
    ofDrawBitmapString("Data Label 3", (3 * vidGrabber.getWidth()) + 20 , 40 + vidGrabber.getHeight());

    ofDrawBitmapString("Instructions:\n\nPress 1, 2 or 3 to choose labels\nPress space to record training examples", 20, 200 + vidGrabber.getHeight());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'c' || key == 'C') data.clear();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch (key) {
        case 49:
            label = 1.0f;
            break;
        case 50:
            label = 2.0f;
            break;
        case 51:
            label = 3.0f;
            break;
        case 32:
            isRecording = !isRecording;
    }
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
