/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.model;

import com.google.web.bindery.autobean.shared.AutoBean.PropertyName;

/**
 *
 * @author jan.hoelscher
 */
public interface Profile {

        @PropertyName(value="uuid")
        String getUuid();

        @PropertyName(value="name")
        String getName();

        void setName(String name);
        @PropertyName(value="audio")
        Audio getAudio();
        
        @PropertyName(value="audio")
        void setAudio(final Audio audio);

        @PropertyName(value="video")
        Video getVideo();

        @PropertyName(value="video")
        void setVideo(final Video video);

        @PropertyName(value="format")
        Format getFormat();

        @PropertyName(value="format")
        void setFormat(final Format video);
}

