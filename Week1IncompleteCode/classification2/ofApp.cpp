#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
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

    sampleRate = 44100;
    bufferSize = 512;

    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);

    ofSoundStreamSettings settings;
    settings.setOutListener(this);
    settings.sampleRate = sampleRate;
    settings.numOutputChannels = 2;
    settings.numInputChannels = 0;
    settings.bufferSize = bufferSize;
    soundStream.setup(settings);

    kick.load(ofToDataPath("KickBassPedal_normComp.wav"));
    floorTom.load(ofToDataPath("FloorTom_norm.wav"));
    bigRackTom.load(ofToDataPath("BigRackTom_norm.wav"));
    smallRackTom.load(ofToDataPath("SmallRackTom_norm.wav"));
    rackTom.load(ofToDataPath("RackTom_norm.wav"));
    rim.load(ofToDataPath("Rim_norm.wav"));
    rimClick.load(ofToDataPath("RimClick_norm.wav"));

    isTrained = false;
    isRunning = false;
    isEmpty = true;
}

//--------------------------------------------------------------
void ofApp::exit() {
    ofSoundStreamStop();
    ofSoundStreamClose();
}

//--------------------------------------------------------------
void ofApp::update() {
    ofBackground(100, 100, 100);
    vidGrabber.update();

    ofPixelsRef pixelsRef = vidGrabber.getPixels();
    std::vector<double> trainingInput;
    std::vector<double> trainingLabel;

    for (int i = 0; i < vidWidth; i+=5) {
        for (int j = 0; j < vidHeight; j+=5) {
            // get the pixel and its lightness (lightness is the average of its RGB values)
            float lightness = pixelsRef.getColor(i, j).getLightness();
            //RAPIDMIX
            trainingInput.push_back(double(lightness));
            frameData[i + (j * vidWidth)] = lightness;
        }
    }

    if (isRecording) {
        trainingLabel = { label };
        rapidlib::trainingExample tempExample;
        tempExample.input = trainingInput;
        tempExample.output = trainingLabel;
        data.push_back(tempExample);
    }

    if (isRunning && isTrained) {
        /* run the knn on new input and output a class estimation */
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetHexColor(0xffffff);
    if(!isRunning) vidGrabber.draw(0, 20);

    ofSetColor(ofColor::beige);

    /* output the label to the screen */
    //ofDrawBitmapString("Current label: " + ofToString(), 20, 40 + vidGrabber.getHeight());

    ofDrawBitmapString("Number of training examples: " + ofToString(data.size()), 20, 68 + vidGrabber.getHeight());
    ofSetColor(ofColor::cyan);
    ofRectangle trainingExamplesBar;
    trainingExamplesBar.setPosition(20, 80 + vidGrabber.getHeight());
    trainingExamplesBar.setHeight(50);
    trainingExamplesBar.setWidth(data.size());
    ofDrawRectangle(trainingExamplesBar);

    if (label == 1.0f && !isRunning && isRecording) {
        ofSetColor(255);
        data1.loadData(frameData);
    }
    else if (label == 2.0f && !isRunning && isRecording) {
        ofSetColor(255);
        data2.loadData(frameData);
    }
    else if (label == 3.0f && !isRunning && isRecording) {
        ofSetColor(255);
        data3.loadData(frameData);
    }
    data1.draw(vidGrabber.getWidth(), 20);
    data2.draw(2 * vidGrabber.getWidth(), 20);
    data3.draw(3 * vidGrabber.getWidth(), 20);

    ofSetColor(ofColor::beige);
    ofDrawBitmapString("Data Label 1", vidGrabber.getWidth() + 20, 40 + vidGrabber.getHeight());
    ofDrawBitmapString("Data Label 2", (2 * vidGrabber.getWidth()) + 20, 40 + vidGrabber.getHeight());
    ofDrawBitmapString("Data Label 3", (3 * vidGrabber.getWidth()) + 20, 40 + vidGrabber.getHeight());

    ofDrawBitmapString("Instructions:\n\nPress 1, 2 or 3 to choose labels\nPress space to record training examples\nPress 't' to train the model\t\tModel Trained = " + ofToString(isTrained) + "\nPress 'r' to run the model\t\tModel Running = " + ofToString(isRunning) + "\nPress 'q' to reset the model\t\tModel Is Empty = " + ofToString(isEmpty), 20, 200 + vidGrabber.getHeight());
   
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer& output)
{
    for (int i = 0; i < output.getNumFrames(); ++i)
    {
        currentCount = (int)timer.phasor(8);//this sets up a metronome that ticks 8 times a second

        if (lastCount != currentCount)
        {//if we have a new timer int this sample, play the sound
            //This is a 16-step step sequencer based on Mike Z's ofxRapidLib example 

            /* use the knn output label to trigger samples */
            switch (playHead % 16)
            {
            case 1:
                kick.trigger();
                break;
            case 5:
                if ( == 3 ||  == 2) floorTom.trigger();
                break;
            case 7:
                if ( == 1) rackTom.trigger();
                break;
            case 9:
                if ( == 1) {
                    rim.trigger();
                }
                break;
            case 11:
                if ( == 2) {
                    bigRackTom.trigger();
                }
                break;
            case 13:
                if ( == 3) smallRackTom.trigger();
                break;
            case 15:
                if ( == 2) rimClick.trigger();
            }

            ++playHead;//iterate the playhead
            lastCount = 0;//reset the metrotest
        }

        double sequencerOut = kick.playOnce() + rackTom.playOnce() + floorTom.playOnce() + rim.playOnce() + smallRackTom.playOnce() + bigRackTom.playOnce() + rimClick.playOnce();

        mymix.stereo(sequencerOut, outputs, 0.5);
        output[i * output.getNumChannels()] = outputs[0];
        output[i * output.getNumChannels() + 1] = outputs[1];
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 'c' || key == 'C') data.clear();

    if (key == 't' || key == 'T') {
        knn.train(data);
        isTrained = true;
        isEmpty = false;
    }

    if (key == 'r' || key == 'R') {
        isRunning = !isRunning;
    }

    if (key == 'q' || key == 'Q') {
        knn.reset();
        isEmpty = true;
        isTrained = false;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
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
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
