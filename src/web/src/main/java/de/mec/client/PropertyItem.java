/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client;

import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author jan.hoelscher
 */
public class PropertyItem {
    
    private String _key;
    private String _val;

    public String getKey() {
        return _key;
    }

    public String getVal() {
        return _val;
    }

    public void setKey(String _key) {
        this._key = _key;
    }

    public void setVal(String _val) {
        this._val = _val;
    }

    public PropertyItem(final String key, final String value){
        _key=key;
        _val=value;
    }
    /*
    public String getValue() {
        LOG.log(Level.INFO,"getValue:");
        return _val;
    }

    public String setValue(String value) {
        LOG.log(Level.INFO,"setValue:"+value);
        String result=_val;
        _val=value;
        return result;
    }*/
    private static final Logger LOG = Logger.getLogger(PropertyItem.class.toString());
    
}
