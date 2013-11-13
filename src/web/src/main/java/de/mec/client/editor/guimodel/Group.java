/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.editor.guimodel;

import com.google.web.bindery.autobean.shared.AutoBean;

/**
 *
 * @author jan.hoelscher
 */
public interface Group {
    
    //@AutoBean.PropertyName(value = "id")
    String getId();

    //@AutoBean.PropertyName(value = "title")
    String getTitle();

    //@AutoBean.PropertyName(value = "tab")
    String getTab();

}
