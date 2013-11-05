/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.model;

import com.google.web.bindery.autobean.shared.AutoBean;

/**
 *
 * @author jan.hoelscher
 */
public interface Codec {
    /*
    public enum CODEC_TYPE {
        VIDEO_CODEC(0),
        AUDIO_CODEC(1);
        
        private int numVal;

        CODEC_TYPE(int numVal) {
            this.numVal = numVal;
        }

        public int getNumVal() {
            return numVal;
        }
    }
    */
    String getId();
    void setId(String id);
    
    @AutoBean.PropertyName(value="longname")
    String getLongName();

    @AutoBean.PropertyName(value="longname")
    void setLongName(String name);
    
    @AutoBean.PropertyName(value="type")
    int getCodecType();
    
}
