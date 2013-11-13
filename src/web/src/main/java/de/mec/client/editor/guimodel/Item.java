/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.editor.guimodel;

import com.google.web.bindery.autobean.shared.AutoBean;
import java.util.List;
import java.util.Map;

/**
 *
 * @author jan.hoelscher
 */
public interface Item {
    String getKey();
    String getValue();
    @AutoBean.PropertyName(value = "data")
    List<Map<String,String>>getOptionData();
    //List<String> getEnables();
}
