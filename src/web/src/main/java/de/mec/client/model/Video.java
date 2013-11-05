/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.model;

import com.google.web.bindery.autobean.shared.AutoBean;
import com.google.web.bindery.autobean.shared.AutoBean.PropertyName;

/**
 *
 * @author jan.hoelscher
 */
public interface Video {
    
    @PropertyName(value="id")
    String getId();

    @PropertyName(value="id")
    void setId(String id);

    @PropertyName(value="b")
    String getBitrate();

    @PropertyName(value="b")
    void setBitrate(String rate);
}
