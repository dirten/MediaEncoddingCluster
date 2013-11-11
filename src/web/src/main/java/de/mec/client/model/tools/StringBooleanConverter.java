/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.model.tools;

import com.sencha.gxt.data.shared.Converter;

/**
 *
 * @author jan.hoelscher
 */
public class StringBooleanConverter implements Converter<String, Boolean> {

    public String convertFieldValue(Boolean object) {
        //if(object!=null)
        return object.toString();
    }

    public Boolean convertModelValue(String object) {
        return Boolean.parseBoolean(object);
    }
    
}
