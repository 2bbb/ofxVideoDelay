//
//  ofxVideoDelay.h
//
//  Created by 2bit on 2024/01/14.
//

#ifndef ofxVideoDelay_h
#define ofxVideoDelay_h

#include "ofUtils.h"
#include "ofFbo.h"

#include <memory>
#include <vector>
#include <algorithm>

namespace ofx {
    namespace bbb {
        struct VideoDelay {
            struct TimedTexture {
                float time;
                std::shared_ptr<ofFbo> fbo;
            };
            
            void setup(float bufferDuration = 2.0f,
                       float delayTime = 1.0f)
            {
                setBufferDuration(bufferDuration);
                setDelayTime(delayTime);
            }
            
            void update() {
                auto t = ofGetElapsedTimef();
                auto it = std::remove_if(buffer.begin(),
                                         buffer.end(),
                                         [this, t](const TimedTexture &buf) {
                                             auto b = buf.time + bufferDuration < t;
                                             if(b) fbo_pool.push_back(buf.fbo);
                                             return b;
                                         });
                buffer.erase(it, buffer.end());
            }
            
            const ofFbo &getFbo() const {
                return getFbo(delayTime);
            }
            
            const ofFbo &getFbo(float delayTime) const {
                static ofFbo fbo;
                if(buffer.empty()) return fbo;
                auto t = ofGetElapsedTimef();
                auto it = std::min_element(buffer.cbegin(),
                                           buffer.cend(),
                                           [delayTime, t](const TimedTexture &x, const TimedTexture &y) {
                                               return std::fabs(t - (x.time + delayTime)) < std::fabs(t - (y.time + delayTime));
                                           });
                return *it->fbo;
            }
            
            void addTexture(const ofTexture &tex) {
                buffer.emplace_back();
                TimedTexture &buf = buffer.back();
                if(fbo_pool.empty()) {
                    buf.fbo = std::make_shared<ofFbo>();
                    buf.fbo->allocate(tex.getWidth(), tex.getHeight());
                } else {
                    buf.fbo = fbo_pool.back();
                    fbo_pool.pop_back();
                }
                
                buf.time = ofGetElapsedTimef();
                buf.fbo->begin();
                tex.draw(0, 0);
                buf.fbo->end();
            }
            
            void setBufferDuration(float bufferDuration) {
                this->bufferDuration = bufferDuration;
            }
            
            void setDelayTime(float delayTime) {
                this->delayTime = delayTime;
            }
            
            std::size_t getNumBufferedTexture() const {
                return buffer.size();
            }
            
        protected:
            float bufferDuration{2.0f};
            float delayTime{1.0f};
            std::vector<TimedTexture> buffer;
            std::vector<std::shared_ptr<ofFbo>> fbo_pool;
        };
    };
};

using ofxVideoDelay = ofx::bbb::VideoDelay;

#endif /* ofxVideoDelay_h */
