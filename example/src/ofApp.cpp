#include "ofxVideoDelay.h"
#include "ofMain.h"

class ofApp : public ofBaseApp {
    ofVideoGrabber grabber;
    ofxVideoDelay videoDelay;
public:
    void setup() override {
        auto devices = grabber.listDevices();
        for(auto device : devices) {
            if(ofIsStringInString(device.deviceName, "FaceTime")) {
                ofLogNotice() << "set device to " << device.deviceName;
                grabber.setDeviceID(device.id);
            }
        }
        
        grabber.setup(1920, 1080);
        videoDelay.setup();
    }
    void update() override {
        grabber.update();
        if(grabber.isFrameNew()) {
            videoDelay.addTexture(grabber.getTexture());
        }
        videoDelay.update();
    }
    void draw() override {
        ofSetColor(255, 255, 255);
        grabber.draw(0, 0, 960, 540);
        const auto &fbo = videoDelay.getFbo();
        fbo.draw(960, 0, 960, 540);
        
        ofDrawBitmapStringHighlight(ofToString(videoDelay.getNumBufferedTexture()), 20, 20);
    }
};

int main() {
    ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 3);
    settings.resizable = true;
    settings.decorated = true;
    
    settings.setSize(1920, 540);
    
    std::shared_ptr<ofAppBaseWindow> window = ofCreateWindow(settings);
    std::shared_ptr<ofApp> app(new ofApp());
    ofRunApp(window, app);
    
    ofRunMainLoop();
}
